/**********************************************************************
cpcomplex.h - Handle and define Cp (cyclopentadienyl) structures.

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

#ifndef OB_CPCOMPLEX_H
#define OB_CPCOMPLEX_H


#include <openbabel/babelconfig.h>

#include <openbabel/mol.h>
#include <openbabel/math/vector3.h>


namespace OpenBabel {

    #ifndef M_PI
    #define M_PI 3.14159265358979323846
    #endif

	class OBAPI CpComplex {
		protected:
			OBMol* _parent;                      //!< Parent molecule
			unsigned int _idx;                   //!< Cp identifier within the molecule
			unsigned int metal_idx;              //!< Atom idx of central metal
			std::vector<OBAtom*> _cpAtoms;            //!< Atoms for the carbons of the Cp structure
			std::vector<unsigned int> idx_carbons;    //!< Atom indexes for the carbons of the Cp structure (esto ya pierde un poco de sentido con _cpAtoms. De momento lo dejo)
			//vector<int> cpBonds;               //!< Bonds Cp indexes //Esto podria estar bien una vez sea capaz de separar los bonds si hay mas de 1 Cp
			vector3 orientation;                 //!< Cp orientation regarding the metal position for drawing
			vector3 center;                      //!< Cp center, for normal bond connection with metal atom, and aromatic circle position
			double radius;                       //!< Cp's aromatic circle radius
			unsigned int dummy_idx;              //!< Dummy central atom idx


    public:
        //Constructor
        CpComplex() {
            _parent = nullptr;
            _idx = 0;
            metal_idx = 0;
            _cpAtoms.clear();
            idx_carbons.clear();
            orientation.Set(0.0, 1.0, 0.0); //By default going upwards, above the metal
            center.Set(0.0, 0.0, 0.0);
            radius = 0.0;
            dummy_idx = 0;
        }

        unsigned int GetCarbonsSize() { return idx_carbons.size(); }
        OBAtom* CpComplex::BeginAtomCp(OBAtomIterator& i);
        OBAtom* CpComplex::NextAtomCp(OBAtomIterator& i);
        //OBAtom* CpComplex::GetAtom(int idx); //Esto de momento no funciona
        void SetParent(OBMol* ptr) { _parent = ptr; }
        void SetCentroid(vector3& _v) { center = _v; }
        void SetCentroid(const double v_x, const double v_y, const double v_z) { center.Set(v_x, v_y, v_z); }
        void SetRadius(double r) { radius = r; }
        void SetIdx(int idx) { _idx = idx; }
        void SetMetalIdx(int midx) { metal_idx = midx; }
        void SetDummyIdx(int idx) { dummy_idx = idx; }
        double GetDistanceDummyC(OBMol* pmol);
        OBMol* GetParent() { return((OBMol*)_parent); }
        unsigned int GetDummyIdx()   const { return((int)dummy_idx); }
        unsigned int GetIdx()   const { return((int)_idx); }
        unsigned int GetMetalIdx()   const { return((int)metal_idx); }
        unsigned int GetCarbonIdx(int i) const; //Dame el idx del carbono en la posicion i del vector. 0 based por tanto
        const std::vector<unsigned int>& GetIdxCarbons() { return idx_carbons; }
        void FindCentroid();
        vector3& GetCentroid() { return center; };
        double GetRadius() { return radius; }
        void AddIdxCarbon(int idx) { idx_carbons.push_back(idx); }
        void AddCpAtom(OBAtom* atom) { _cpAtoms.push_back(atom); }
        std::vector<unsigned int>::iterator CarbonBegin() { return idx_carbons.begin(); }
        std::vector<unsigned int>::iterator CarbonEnd() { return idx_carbons.end(); }

        //! Destructor
        //virtual ~CpComplex();
        //! Assignment
        //CpComplex& operator = (CpComplex&);
        //! Equivalence
        bool operator==(const CpComplex* other) const { return (GetIdx() == other->GetIdx()); }

        //! Clear all data.
        //! \return True if successful.
        bool Clear();

        //! \name Methods to modify internal information
        //@{


        //@}

        //! \name Methods to retrieve information
        //@{


        //@}

        //! \name Addition of data for a Cp
        //@{


        //@}



        //! \name Iteration methods
        //@{


        //@}



    }; // class CpComplex

}// namespace OpenBabel

#endif   // OB_CPCOMPLEX_H

//! \file cpcomplex.h
//! \brief Handle Cp structures























