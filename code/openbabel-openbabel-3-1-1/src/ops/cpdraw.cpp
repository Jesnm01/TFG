/**********************************************************************
cpdraw.cpp - An OBOp for Cp (cyclopentadienyl) detection and svg depiction

Copyright (C) 2023 by Jesus N. M.

This file is part of the Open Babel project.
For more information, see <http://openbabel.org/>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
***********************************************************************/
#include <openbabel/babelconfig.h>
#include <iostream>
#include<openbabel/op.h>
#include<openbabel/mol.h>
#include<openbabel/bond.h>
#include <openbabel/obiter.h>
#include <openbabel/elements.h>
#include <openbabel/oberror.h>
#include <openbabel/ring.h>
#include <openbabel/cpcomplex.h>
#include <openbabel/generic.h>
#include <openbabel/obmolecformat.h>
#include "../formats/smilesformat.cpp"

using namespace std;

namespace OpenBabel
{
    class OpCpDraw : public OBOp
    {
    public:
        OpCpDraw(const char* ID) : OBOp(ID, false) {};
        const char* Description() {
            return
                "Cp estructure-like detection and svg depiction.\n"
                "Works in conjunction with 'gen2D' plugin for 2D coordinates generation"
                "Jesus N. M.";
        }
        virtual bool WorksWith(OBBase* pOb) const { return dynamic_cast<OBMol*>(pOb) != nullptr; }
        virtual bool Do(OBBase* pOb, const char* OptionText = nullptr, OpMap* pOptions = nullptr, OBConversion* pConv = nullptr);
        bool isCpBond(OBBond* bond, unsigned int idxM); //Metodo que comprueba si a priori podría ser un enlace tipo Cp
        bool FindRingWithCarbon(vector<OBRing*>& rlist, int carbonIdx, OBRing*& result);
        void CanonizeOgm(OBMol* mol, OBConversion* pConv);


    };

    /////////////////////////////////////////////////////////////////
    OpCpDraw theOpCpDraw("CpDraw"); //Global instance

    /////////////////////////////////////////////////////////////////
    bool OpCpDraw::Do(OBBase* pOb, const char* OptionText, OpMap* pOptions, OBConversion* pConv)
    {
        OBMol* pmol = dynamic_cast<OBMol*>(pOb);
        if (!pmol)
            return false;

        /*
            Apuntes proceso canonizacion:
                - Solo aplicarlo a moleculas que tengan metales de interes para organometalica (sino, esq el algoritmo no funciona porque se basan en esos atomos centrales) (si lo quiero aplicar a todo SMILES, quizas pueda coger el de mayor peso atomico como atomo central, en caso de no tener metal digo)
                - Si el smiles es inconexo, no aplicamos nada. Es decir, suponemos que el SMILES de entrada tiene una buena conectividad, pero comprobamos que no tenga puntos (yo no puedo saber/inventarme los enlaces entre atomos)
                  ESTO de arriba creo que si sirve, porque el algoritmo que lo hace ya tiene esto en cuenta. (obviamente, solo reordenará cada fragmento individual por separado, dejar claro eso en las reglas)
                - centralidad del metal (conexiones directas que tiene, y cada una de ellas es el inicio de un bloque)
                - Problema: si hay varios metales, es muy comun esto, tengo que darle prioridad a uno de ellos para que sea el central. Si son 2 metales iguales, tengo que ver que hacer
                - REVISAR LA CLASE DE SMILESFORMAT.CPP, tiene metodos para canonizar ya hechos y estructuras tipo arbol que me pueden servir mucho para escribir el Smiles de salida

        */

        //Sacamos el SMILES Canonico e identificamos los bloques
        CanonizeOgm(pmol, pConv);
        cout << "CanSmiles: " << pmol->GetCanSmiles() << "\n";
        



        //Algoritmo de deteccion de Cp
        /*Quedarse con el metal que tiene enlace con el 1º carbono. Comprobar que los demas carbonos de seguido, tb tienen enlace con ese metal en concreto. 
        Ver cuantos carbonos hay de ese estilo, y quedarse con la cantidad (esto determinará el tipo de ciclo Cp (de 5, de 6, de 4, lo que sea...
        para luego hacer el dibujo predeterminado con la misma cantidad de vertices))*/
        /* Para saber si es Cp-like:
            - que el carbono esté en un anillo
            - que esos carbonos estén enlazados tambien con el mismo metal
            - una vez detectado el nº de carbonos del anillo, algunas comprobaciones de seguridad para descartar falsos positivos en las detecciones*/
        vector<int> test(pmol->NumBonds(),0);
        vector<pair<int,int>> cpBonds(pmol->NumBonds(), std::make_pair(-1,-1)); //Por defecto lo ponemos a -1. con esto comprobamos tb que si el valor es -1, no es un cpbond
        OBAtom* begin;
        OBAtom* end;
        int contador = 0;
        int atomNMetal = -1; 
        unsigned int idxMetal = 0;
        bool cpTest = false;
        FOR_BONDS_OF_MOL(b,pmol) {
            begin = b->GetBeginAtom();
            end = b->GetEndAtom();
            if (isCpBond(&*b, idxMetal)) {
                if (!idxMetal)
                    (begin->IsOgmMetal()) ? idxMetal = begin->GetIdx() : idxMetal = end->GetIdx();
                cpBonds[b->GetIdx()] = make_pair(begin->GetIdx(), end->GetIdx());
            }

            //Debug
            contador++;
            
        }
        

        //Si no detecta un metal de Cp, acabamos
        if (idxMetal) {


            std::vector<std::vector<pair<int, int>>> individualCpBonds;
            std::vector<pair<int, int>> temp_cp;
            int usedCarbons = 0;
            BranchBlock* newBranch = nullptr, * currentBranch = nullptr;
            unsigned int cnt_carbon; //current carbon
            for (int i = 0; i < cpBonds.size(); i++) {
                if (cpBonds[i].first != -1) {
                    //Sacamos el idx del carbono 
                    (pmol->GetBond(i)->GetBeginAtom()->IsOgmMetal()) ? //no sabemos si el metal es el begin o el end
                        cnt_carbon = pmol->GetBond(i)->GetEndAtom()->GetIdx() :
                        cnt_carbon = pmol->GetBond(i)->GetBeginAtom()->GetIdx();
                    newBranch = pmol->FindBranch(cnt_carbon);
                    if (newBranch) {
                        if (!currentBranch) //Si es el inicio del bloque que tratamos, lo asignamos a current 
                            currentBranch = newBranch;
                        if (newBranch == currentBranch) { //Si es la misma branch, es que seguimos identificando el mismo cp que la iteracion anterior
                            temp_cp.push_back(cpBonds[i]);
                            usedCarbons++;
                        }
                        else {
                            //Ha encontrado un bloque pero es distinto que el anterior, por lo que ya hemos acabado con el cp anterior 
                            //(puede que lo haya encontrado entero, en cuyo caso lo metemos definitaivamente a individualCps; 
                            //o puede que no todos los carbonos del bloque se hayan usado, por lo que no lo metemos)

                            if (usedCarbons == currentBranch->Size()) { //todo el bloque usado
                                individualCpBonds.push_back(temp_cp);
                            }

                            //Reseteamos el contador para el siguiente bloque
                            usedCarbons = 0;
                            currentBranch = nullptr;
                            temp_cp.clear();
                            i--; //Volvemos a la interacion anterior para que no se salte el carbono que ha hecho cambiar de bloque
                        }
                    }
                    else {//No me ha encontrado una branch en la que esté el carbono

                    }
                }
                else {
                }
                //Si es el ultimo elemento del vector, comprobamos si nos dejamos algun bloque sin insertar
                if (i == cpBonds.size() - 1)
                    if (usedCarbons == currentBranch->Size()) { //todo el bloque usado
                        individualCpBonds.push_back(temp_cp);
                    }
            }



            //Esto cambiarlo por el conteo de .size() del vector que me cree con los indices de los Cp, una vez pueda parsear el SMILES de nuevo
            int cpBondSum = 0;
            for (int i = 0; i < cpBonds.size(); i++) {
                if (cpBonds[i].first != -1) cpBondSum += 1;
            }

            /*----------FALLO----------------
            AQUI ME PUEDE DETECTAR CP EN MOLECULAS TIPO MOL_3. Hay 4 enlaces tipo Cp, pero el resto del anillo no está enlazado con el metal, por lo que no seria cp, pero si me lo detecta
            TENDRIA QUE HACER UNA COMPROBACION EXTRA:
                - cojo un carbono de los que tengo marcado un bondCp, y compruebo si todos los demas carbonos del mismo anillo son tambien parte de un bondCp.
                Si si, seguimos, si no, falsa alarma (parecia Cp pero no es cp finalmente) y salimos del metodo.
                ESTO TAMPOCO FUNCIONA, PORQUE LOS ANILLOS LOS DIVIDEN SEGUN SSSR, siendo tuplas de 3 atomos en los tipo Cp. Esto lo complica todo. (para los bencenos y anillos normales si lo arregla)
            -----------------------------------
            */

            //Algunas comprobaciones de seguridad
            // -    Comprobar que el metal tiene al menos el tantos bonds como carbonos detectamos en los ciclos
            // -    Que no me detecte carbonos aislados (que quizas sean parte de un anillo, pero el anillo entero no sea Cp) 
            //      Cojo un carbono de los que tengo marcado un bondCp, y compruebo si todos los demas carbonos del mismo anillo son tambien parte de un bondCp
            //Esto es para que si despues del bucle, tenemos bonds que tienen pinta de Cp, pero luego vemos que el metal en cuestion no tiene enlaces suficientes, algo ha ido mal, o realmente no eran Cp bonds
            OBAtom* atom;
            //OBAtom* carbon;
            int carbonIdx = 0;
            //OBBond* bond_tmp;
            vector<OBRing*> rlist;
            vector<OBRing*>::iterator itr;
            OBAtomIterator it;
            OBRing* ringCarbon = nullptr;
            vector<int> rpath;
            bool goodInsert = true;

            if (pmol->GetAtom(idxMetal)->GetExplicitDegree() < cpBondSum) {
                obErrorLog.ThrowError(__FUNCTION__, "Failed to detect Cp structure, metal has not enough bonds", obWarning);
                return false;
            }

            /*----- Para cada uno de los cp individuales detectados ------*/
            for (int icp = 0; icp < individualCpBonds.size(); icp++) {
                CpComplex* cp = new CpComplex;
                unsigned int cpMetalIdx;
                std::vector<pair<int, int>>cpIndvBonds = individualCpBonds[icp];

                (pmol->GetAtom(cpIndvBonds[0].first)->IsOgmMetal()) ? //no sabemos si el metal es el first o el second
                    cpMetalIdx = pmol->GetAtom(cpIndvBonds[0].first)->GetIdx() :
                    cpMetalIdx = pmol->GetAtom(cpIndvBonds[0].second)->GetIdx();
                cp->SetMetalIdx(cpMetalIdx);

                int idxInsert;
                for (int i = 0; i < cpIndvBonds.size(); i++) {
                    (pmol->GetAtom(cpIndvBonds[i].first)->IsOgmMetal()) ?
                        idxInsert = pmol->GetAtom(cpIndvBonds[i].second)->GetIdx() :
                        idxInsert = pmol->GetAtom(cpIndvBonds[i].first)->GetIdx();
                    cp->AddIdxCarbon(idxInsert);
                    atom = pmol->GetAtom(idxInsert);
                    cp->AddCpAtom(atom);
                }

                //Descartar carbonos aislados para no modificar anillos erroneos
                //Para moleculas con enlaces como la mol_3, quizas pueda meter una excepcion, porque el resultado me gusta como queda (habria que no eliminar los bonds al final, y levantar todo el anillo o algo asi. Pero claro, es muy especifico)
                //Sacamos la lista de anillos totales de la molecula
                if (!pmol->HasLSSRPerceived())
                    pmol->FindLSSR();

                rlist = pmol->GetLSSR();

                //De momento, solo puedo comprobar esto con 1 anillo. Tenemos el mismo problema de separar los Cp. En cpBonds tengo todos los posibles enlaces: no se cuando salgo de comprobar 1 y me meto en otro 
                //Esto cuando tenga la separacion de Cps, tendre que cambiarlo
                map<int, bool> atomVisited; //map<key:idx_carbon, bool:checked_good>
                for (auto id : cp->GetIdxCarbons()) //Relleno el map con los mismos idx de los carbonos del cp
                    atomVisited.emplace(id, 0);
                vector<unsigned int> cppath = cp->GetIdxCarbons();
                for (int i = 0; i < cp->GetCarbonsSize() && goodInsert; i++) {
                    carbonIdx = cp->GetCarbonIdx(i);
                    //if (cpBonds[i].first != -1) {
                        //bond_tmp = pmol->GetBond(i);
                        //carbon = bond_tmp->GetEndAtom();

                    if (atomVisited.at(carbonIdx) == 0) {
                        /*Esto deberia hacer que devolviera una lista de obbrings, por si el carbono está en varios anillos distintos, y luego comprobar todos los anillos.
                        Simplemente viendo si todos los carbonos del _path están en el Cp, seria valido. Si el Cp tiene menos, el Cp está mal detectado y no se insertará en la molecula. Lo mismo si tiene más.
                        Esto no funciona del todo para todas las moleculas. Mol3 da problemas*/
                        if (FindRingWithCarbon(rlist, carbonIdx, ringCarbon)) {
                            rpath = ringCarbon->_path;

                            for (int j = 0; j < rpath.size(); j++) {
                                if (pmol->GetAtom(rpath[j])->GetAtomicNum() == 6) { //Si es un carbono (lo compruebo, porque en los ciclos de cp, normalmente se dividen en ciclos mas pequeños de 3 (C-C-M), y para saltarme el metal)
                                    auto it = find(cppath.begin(), cppath.end(), rpath[j]);
                                    if (it != cppath.end()) {
                                        //Me ha encontrado el idx del rpath[i] en el cp. Aniado ese atomo como visto
                                        if (atomVisited[*it] == 0) atomVisited.at(*it) = 1;
                                    }
                                    else { //No me ha encontrado el idx de un rpath donde el carbono que estabamos comprobando si estaba, pero rpth[i] no está, por lo que no son el mismo anillo. Abortamos
                                        goodInsert = false;
                                        break;
                                    }
                                }
                            }
                            //Si los encuentra todos, ese anillo contiene todos los carbonos 
                        }

                    }

                    //}
                }

                //Si es valido lo podemos insertar en la molecula.
                if (goodInsert) {
                    cp->SetParent(pmol);
                    pmol->AddCpComplex(*cp);
                    //Sabiendo que es valido, marcamos todos los atomos del cp con un flag especial (servirá mas tarde en el dibujado para detectar estos atomos en concreto)
                    for (atom = cp->BeginAtomCp(it); atom; atom = cp->NextAtomCp(it)) {
                        atom->SetInCp();
                    }
                }
                //else { //Si idxMetal sigue siendo 0, es que no hay ningun Cp en la molecula, por lo que lo dejamos así, como si no se hubiera hecho nada
                //}

                //cout << "\n";


                //cout << "Coordenadas dentro de cpdraw Do antes de tocar nada: \n";
                //for (int i = 1; i <= pmol->NumAtoms(); i++) {
                //    atom = (pmol->GetAtom(i));
                //    //cout << "[idx= " << atom->GetIdx() << "][atomic_number: " << atom->GetAtomicNum() << "] x: " << atom->GetX() << "; y: " << atom->GetY() << "\n";
                //}
                cout << "\n\n";


                /* Cambiamos las coordenadas de los carbonos en cuestion
                2 Opciones:
                    - Poner las coordenadas a ojo que yo quiera con respecto al metal y la orientacion almacenada
                    - Mover las coordenadas que ya tienen, una cantidad 'x' arbitraria en direccion a la orientacion. Esto mantendrá el ratio de bond lenght de 1 presumiblemente
                translate all atoms an offset position according to the orientation Cp has stored

                Necesito calcular la direccion en la que trasladar los carbonos, y la cantidad que quiero moverlos en X e Y
                Como minimo tengo que mover los carbonos en la direccion correcta para que el mas alejado del metal, quede por encima/sobrepase (mayor Y o menor Y en funcion de la orientacion) el metal y un poco mas para darle espacio
                P.ej: si Fe (x=-0.86, y=0.5) orientacion hacia arriba. El carbono más bajo tiene que estar como minimo por encima de y=0.5 + 'espacio' (espacio siendo el valor que yo quiera, ire probando)
                Para eso tengo que ver que carbono es el mas lejano, y calcular la diferencia de distancias con el metal segun la orientacion*/

                /*
                * De momento, como solamente estoy trabajando con Cps individuales, me voy a simplificar esto un poco, y los voy a colocar siempre hacia arriba. Para cuando tenga 2 compuestos, colocaré uno hacia arriba y otro hacia abajo, y así...
                * Lo de la orientacion es muy buena idea, pero se me está complicando.
                */
                //Comprobamos si se ha detectado algun Cp. Si no, no hacemos nada y salimos del metodo
                //if (idxMetal && pmol->HasCp()) {

                //Usamos el Cp (si existe) que hemos aniadido a la molecula (es distinto del que hemos creado antes, ya que internamnete al añadirlo, se copia en uno nuevo)
                cp = pmol->GetCpComplex(icp + 1);
                if (cp) {
                    double offsetX = 0.0, offsetY = 0.0;
                    OBAtom* atomMetal;
                    atomMetal = pmol->GetAtom(idxMetal);

                    // find min/max values de los carbonos del Cp
                    //CpComplex* cp;
                    //cp = pmol->GetCpComplex(1); //de momento solo trabajamos con 1 cp

                    double min_x, max_x;
                    double min_y, max_y;
                    double min_z, max_z;
                    atom = cp->BeginAtomCp(it);
                    if (atom != nullptr) {
                        min_x = max_x = atom->GetX();
                        min_y = max_y = atom->GetY();
                        min_z = max_z = atom->GetZ();
                        for (atom = cp->NextAtomCp(it); atom; atom = cp->NextAtomCp(it)) {
                            min_x = std::min(min_x, atom->GetX());
                            max_x = std::max(max_x, atom->GetX());
                            min_y = std::min(min_y, atom->GetY());
                            max_y = std::max(max_y, atom->GetY());
                            min_z = std::min(min_z, atom->GetZ());
                            max_z = std::max(max_z, atom->GetZ());
                        }
                    }
                    cout << "min_x: " << min_x << "\n"; cout << "max_x: " << max_x << "\n";
                    cout << "min_y: " << min_y << "\n"; cout << "max_y: " << max_y << "\n\n";

                    /*Antes de cambiar las coordenadas de los carbonos, vamos a quitar de en medio cualquier otro atomo que esté por esa zona. Tendria que calcular el bounding box de alguna forma, y ver si algun atomo cae dentro. si es asi, cambiarle las coordenadas
                    Esto es un poco chapuza, seguramente solo funcione con enlaces simples que estén por ahi sueltos. Para estrcuturas mas complejas esto no sirve

                    Poner las coordenadas con respecto al metal (esto seguramente no haga falta al final, o si, tengo que mirarlo.)
                    Calcular las distancias entre el metal y cada carbono.Si veo que las X son todas del mismo signo, quiere decir que todos los carbonos están a un lado del metal, y puedo quedarme con la menor distancia, para saber el X que tengo que desplazar los carbonos
                    -Si todas las X son positivas o negativas sabemos que los carbonos están todos a un lado
                    -Si todas las Y son positivas o negativas sabemos que los carbonos están encima o debajo
                    -Si tienen signo distinto en las X, significa que algunos carbonos están a la izquierda y otros a la derecha del metal
                    -Si tienen signo distinto en las Y, significa que algunos carbonos están por encima y otros por debajo del metal
                    -Si hay mismo signo en Y y distinto en X, es que está por encima del metal, que es lo que queremos conseguir (no tenemos que cambiar las coordenadas)
                    deberia cambiar lo de abajo 'fabs(...) con estas reglas'
                    */

                    //En realidad, de momento solamente me interesa las coordenadas en Y, puesto que voy a poner el Cp por encima del metal
                    //Calculo la diferencia entre la Y del metal y la minima Y de los carbonos. Esa será la distancia minima que tengo que trasladar los atomos hacia arriba
                    offsetY = fabs(atomMetal->GetY() - min_y);
                    double spacing = 1; //Espacio extra

                    //cout << "Modificamos las coordenadas de los carbonos Cp:\n";
                    for (atom = cp->BeginAtomCp(it); atom; atom = cp->NextAtomCp(it)) {
                        atom->SetVector(atom->GetX(), atom->GetY() + offsetY + spacing, atom->GetZ());
                        //cout << "[idx= " << atom->GetIdx() << "][atomic_number: " << atom->GetAtomicNum() << "] x: " << atom->GetX() << "; y: " << atom->GetY() << "\n";
                    }

                    // create new dummy atom entre el metal y el centro del Cp (esto se calcula haciendo la media entre las coordenadas de todos los carbonos, para X e Y)
                    vector3 centroidCp;
                    cp->FindCentroid();
                    centroidCp = cp->GetCentroid();
                    cout << "Centroid cp: " << centroidCp << "\n";



                    //Sacamos el radio del circulo (el poligono incial es irregular, pero bueno, sacamos una distancia mas o menos viable y en base a eso lo hacemos regular. Osea, la distancia a los carbonos son iguales, pero a los lados no)
                    double result = 0.0;
                    vector3 tmp = (centroidCp)-(pmol->GetAtom(cp->GetCarbonIdx(0)))->GetVector();
                    result = tmp.length();
                    cout << "Distancia centroide-carbonos: " << result << "\n";


                    //TODO ESTE PROCESO DE TRANSFORMACION DE LOS CARBONOS LO PODRIA METER EN UN METODO Y YA
                    //Movemos los carbonos en forma de poligono regular en base al centro calculado de centroidCp con un radio de centroide-carbono
                    int n_lados = cp->GetCarbonsSize();
                    double radioPentagono = result;
                    double alpha, _x, _y, _z;
                    for (int i = 0; i < n_lados; i++) {
                        alpha = 2 * M_PI * i / n_lados;
                        _x = centroidCp.x() + radioPentagono * cos(alpha); //Para usar un origen distinto de (0,0), sumamos (x,y)
                        _y = centroidCp.y() + radioPentagono * sin(alpha);
                        pmol->GetAtom(cp->GetCarbonIdx(i))->SetVector(_x, _y, 0.0);
                    }

                    //cout << "Coordenadas de los carbonos Cp en poligono regular :\n";
                    //for (atom = cp->BeginAtomCp(it); atom; atom = cp->NextAtomCp(it)) {
                    //    cout << "[idx= " << atom->GetIdx() << "][atomic_number: " << atom->GetAtomicNum() << "] x: " << atom->GetX() << "; y: " << atom->GetY() << "\n";
                    //}




                    //Calculamos de nuevo el centroid y la distancia, esta vez con el poligono regular y ya rotado en perspectiva
                    cp->FindCentroid();
                    centroidCp = cp->GetCentroid();
                    cout << "Nuevo Centroid cp: " << centroidCp << "\n";

                    //Mover de nuevo los carbonos para darle perspectiva al pentagono
                    //Aplicamos una rotacion en un eje paralelo al Eje de coordenadas X. Dicho eje quiero que pase justamente por el centro del poligono para que la rotacion del poligono sea sobre su centro (centroide) (por lo que utilizamos las coordenadas del centroide en la formula)
                    OBAtom* _atom;
                    double alphaR = deg2rads(PERSPECTIVE_DEG);
                    for (int i = 0; i < n_lados; i++) {
                        _atom = pmol->GetAtom(cp->GetCarbonIdx(i));
                        _y = _atom->GetY() * cos(alphaR) - _atom->GetZ() * sin(alphaR) + (centroidCp.GetY() * (1.0 - cos(alphaR)) + _atom->GetZ() * sin(alphaR));
                        _z = _atom->GetY() * sin(alphaR) + _atom->GetZ() * cos(alphaR) + (centroidCp.GetZ() * (1.0 - cos(alphaR)) - _atom->GetY() * sin(alphaR));
                        _atom->SetVector(_atom->GetX(), _y, _z);                            // Al ser rotacion en eje X, la x queda igual
                    }


                    //Creacion del circulo central y su rotacion
                    vector3 _coord;
                    int circle_sides = 30;
                    radioPentagono = result * 0.6;
                    double _cx = 0.0, _cy = 0.0, _cz = 0.0;
                    double __cy, __cz;
                    for (int i = 0; i < circle_sides; i++) {
                        alpha = 2 * M_PI * i / circle_sides;
                        _cx = centroidCp.x() + radioPentagono * cos(alpha); //Para usar un origen distinto de (0,0), sumamos (x,y)
                        _cy = centroidCp.y() + radioPentagono * sin(alpha);

                        //lo rotamos directamente antes de meterlo
                        __cy = _cy * cos(alphaR) - _cz * sin(alphaR) + (centroidCp.GetY() * (1.0 - cos(alphaR)) + _cz * sin(alphaR));
                        __cz = _cy * sin(alphaR) + _cz * cos(alphaR) + (centroidCp.GetZ() * (1.0 - cos(alphaR)) - _cz * sin(alphaR));
                        _coord.Set(_cx, __cy, __cz);
                        cp->AddCircleCoord(_coord);
                    }

                    /*cout << "Coordenadas del ciruclo Cp en perspectiva :\n";
                    for (int i = 0; i < circle_sides; i++) {
                        _coord = cp->GetCircleCoord(i);
                        cout << "[" << i << "] x: " << _coord.GetX() << "; y: " << _coord.GetY() << "\n";
                    }*/


                    OBAtom* dummy;
                    dummy = pmol->NewAtom();
                    dummy->SetAtomicNum(0);
                    dummy->SetVector(centroidCp);

                    cp->SetDummyIdx(dummy->GetIdx());
                    // bond dummy atom to center metal
                    pmol->AddBond(idxMetal, dummy->GetIdx(), 1);
                    //Esto va bien, se crea el bond dummy y el dibujado pinta la linea al centro del Cp como queria. Pero como es dummy, elem==0, pinta un asterisco como si fuera un radical o algo asi. (arregaldo)
                    //UNA VEZ DIBUJADO EL SVG, DEBERIA ELIMINAR ESTE ATOM Y EL BOND CREADO, POR SI LUEGO SE HACE ALGUNA QUE OTRA OPERACION CON LA MOLECULA NO JODER NADA. (esto en vd no creo que haga falta, si solamente le pido el dibujado, cuando acaba, se destruye todo)

                    cout << "Dummy bond created \n";
                }
                
            }
            //Si trabajamos con 1 cp solamente (de momento), lo que tengo que hacer es crearme un struct CpComplex metiendo el idx_metal, y cada uno de los idx end() de los bonds que sean ==1


            /*Varias maneras para dibujar el pan:
                1.- puedo mover los carbonos, crear el bond con el centroide, y eliminar los bonds de los carbonos-M para quitar las lineas.
                Eliminar los bonds puede ser peligroso si luego se van a realizar mas operaciones con la molecula, pero si solamente se va a dibujar, hace el apaño
                (lo engorroso será si alguno de los carbonos del Cp, tiene alguna rama. Aunuqe ahora que lo pienso ese bond sigue estando, solamnete estoy quitando el bond C-M)
             */

             /*------ Quitar los bonds M - C / C - M -----*/
            OBBond* bondToDelete;
            OBAtom* metal, * carbono;

            for (int i = 1; i <= pmol->GetCpSize(); i++) {
                CpComplex* cp = pmol->GetCpComplex(i);

                for (int i = 0; i < cp->GetCarbonsSize(); i++) {
                    metal = pmol->GetAtom(cp->GetMetalIdx());
                    carbono = pmol->GetAtom(cp->GetCarbonIdx(i));
                    bondToDelete = pmol->GetBond(metal, carbono); //Me da igual quien sea el begin o el end, el metodo saca el bond entre los atomos especificados, pero no es estricto en ese sentido
                    pmol->DeleteBond(bondToDelete);
                }


                cout << "Cp Bonds deleted. New molecule bond list: \n";
                FOR_BONDS_OF_MOL(b, pmol) {
                    begin = b->GetBeginAtom();
                    end = b->GetEndAtom();
                    cout << "[" << begin->GetIdx() << "]" << OBElements::GetSymbol(begin->GetAtomicNum()) << "-"
                        << "[" << end->GetIdx() << "]" << OBElements::GetSymbol(end->GetAtomicNum()) << "\n";
                }
            }
        }


            //PARA HACER EL CIRCULO DEL PAN, PUEDO COGER EL CENTROIDE (en definitiva, las coords del dummy atom) Y LA DISTANCIA A CUALQUIERA DE LOS CARBONOS, Y METER CODIGO XML DONDE TOQUE, CON EL PUNTO DEL CENTROIDE Y UN POCO MENOS DE LA DISTANCIA CALCULADA
            //Para que esto funcione el poligono que forme el anillo de carbonos deberia ser regular. Siendo irregular, la distancia del centroide a cada uno de los lados no es la misma
            //Para que quede bien, deberia quitar tb las rayas de los dobles enlaces y el punto del radical. Esto ya si que deberia hacerlo con la variable esta del atom.h que indique si es parte de cp o no, o con un flag (investigar esto)
        /*
            - Tengo que marcar los carbonos como aromaticos (esto hace que el ring con esos carbonos sea aromatico). Luego, recorrer la lista de bonds, y si el bgn y el end atoms son aromaticos, marcar el bond con order 5.Para poder en los metodos de drawRing, poner la condicion
        
            - Llegará el momento en el que tenga mas de 1 cp complex. Por lo que tendré que trabajar iterando. Para dibujar varios, tendré que guardar toda la informacion relacionada con los Cp en el struct
              (el radio, calculado aqui; el centro, calculado aqui; la orientacion, calculado no se donde; y demas cosas que me harna falta en un futuro para dibujarlo correctamente)
              Tendré que hacerme metodos propios parecidos a los que ya hay de DrawRing y todo eso, para pasar por parametro el CpComplex que me interese. Puedo comporbar si el ring que está dibujando
              es el mismo que mi CpComplex, mirando si los idx de los carbonos del Cp son los mismos que los del path del ring. Asi sabrñe cuándo está dibujando el ring de mi Cp, para luego meterle yo el circulo central
        
            - PUEDO PINTAR SIEMPRE EL CP HACIA ARRIBA, Y LUEGO ROTARLO (con centro de rotacion el metal) EN FUNCION DE LA ORIENTACION QUE TENGA (quizas tenga que cambiar la orientacion por un angulo o algo asi)
        */



        return true;
    }    


    bool OpCpDraw::isCpBond(OBBond* bond, unsigned int idxM)
    {
        if (bond->GetBondOrder() != 1)
            return false;

        OBAtom* M = nullptr, * C = nullptr; //M for metal and C for carbon

        OBAtom* begin = bond->GetBeginAtom();
        //bool idxGood = true;
        /*if (idxM)
            (begin->GetIdx() == idxM) ? idxGood = true : idxGood = false;*/
        if (begin->IsOgmMetal() /*&& idxGood*/) //Si es un Ogm metal y el idx de ese metal es el mismo que los anteriores bonds
            M = begin;
        if (begin->GetAtomicNum() == 6)
            C = begin;

        OBAtom* end = bond->GetEndAtom();
        if (end->IsOgmMetal())
            M = end;
        if (end->GetAtomicNum() == 6)
            C = end;

        if (!M || !C)
            return false;

        /*if (M->GetExplicitDegree() < 10)
            return false;*/

        return C->IsInRing();
    }

    bool OpCpDraw::FindRingWithCarbon(vector<OBRing*> &rlist, int carbonIdx, OBRing* &result) //Esto puede resultar en error, porque devuelve el 1º anillo que encuentre que cumpla la condicion. Pero el mismo carbono puede estar en varios anillos, y no ser el 1º que encuentre el que queremos comprobar
    {
        vector<OBRing*>::iterator itr;
        for (itr = rlist.begin(); itr != rlist.end(); ++itr)
            if ((*itr)->IsInRing(carbonIdx)){
                result = (*itr);
                return (true);
            }
        return (false);
    }

    void OpCpDraw::CanonizeOgm(OBMol* pmol, OBConversion* pConv)
    {
        OBBitVec fragatoms(pmol->NumAtoms());

        OBPairData* dp = (OBPairData*)pmol->GetData("SMILES_Fragment");
        const char* ppF = pConv->IsOption("F");
        if (dp) {
            fragatoms.FromString(dp->GetValue(), pmol->NumAtoms());
        }
        else if (ppF) { // Use info from option "F"
            fragatoms.FromString(ppF, pmol->NumAtoms());
        }
        // If no "SMILES_Fragment" data, fill the entire OBBitVec
        // with 1's so that the SMILES will be for the whole molecule.
        else {
            FOR_ATOMS_OF_MOL(a, *pmol)
            {
                fragatoms.SetBitOn(a->GetIdx());
            }
        }

        std::string buffer;
        buffer.reserve(1000);

        if (pmol->NumAtoms() > 0 || pmol->IsReaction()) {
            CreateCansmiString(*pmol, buffer, fragatoms, pConv);
        }
    }

}//namespace
