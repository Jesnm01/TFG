#include "obtest.h"
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/atom.h>
#include <openbabel/obiter.h>


#include <iostream>
#include <vector>
#include <algorithm>
#include "../src/formats/smilesformat.cpp"

using namespace std;
using namespace OpenBabel;

#ifdef TESTDATADIR
    string ogmtestdatadir = TESTDATADIR;
    //Block test
    string blocksmiles_file = ogmtestdatadir + "blocktest.smi";
    //Random test
    string complete_dataset_file = "C:/TFG/dataset/complete_dataset.smi";
    //string complete_dataset_file = "C:/TFG/dataset/debug_dataset.smi";
    string complete_canon_file = "C:/TFG/dataset/complete_canon_dobleCp.smi";
    string complete_random_file = ogmtestdatadir + "randomCanontest.smi";
    string complete_random_canon_file = "C:/TFG/dataset/complete_random_canon.smi";
    
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
    if (!conv.SetInAndOutFormats("smi", "can"))
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
        //cout << buffer;
        buffer = "-:" + buffer;
        //buffer.insert(buffer.begin(), '-:');
        //cout << buffer;
        vector<string> FileList, OutputFileList;
        string OutputFileName;
        FileList.push_back(buffer);
        OutputFileName = "C:\\TFG\\dataset\\output\\test\\datatests_mycanon_v3_doubleCpBlocks.can";

        int count = conv.FullConvert(FileList, OutputFileName, OutputFileList);

        OB_REQUIRE((count > 0));
    }

    /*OBConversion conv;
    conv.SetInFormat("smi");
    conv.SetOutFormat("smi");*/

    
    return 0;
}

int DrawDoubleCpTest() {
    cout << endl << "# Testing Detection and Drawing Double Cp in SVG Depiction...  \n";

    OBConversion conv;
    conv.SetInFormat("smi");
    conv.SetOutFormat("svg");
    OBMol mol;    

    vector<string> FileList, OutputFileList;
    string OutputFileName;
    FileList.push_back("-:[Cl-][Au+][P](C=1C=CC=CC1)(C=2C=CC=CC2)[C-]34[CH]5=[CH]6[CH]7=[CH]3[Fe+2]6789%10%1154[CH]=%12[CH]%11=[CH]%10[C-]9([CH]%128)[P]([Au+][Cl-])(C=%13C=CC=CC%13)C=%14C=CC=CC%14");
    OutputFileName = "C:\\TFG\\dataset\\output\\test\\mol23_dobleCpDebug.svg";

    int count = conv.FullConvert(FileList, OutputFileName, OutputFileList);

    OB_REQUIRE((count>0));
    return 0;
}

int SelectCanonMetal() {
    cout << endl << "# Testing Metal Selection in Canonical Algorithm...  \n";

    OBConversion conv;
    conv.SetInFormat("smi");
    conv.SetOutFormat("smi");
    OBMol mol;

    vector<string> FileList, OutputFileList;
    string OutputFileName;
    FileList.push_back("-:[Cl-][Au+][Fe+2]([Au+][Cl-])[Pd+2]([Br-])[Br-]");
    OutputFileName = "C:\\TFG\\dataset\\output\\test\\molrandom_selectMetal.smi";

    int count = conv.FullConvert(FileList, OutputFileName, OutputFileList);

    cout << "CanSmiles: " << mol.GetCanSmiles();

    OB_REQUIRE((count > 0));
    return 0;
}

int RandomCanon() {
    cout << endl << "# Testing Canon Persistance when using random labels...\n";


    OBMol mol, mol2;

    OBConversion convAntiC;
    //convCanon.SetOutStream(&cout);
    convAntiC.SetInAndOutFormats("smi", "smi");
    convAntiC.AddOption("C"); //Anticanonical smiles

    /*if (!convAntiC.SetInAndOutFormats("smi", "smi"))
    {
        cout << "Bail out! SMILES format is not loaded" << endl;
        return -1;
    }*/
    

    
    //Convertimos los SMILES originales a random SMILES usando la opcion -C anticanonical
    vector<string> FileList, OutputFileList;
    string OutputFileName;
    FileList.push_back(complete_dataset_file);
    OutputFileName = complete_random_file;
    int count = convAntiC.FullConvert(FileList, OutputFileName, OutputFileList);
    OB_REQUIRE((count > 0));

    //Convertimos los random SMILES en canonicos
    OBConversion convCanon;
    convCanon.SetInAndOutFormats("smi", "smi");
    unsigned int currentTest = 0;
    vector<string> FileList2, OutputFileList2;
    string OutputFileName2;
    FileList2.push_back(complete_random_file);
    OutputFileName2 = complete_random_canon_file;

    count = convCanon.FullConvert(FileList2, OutputFileName2, OutputFileList2);
    OB_REQUIRE((count > 0));

    std::ifstream mifs, mifs2, mifs3, mifs4;
    if (!SafeOpen(mifs, complete_random_canon_file.c_str()))
    {
        cout << "Bail out! Cannot read file " << complete_random_canon_file << endl;
        return -1; // test failed
    }
    if (!SafeOpen(mifs2, complete_canon_file.c_str()))
    {
        cout << "Bail out! Cannot read file " << complete_canon_file << endl;
        return -1; // test failed
    }
    if (!SafeOpen(mifs3, complete_random_file.c_str()))
    {
        cout << "Bail out! Cannot read file " << complete_random_file << endl;
        return -1; // test failed
    }
    if (!SafeOpen(mifs4, complete_dataset_file.c_str()))
    {
        cout << "Bail out! Cannot read file " << complete_dataset_file << endl;
        return -1; // test failed
    }


    //Leemos ambos ficheros y comparamos los resultados
    std::string canon, random_canon, random, original;
    while (mifs.good() && mifs2.good() && mifs3.good())
    {
        if (std::getline(mifs, random_canon) && std::getline(mifs2, canon) && std::getline(mifs3, random) && std::getline(mifs4, original)) {
            cout << "Original SMILES for \t\t" << ++currentTest << ": " << original << "\n";
            cout << "Random SMILES for \t\t" << currentTest << ": " << random << "\n";
            cout << "Canon from Random SMILES for \t" << currentTest << ": " << random_canon << "\n";
            cout << "Canon original SMILES for \t" << currentTest << ": " << canon << "\n\n";
            OB_COMPARE(random_canon, canon);
        }
        
    }

    mifs.close();
    mifs.clear();
    mifs2.close();
    mifs2.clear();

    return 0;
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
    case 2:
        result = DrawDoubleCpTest();
        break;
    case 3:
        result = SelectCanonMetal();
        break;
    case 4:
        result = RandomCanon();
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
}

