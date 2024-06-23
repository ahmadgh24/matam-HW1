#include "BlockChain.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
    cout << argv[2];
    ifstream source(argv[2]);
    ofstream target(argv[3]);

    BlockChain b;
    string s;
    // cout << "a" << endl;
    b = BlockChainLoad(source);
    BlockChainDump(b, target);
    // if (strcmp(argv[1], "format") == 0) {
    //     cout << "hi";

    //     BlockChain b;
    //     b = BlockChainLoad(source);
    //     // BlockChainDump(b, target);
    // }

    source.close();
    target.close();
    return 0;
}