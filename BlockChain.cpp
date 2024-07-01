#include "BlockChain.h"
#include "Transaction.h"
#include <iostream>
using namespace std;


int BlockChainGetSize(const BlockChain& blockChain) {
    int ctr = 0;
    const BlockChain* current = &blockChain;
    while (current != nullptr) {
        ctr++;
        current = current->next;
    }
    return ctr;
}


int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name) {
    int balance = 0; 
    const BlockChain* current = &blockChain;
    while (current != nullptr) {
        if (current->transaction.sender == name) {
            balance -= current->transaction.value;
        } else if (current->transaction.receiver == name) {
            balance += current->transaction.value;
        }
        current = current->next;
    }

    return balance;
}

void BlockChainAppendTransaction(
    BlockChain& blockChain,
    unsigned int value,
    const string& sender,
    const string& receiver,
    const string& timestamp
) {
    Transaction t;
    t.receiver = receiver;
    t.sender = sender;
    t.value = value;
    BlockChainAppendTransaction(blockChain, t, timestamp);
}

void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
) {
    BlockChain* temp = new BlockChain;
    temp->transaction = blockChain.transaction;
    temp->time = blockChain.time;
    temp->next = blockChain.next;
    blockChain.time = timestamp;
    blockChain.transaction = transaction;
    blockChain.next = temp;
}


BlockChain BlockChainLoad(ifstream& file) {  
    BlockChain* head = nullptr;
    BlockChain* temp = nullptr;

    std::string sender, receiver, timestamp;
    sender = receiver = timestamp = "";
    unsigned int value;
    while(file >> sender >> receiver >> value >> timestamp) {
        BlockChain *current = new BlockChain;
        current->time = timestamp;

        current->transaction = {0, sender, receiver};
        current->transaction.value = value;

        if (head == nullptr) {
            head = current;
            head->headLocation = head;
            temp = head;
        } else {
            temp->next = current;
            temp = temp->next;
        }
        sender = receiver = timestamp = "";

        
    }
    return *head;
}

void BlockChainDump(const BlockChain& blockChain, ofstream& file) {
    file << "BlockChain Info:" << std::endl;
    int i = 1;
    const BlockChain* current = &blockChain;
    while (current != nullptr) {
        file << i << "." << std::endl;
        TransactionDumpInfo(current->transaction, file);
        file << "Transaction timestamp: " << current->time << std::endl;
        i++;
        current = current->next;
    }
}

void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file) {
    const BlockChain* current = &blockChain;
    while (current != nullptr) {
        if (current->next != nullptr) {
            file << TransactionHashedMessage(current->transaction) << std::endl;
        } else {
            file << TransactionHashedMessage(current->transaction);
        }
        current = current->next;
    }
}

bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file) {
    string line;
    const BlockChain* current = &blockChain;
    while (current != nullptr) {
        getline(file, line);
        if (!TransactionVerifyHashedMessage(current->transaction, line)) {
            return false;
        }
        current = current->next;
    }
    if (file.eof() && current == nullptr) {
        return true;
    }
    return false;
}


void BlockChainCompress(BlockChain& blockChain) {
    BlockChain* temp = &blockChain;
    
    while (temp != nullptr && temp->next != nullptr) {
        BlockChain* next = temp->next;
        if (temp->transaction.sender == next->transaction.sender &&
            temp->transaction.receiver == next->transaction.receiver) {  
            temp->transaction.value += next->transaction.value;
            temp->next = next->next;
            delete next;
        } else {
            temp = next;
        }
    }
}

void BlockChainTransform(BlockChain& blockChain, updateFunction function) {
        BlockChain* current = &blockChain;
        while (current != nullptr) {
            current->transaction.value = function(current->transaction.value);
            current = current->next;
        }
}


void destroy(BlockChain* blockChain) {
    BlockChain* current = blockChain->next;
    while (current) {
        BlockChain* nextBlock = current->next;
        delete current;
        current = nextBlock;
    }
    delete blockChain->headLocation;
}