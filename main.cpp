#include "BlockChain.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Usage: ./mtm_blockchain <op> <source> <target>" << endl;
        return 0;
    }
    ifstream source(argv[2]);
    BlockChain b;
    b = BlockChainLoad(source);

    if (strcmp(argv[1], "format") == 0) {
        ofstream target(argv[3]);
        BlockChainDump(b, target);
        target.close();
    } else if (strcmp(argv[1], "hash") == 0) {
        ofstream target(argv[3]);
        BlockChainDumpHashed(b, target);
        target.close();
    } else if (strcmp(argv[1], "compress") == 0) {
        ofstream target(argv[3]);
        BlockChainCompress(b);
        BlockChainDump(b, target);
        target.close();
    } else if (strcmp(argv[1], "verify") == 0) {
        ifstream target(argv[3]);
        bool verify = BlockChainVerifyFile(b, target);
        cout << "Verification " << (verify ? "passed" : "failed") << endl;
        target.close();
    }

    source.close();
    return 0;
}
