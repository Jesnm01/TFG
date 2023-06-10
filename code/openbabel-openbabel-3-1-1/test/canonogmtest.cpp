#include "obtest.h"
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/atom.h>
#include <openbabel/obiter.h>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace OpenBabel;

#ifdef TESTDATADIR
    string ogmtestdatadir = TESTDATADIR;
    string blocksmiles_file = ogmtestdatadir + "blocktest.smi";
#else
string blocksmiles_file = "blocktest.smi";
#endif

int BranchBlocksTest()
{
    cout << endl << "# Testing Block Identification in Canonical SMILES Generation ...  \n";

    std::ifstream mifs(blocksmiles_file.c_str());
    if (!mifs)
    {
        cout << "Bail out! Cannot read test data " << blocksmiles_file << endl;
        return -1; // test failed
    }

    OBConversion conv(&mifs, &cout);
    if (!conv.SetInAndOutFormats("SMI", "SMI"))
    {
        cout << "Bail out! SMILES format is not loaded" << endl;
        return -1;
    }

    OBMol mol;
    string buffer;
    unsigned int currentMol = 0;
    //read in molecules (as SMI), write as CANSMI, read in again
    while (getline(mifs, buffer))
    {
        mol.Clear();
        if (!conv.ReadString(&mol, buffer)) {
            cout << "not ok " << ++currentMol << " # SMILES read failed"
                << " buffer was " << buffer << "\n";
            continue;
        }
        if (mol.Empty())
            continue;

        buffer = conv.WriteString(&mol);

        //Ya ha parseado mol
        //Hacer cosas con mol. En este caso, mostrar sus branches
        mol.ShowBranches();
    }
    //const char* smiles = {
    //"[Cl-][Au+][P](C=1C=CC=CC1)(C=2C=CC=CC2)[C-]34[CH]5=[CH]6[CH]7=[CH]3[Fe+2]6789%10%1154[CH]=%12[CH]%11=[CH]%10[C-]9([CH]%128)[P]([Au+][Cl-])(C=%13C=CC=CC%13)C=%14C=CC=CC%14" };

    //OBConversion conv;
    //conv.SetInFormat("smi");
    //conv.SetOutFormat("smi");

    //OBMol mol;
    //conv.ReadString(&mol, smiles);

    //std::string buffer;

    //buffer = conv.WriteString(&mol);

    ////Ya ha parseado mol
    ////Hacer cosas con mol. En este caso, mostrar sus branches
    //mol.ShowBranches();

}


int canonogmtest(int argc, char* argv[]) {

    int defaultchoice = 1;

    int choice = defaultchoice;

    if (argc > 1) {
        if (sscanf(argv[1], "%d", &choice) != 1) {
            printf("Couldn't parse that input as a number\n");
            return -1;
        }
    }
    // Define location of file formats for testing
#ifdef FORMATDIR
    char env[BUFF_SIZE];
    snprintf(env, BUFF_SIZE, "BABEL_LIBDIR=%s", FORMATDIR);
    putenv(env);
#endif  
    int result = 0;
    switch (choice) {
    case 1:
        result = BranchBlocksTest();
        break;
        //case N:
        //  YOUR_TEST_HERE();
        //  Remember to update CMakeLists.txt with the number of your test
        //  break;
    default:
        cout << "Test number " << choice << " does not exist!\n";
        return -1;
    }

    return result;




    //OBMol mol;
    //OBConversion conv;
    //conv.SetInFormat("smi");
    //conv.SetOutFormat("smi");

    //std::string line;
    //OBMol mol;
    //OBConversion conv;
    //conv.SetInFormat("smi");
    //conv.ReadString(&mol, "CC[2H]");

    //OBForceField* pFF = ::FindForceField("UFF");
    //OB_REQUIRE(pFF);

    //OB_ASSERT(pFF->Setup(mol));
    ////...

    //// Test writing U smiles after I smiles
    //OBConversion conv;
    //conv.SetInFormat("smi");
    //OBMol mol;
    //conv.ReadString(&mol, "C(=O)([O-])C(=O)O");
    //conv.SetOutFormat("smi");
    //conv.SetOptions("I", OBConversion::OUTOPTIONS);
    //std::string res = conv.WriteString(&mol, true);
    //OB_COMPARE(res, "C(=O)(C(=O)O)[O-]");
    //conv.SetOptions("U", OBConversion::OUTOPTIONS);
    //res = conv.WriteString(&mol, true);
    //OB_COMPARE(res, "C(=O)(C(=O)[O-])O");
    ////...


    
}

