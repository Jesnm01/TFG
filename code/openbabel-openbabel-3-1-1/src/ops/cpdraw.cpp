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
//#include <openbabel/mcdlutil.h>
//#include <openbabel/stereo/stereo.h>
#include <openbabel/obiter.h>
#include <openbabel/elements.h>
#include <openbabel/oberror.h>
#include "../formats/smilesformat.cpp"
#include <openbabel/mcdlutil.h>

using namespace std;

namespace OpenBabel
{
    //Quizas deberia definir el struct este en mol.h, porque al final el que tiene la variable de esto es la molecula, y asi no me complico tanto con los include (creo, porque todo el mundo incluye mol.h en algun momento)
    //De hecho, deberia hacerlo una clase aparte, porque como siga metiendo metodos y variables... Ya no se si en fichero aparte (para no joder el cmakelist) o meterlo en alguno
    struct CpComplex {
        unsigned int idx;           //Cp identifier within the molecule
        unsigned int metal_idx;     //Atom idx of central metal
        vector<OBAtom*> _cpAtoms;    //Atoms for the carbons of the Cp structure
        vector<int> idx_carbons;    //Atom indexes for the carbons of the Cp structure (esto ya pierde un poco de sentido con _cpAtoms. De momento lo dejo)
        //vector<int> cpBonds;      //Bonds Cp indexes //Esto no deberia ir aqui creo. Si hay mas de 1 cp, esta informacion no le hace falta a cada cp por separado, porque tiene todos los bonds tipo Cp
        vector3 orientation;        //Cp orientation regarding the metal position for drawing
        //Meter vector de path o coordenadas o algo de eso, para que pueda dibujarlo


        //Constructor
        CpComplex() {
            idx = 0;
            metal_idx = 0;
            _cpAtoms.clear();
            idx_carbons.clear();
            orientation.Set(0.0, 1.0, 0.0); //By default going upwards, above the metal
        }

        unsigned int GetCarbonsSize() { return idx_carbons.size(); }
        OBAtom* CpComplex::BeginAtomCp(OBAtomIterator& i);
        OBAtom* CpComplex::NextAtomCp(OBAtomIterator& i);
        void GetCentroid(vector3& _v);
    };

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
        bool isFerroceneBond(OBBond* bond);
        bool isCpBond(OBBond* bond, unsigned int idxM); //Metodo que comprueba si a priori podría ser un enlace tipo Cp
        //void CalcBBox();

    //private:
        //vector<CpComplex*> _cps; //Vector de punteros a strcut, por si hubiera mas de 1 cp
    };

    /////////////////////////////////////////////////////////////////
    OpCpDraw theOpCpDraw("CpDraw"); //Global instance

    /////////////////////////////////////////////////////////////////
    bool OpCpDraw::Do(OBBase* pOb, const char* OptionText, OpMap* pOptions, OBConversion* pConv)
    {
        OBMol* pmol = dynamic_cast<OBMol*>(pOb);
        if (!pmol)
            return false;


        //...
        //Algoritmo de deteccion de Cp
        /*HACER: quedarse con el metal que tiene enlace con el 1º carbono. Comprobar que los demas carbonos de seguido, tb tienen enlace con ese metal en concreto. 
        Ver cuantos carbonos hay de ese estilo, y quedarse con la cantidad (esto determinará el tipo de ciclo Cp (de 5, de 6, de 4, lo que sea...
        para luego hacer el dibujo predeterminado con la misma cantidad de vertices))*/
        /* Para saber si es Cp-like:
            - que el carbono esté en un anillo
            - que esos carbonos estén enlazados tambien con el mismo metal
            - una vez detectado el nº de carbonos del anillo, comprobar que el metal tiene al menos ese numero de enlaces*/
        vector<int> test(pmol->NumBonds(),0);
        vector<int> cpBonds(pmol->NumBonds(), 0);
        OBAtom* begin;
        OBAtom* end;
        int i = 0;
        int atomNMetal = -1; 
        unsigned int idxMetal = 0;
        bool cpTest = false;
        FOR_BONDS_OF_MOL(b,pmol) {
            begin = b->GetBeginAtom();
            end = b->GetEndAtom();
            if (begin->IsOgmMetal(atomNMetal)) { //Establezco que el metal debe ser siempre el atomo begin de los bonds (he tomado esa decision)
                //Comprobamos que podría ser un Cp bond de primeras
                cpTest = isCpBond(&*b,idxMetal);
                if (cpTest) { //Una vez encuentro el primer bond M-C type, guardo el idx del metal, para hacer comprobaciones en los siguientes bonds
                    if(!idxMetal) idxMetal = begin->GetIdx();
                    cpBonds[b->GetIdx()] = cpTest;
                }
            }

            //Debug couts
            cout << "isOgmMetal: " << atomNMetal;
            cout << "[" << begin->GetIdx() << "]" << OBElements::GetSymbol(begin->GetAtomicNum()) << "-" << "[" << end->GetIdx() << "]" << OBElements::GetSymbol(end->GetAtomicNum())
                << ": " << cpBonds[i] << "\n";
            i++;
            
        }

        //DE MOMENTO TRABAJO COMO SI SOLAMENTE HUBIERA 1 CP, POR LO QUE TODOS LOS CPBONDS LOS TRATARÉ COMO SI FUERAN DEL MISMO CP-COMPLEX
        
        //TO DO en el futuro: detectar varias estructuras Cp
        // //NECESITO SABER CUANDO ACABA Y CUANDO EMPIEZA CADA CP, EN CASO DE HABER VARIOS
        // //¿Necesito hacerme una clase Cp para almacenar todo esto?
        // Se me ocurre ver si el carbono tiene implicit hydrogen o no (los Cp like tienen 1 hidrogeno explicito. En el codigo se le llama implicit durante el parseSmiles, pero en realidad el valor que tenga el atomo en la variable _imph)
        // MEJOR: parsear de nuevo rapidamente el string, y ver con un par de reglas cuando acaba y cando empieza el Cp:
        //      - Normalmente, el 1º carbono tiene un numero de ciclo. Ese numero lo guardo, y cuando lo vea de nuevo, es cierre de ciclo Cp
        //      - Como tb tengo el vector de bonds Cp like que acabo de sacar aqui, puedo ir directemente a los bonds que estén marcados (claro, porque puede haber ciclos de carbonos normales que no sean Cp)
        //      - Empiezo en el 1º atomo de los bonds que me marque como Cp, y conforme vaya parseando y encontrandome mas carbonos me voy quedando con sus idx, y los meto en un vector (cuando acabe, sabré tb con el size del vector la cantidad de carbonos del Cp). (me puedo hacer un struct con esa info si es necesario)
        //      - Si encuentro una rama dentro del Cp, la ignoro, y no sumo esos carbonos que puedieran haber dentro de la rama para el sum de carbonos totales del Cp.
        //...

        //Parseamos la cadena de nuevo, para averiguar donde empiezan y donde acaban cada uno de los Cp-complex
        /*string smiles = pmol->GetSmiles();
        OBSmilesParser sp(pConv->IsOption("a", OBConversion::INOPTIONS));
        bool result = sp.CpDetect(*pmol, smiles, cpBonds, _cps);*/


        


        //Esto cambiarlo por el conteo de .size() del vector que me cree con los indices de los Cp, una vez pueda parsear el SMILES de nuevo
        int cpBondSum = 0;
        for (int i = 0; i < cpBonds.size(); i++) {
            cpBondSum += cpBonds[i];
        }

        //Comprobar que el metal tiene al menos el tantos bonds como carbonos detectamos en los ciclos
        //Esto es para que si despues del bucle, tenemos bonds que tienen pinta de Cp, pero luego vemos que el metal en cuestion no tiene enlaces suficientes, algo ha ido mal, o realmente no eran Cp bonds
        if (idxMetal) {
            if (pmol->GetAtom(idxMetal)->GetExplicitDegree() < cpBondSum) {
                obErrorLog.ThrowError(__FUNCTION__, "Failed to detect Cp structure, metal has not enough bonds", obWarning);
                return false;
            }

            //Si trabajamos con 1 cp solamente (de momento), lo que tengo que hacer es crearme un struct CpComplex metiendo el idx_metal, y cada uno de los idx end() de los bonds que sean ==1
            CpComplex* cp = new CpComplex;
            cp->metal_idx = idxMetal;
            int idxInsert;
            for (int i = 0; i < cpBonds.size(); i++) {
                if (cpBonds[i] == 1) {
                    idxInsert = pmol->GetBond(i)->GetEndAtom()->GetIdx();
                    cp->idx_carbons.push_back(idxInsert);
                    cp->_cpAtoms.push_back(pmol->GetAtom(idxInsert));
                }
            }
            pmol->AddCpComplex(cp);
        }
        else { //Si idxMetal sigue siendo 0, es que no hay ningun Cp en la molecula, por lo que lo dejamos así, como si no se hubiera hecho nada

        }
        

        cout << "\n";

        

        //Debug: ver las coordenadas que ha generado gen2D
        OBAtom* atom;
        OBAtomIterator it;
        //for (atom = pmol->BeginAtom(i); atom; atom = pmol->NextAtom(i))
        //{
        //    //atom->SetVector(atom->GetX() * f, -atom->GetY() * f, atom->GetZ());
        //    cout << "[idx= " << atom->GetIdx() << "][atomic_number: " << atom->GetAtomicNum() << "] x: " << atom->GetX() << "; y: " << atom->GetY() << "\n";
        //}

        cout << "Coordenadas dentro de cpdraw Do antes de tocar nada: \n";
        for (int i = 1; i <= pmol->NumAtoms(); i++) {
            atom = (pmol->GetAtom(i));
            cout << "[idx= " << atom->GetIdx() << "][atomic_number: " << atom->GetAtomicNum() << "] x: " << atom->GetX() << "; y: " << atom->GetY() << "\n";
        }
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
        double offsetX = 0.0, offsetY = 0.0;
        OBAtom* atomMetal;
        atomMetal = pmol->GetAtom(idxMetal);

        //vector3 begin = beginAtom->GetVector();
        //vector3 end = endAtom->GetVector();
        //vector3 vb = end - begin;

        //vb.normalize();

        // find min/max values de los carbonos del Cp
        CpComplex* cp;
        cp = pmol->GetCpComplex(1); //de momento solo trabajamos con 1 cp

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

        Poner las coordenadas con respecto al metal
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

        cout << "Modificamos las coordenadas de los carbonos Cp:\n";
        for (atom = cp->BeginAtomCp(it); atom; atom = cp->NextAtomCp(it)) {
            atom->SetVector(atom->GetX(), atom->GetY() + offsetY + spacing, atom->GetZ());
            cout << "[idx= " << atom->GetIdx() << "][atomic_number: " << atom->GetAtomicNum() << "] x: " << atom->GetX() << "; y: " << atom->GetY() << "\n";
        }

        // create new dummy atom entre el metal y el centro del Cp (esto se calcula haciendo la media entre las coordenadas de todos los carbonos, para X e Y)
        vector3 centroidCp;
        cp->GetCentroid(centroidCp);
        cout << "Centroid cp: " << centroidCp << "\n";

        OBAtom* dummy;
        dummy = pmol->NewAtom();
        dummy->SetAtomicNum(0);
        dummy->SetVector(centroidCp);
        //Aqui no se si tendria que hacer un endmodify o algo asi, para que las coordenadas de _v se pasen a _c o viceversa o algo de eso

        // bond dummy atom to center metal
        pmol->AddBond(idxMetal, dummy->GetIdx(), 1);
        //Esto va bien, se crea el bond dummy y el dibujado pinta la linea al centro del Cp como queria. Pero como es dummy, elem==0, pinta un asterisco como si fuera un radical o algo asi.
        // Esto hace que quiera pintarle un symbol y que deje un espacio para el *. Hace que la linea no quede justo donde he calculado 
        //Esto supongo que lo puedo parchear configurando una variable en atom.h que marque si es dummy_cp o algo asi, y luego en el depict.cpp linea ~665, comprobar esa condicion
        //UNA VEZ DIBUJADO EL SVG, DEBERIA ELIMINAR ESTE ATOM Y EL BOND CREADO, POR SI LUEGO SE HACE ALGUNA QUE OTRA OPERACION CON LA MOLECULA NO JODER NADA. (esto en vd no creo que haga falta, si solamente le pido el dibujado, cuando acaba, se destruye todo)

        cout << "Dummy bond created \n";

        /*Varias maneras para dibujar el pan: 
            1.- puedo mover los carbonos, crear el bond con el centroide, y eliminar los bonds de los carbonos-M para quitar las lineas. 
            Eliminar los bonds puede ser peligroso si luego se van a realizar mas operaciones con la molecula, pero si solamente se va a dibujar, hace el apaño 
            (lo engorroso será si alguno de los carbonos del Cp, tiene alguna rama. Aunuqe ahora que lo pienso ese bond sigue estando, solamnete estoy quitando el bond C-M)
            Seria mucho mejor hacer algo para que no pinte estos bonds, que los ignorase en el dibujado (y mantener la estructura de enlaces), pero de momento no se como hacer eso.
            2.- puedo crear el bond, y alrededor del atomo dummy, crear las coordenadas de los carbonos
         */

        //Quitar los bonds C-M
        OBAtom* beginDel = pmol->GetAtom(idxMetal);
        OBAtom* endDel;
        begin = pmol->GetAtom(idxMetal);
        OBBond* bondToDelete;

        for (int i = 0; i < cp->idx_carbons.size(); i++) {
            end = pmol->GetAtom(cp->idx_carbons[i]);
            bondToDelete = pmol->GetBond(begin, end);
            pmol->DeleteBond(bondToDelete);
        }


        cout << "Cp Bonds deleted. New molecule bond list: \n";
        FOR_BONDS_OF_MOL(b, pmol) {
            begin = b->GetBeginAtom();
            end = b->GetEndAtom();
            cout << "[" << begin->GetIdx() << "]" << OBElements::GetSymbol(begin->GetAtomicNum()) << "-" 
                 << "[" << end->GetIdx() << "]" << OBElements::GetSymbol(end->GetAtomicNum()) << "\n";
        }





        /*ANTES DE HACER EL RETURN, TENGO QUE HABER MODIFICADO TODO LO QUE NECESITE. LA MOLECULA TIENE QUE ESTAR YA CAMBIADA (ME REFIERO A LAS COORDENADAS DE LOS CARBONOS DEL CP. 
        O mejor, como voy a hacer el dibujin ese del pan con el ciruclo y tal, deberia de alguna manera meterle una variable a la molecula, para luego en el metodo de dibujado 
        (que tendré que modificar) ignorar los carbonos cuyos idx estén en el CpComplex, y en su lugar poner el dibujin)
        Algo tipo:
          -Si !Empty() el vector de _cps de la molecula, los carbonos que tengan el idx dentro, le hago algo en el metodo de dibujado para que no introduzca su codigo xml, 
         y en su lugar meter un codigo xml que dibuje un panecillo con las coordenadas que vea oportunas (por hacer)

        Si quiero aprovechar lo que hay hecho del sistema de dibujado, lo que tengo que hacer es cambiar las coordenadas de los carbonos del Cp. Esto lo que pasa es que me pintará las lineas sueltas y quedará algo tipo SciFinder
        Si quiero pintar el pan como tal, tengo que hacer mas cosas:
              - O bien: Crear 1 enlace entre el metal y un atomo dummy que estuviera en el centro del Cp segun la orientacion que le demos, y pintar ese bond con el sistema que ya hay 
         Meido meh esto, porque luego el pan lo tengo que poner las coords a mano, e ignorar los carbonos que ya hay en la molecula
              - O bien: lo pinto todo a mano.
              - Puedo marcar los atomos con un flag o algo que indiquen que son parte del Cp. Y luego cuando dibuje los bonds, si ve que son Cp.. (esto no se muy bien como seguirlo, no le veo futuro)
         Ambas opciones para el pan creo que van a quedar fatal, porque me salto los escalados y demas operaciones que hace*/
        return true;
    }

    OBAtom* CpComplex::BeginAtomCp(OBAtomIterator& i)
    {
        i = _cpAtoms.begin();
        return i == _cpAtoms.end() ? nullptr : (OBAtom*)*i;
    }

    OBAtom* CpComplex::NextAtomCp(OBAtomIterator& i)
    {
        ++i;
        return i == _cpAtoms.end() ? nullptr : (OBAtom*)*i;
    }

    void CpComplex::GetCentroid(vector3& _v)
    {
        double sumX = 0.0, sumY = 0.0;
        for (int i = 0; i < _cpAtoms.size(); i++) {
            sumX += _cpAtoms[i]->GetX();
            sumY += _cpAtoms[i]->GetY();
        }
        sumX = sumX / _cpAtoms.size();
        sumY = sumY / _cpAtoms.size();

        _v.Set(sumX, sumY, 0.0);
    }


    bool OpCpDraw::isCpBond(OBBond* bond, unsigned int idxM)
    {
        if (bond->GetBondOrder() != 1)
            return false;

        OBAtom* M = nullptr, * C = nullptr; //M for metal and C for carbon

        OBAtom* begin = bond->GetBeginAtom();
        bool idxGood = true;
        if (idxM)
            (begin->GetIdx() == idxM) ? idxGood = true : idxGood = false;
        if (begin->IsOgmMetal() && idxGood) //Si es un Ogm metal y el idx de ese metal es el mismo que los anteriores bonds
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


    

    /*Este metodo me detecta solamente 8 enlaces de ferroceno cuando deveria pillar 10 (1 por cada enlace Fe - C de ambos Cp).
    La cosa es que comprueba que el Carbono tenga 1 doble enlace al menos.En los Cp, al kekulizar, hay un carbono que se queda sin doble enlace
    (en teoria los Cp serían aromáticos pero esto no los trata como tal al kekulizar) por lo que ese es el que no detecta en el metodo*/

    /*TO DO:
        Se me ocurre tb, poner el Do mio antes del Do de gen2D, y modificar ese para que si detecta mi flag especial, no aplique su algoritmo o algo asi (esto es fumada en vd)*/
    bool OpCpDraw::isFerroceneBond(OBBond* bond)
    {
        if (bond->GetBondOrder() != 1)
            return false;

        OBAtom* Fe = nullptr, * C = nullptr;

        OBAtom* begin = bond->GetBeginAtom();
        if (begin->GetAtomicNum() == 26)
            Fe = begin;
        if (begin->GetAtomicNum() == 6)
            C = begin;

        OBAtom* end = bond->GetEndAtom();
        if (end->GetAtomicNum() == 26)
            Fe = end;
        if (end->GetAtomicNum() == 6)
            C = end;

        if (!Fe || !C)
            return false;

        if (Fe->GetExplicitDegree() < 10)
            return false;

        return C->HasDoubleBond() && C->IsInRing();
    }
}//namespace
