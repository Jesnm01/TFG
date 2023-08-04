Open Babel
----------

[![GitHub release](https://img.shields.io/github/release/openbabel/openbabel.svg?maxAge=86400)](https://github.com/openbabel/openbabel/releases)
[![Download Open Babel](https://img.shields.io/sourceforge/dt/openbabel.svg?maxAge=86400)](https://github.com/openbabel/openbabel/releases)
[![Travis CI](https://img.shields.io/travis/openbabel/openbabel.svg)](https://travis-ci.org/openbabel/openbabel)
[![Google Scholar Citations](https://openbabel.org/citations.svg?maxAge=86400)](https://scholar.google.com/scholar?oi=bibs&hl=en&cites=13319995025871922899&as_sdt=5)

The repository with the current code (forked from the official openbabel repo) with the changes I mention below can be found in [this other repository](https://github.com/Jesnm01/openbabel/tree/organometallic_additions)

This is a clone of the official Openbabel repository for the 3-1-1 release, which I've been working with (later changes to the master branch do not exist here, this is the code extracted from the 3-1-1 release zip file).

These are the classes to which I added new variables or methods for the porpuse of my end-of-degree project:

- Added new features to these classes:
    - _OBMol_
    - _OBAtom_
    - _OBMol2Cansmi_
    - _OBCanSmiNode_
    - _SVGPainter_ (consecuently to _OBPainter_, _ASCIIPainter_ and _CommandPainter_)
- Added these new classes:
    - _CpComplex_: handle and define Cp (cyclopentadienyl) structures
    - _BranchBlock_: detect branches during the canonicalization process, mainly for Cp depiction purposes
    - _OpCpDraw_: plugin operation for identification and Cp storing inside the molecule for its later depiction.
    - _SubTreeSizes_: helping struct for new organometallic canonicalization method.
    - _subtreecomp_ and _mycomp_: custom comparator objects for the new organometallic canonicalization method.
- Modified some methods such as _CreateCansmiString_ or _OBDepict::DrawMolecule_ (among others) to fit in the new code. Modifications have comments above starting with '//New:' explaining them
