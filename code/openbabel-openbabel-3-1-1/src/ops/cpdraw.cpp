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

using namespace std;

namespace OpenBabel
{
    //Quizas deberia definir el struct este en mol.h, porque al final el que tiene la variable de esto es la molecula, y asi no me complico tanto con los include (creo, porque todo el mundo incluye mol.h en algun momento)
    //De hecho, deberia hacerlo una clase aparte, porque como siga metiendo metodos y variables... Ya no se si en fichero aparte (para no joder el cmakelist) o meterlo en alguno
    struct CpComplex {
        unsigned int idx;           //Cp identifier within the molecule
        unsigned int metal_idx;     //Atom idx of central metal
        vector<int> idx_carbons;    //Atom indexes for the carbons of the Cp structure
        //vector<int> cpBonds;      //Bonds Cp indexes //Esto no deberia ir aqui creo. Si hay mas de 1 cp, esta informacion no le hace falta a cada cp por separado, porque tiene todos los bonds tipo Cp
        //deberia meter info sobre la orientacion del cp luego para dibujarlo de una manera o de otra
        vector3 orientation;        //Cp orientation regarding the metal position for drawing


        //Constructor
        CpComplex() {
            idx = 0;
            metal_idx = 0;
            idx_carbons.clear();
            orientation.Set(0.0, 1.0, 0.0); //By default going upwards, above the metal
        }

        unsigned int GetCarbonsSize() { return idx_carbons.size(); }
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
            - que el carbono tenga seguido otros carbonos iguales (si tiene una rama no será igual, pero eso lo veré luego), y que esos carbonos estén enlazados tambien con el mismo metal
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
            //test[b->GetIdx()] = isFerroceneBond(&*b);
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
        if (pmol->GetAtom(idxMetal)->GetExplicitDegree() < cpBondSum) {
            obErrorLog.ThrowError(__FUNCTION__, "Failed to detect Cp structure, metal has not enough bonds", obWarning);
            return false;
        }

        cout << "\n";

        //Si trabajamos con 1 cp solamente (de momento), lo que tengo que hacer es crearme un struct CpComplex metiendo el idx_metal, y cada uno de los idx end() de los bonds que sean ==1
        CpComplex* cp = new CpComplex;
        cp->metal_idx = idxMetal;
        //int k = 0;
        int idxInsert;
        for (int i = 0; i < cpBonds.size(); i++) {
            if (cpBonds[i] == 1) {
                idxInsert = pmol->GetBond(i)->GetEndAtom()->GetIdx();
                cp->idx_carbons.push_back(idxInsert);
            }
        }
        pmol->AddCpComplex(cp);




        //ANTES DE HACER EL RETURN, TENGO QUE HABER MODIFICADO TODO LO QUE NECESITE. LA MOLECULA TIENE QUE ESTAR YA CAMBIADA (ME REFIERO A LAS COORDENADAS DE LOS CARBONOS DEL CP. 
        //O mejor, como voy a hacer el dibujin ese del pan con el ciruclo y tal, deberia de alguna manera meterle una variable a la molecula, para luego en el metodo de dibujado 
        //(que tendré que modificar) ignorar los carbonos cuyos idx estén en el CpComplex, y en su lugar poner el dibujin)
        return true;
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
        - inventar otra manera de detectar enlaces con estructuras Cp. Esto en realidad con lo que tengo aquí ya me sirve, pero mirar ademas
            - ^^deberia recorrer los bonds, ver si hay bonds seguidos (o si no fueran seguidos, por si hubiera una rama en algun punto del Cp, 
            eso lo dejamos para mas adelante) de tipo [cH][cH]..., o [CH][CH]... o [CH]=[CH]=[CH][CH-][CH]; y marcar eso de alguna forma. 
            Se me ocurre, sustituir los bonds que hayan sido marcados como parte de metallocenos y poner en su lugar un unico bond especial con algun tipo de flag 
            o lo que sea que indique que es un Cp.
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
