#include "BlockChain.h"
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4) {
         cout << getErrorMessage() << endl;
        return 0;
    }
    ifstream source(argv[2]);
    if(source.peek() == ifstream::traits_type::eof()) {
        ofstream target(argv[3]);
        target.close();
        return 0;
    }
    BlockChain b;
    b = BlockChainLoad(source);
    if (strcmp(argv[1], "verify") == 0) {
        ifstream targetI(argv[3]);
        bool verify = BlockChainVerifyFile(b, targetI);
        cout << "Verification " << (verify ? "passed" : "failed") << endl;
    } else {
        ofstream target(argv[3]);
        if (strcmp(argv[1], "format") == 0) {
            BlockChainDump(b, target);
        } else if (strcmp(argv[1], "hash") == 0) {
            BlockChainDumpHashed(b, target);
        } else if (strcmp(argv[1], "compress") == 0) {
            BlockChainCompress(b);
            BlockChainDump(b, target);
        }
    }
    destroy(&b);
    return 0;
}
