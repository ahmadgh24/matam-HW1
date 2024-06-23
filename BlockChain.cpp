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
    BlockChain* newBlock = new BlockChain;
    newBlock->transaction.value = value;
    newBlock->transaction.sender = sender;
    newBlock->transaction.receiver = receiver;
    newBlock->time = timestamp;
    newBlock->next = &blockChain;
    blockChain = *newBlock;
}

void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
) {
    BlockChain* newBlock = new BlockChain;
    newBlock->transaction = transaction;
    newBlock->time = timestamp;
    newBlock->next = &blockChain;
    blockChain = *newBlock;
}


BlockChain BlockChainLoad(ifstream& file) {  
    BlockChain *head;
    BlockChain* temp = nullptr;

    std::string line, sender, receiver, valueStr, timestamp;
    sender = receiver = valueStr = timestamp = "";
    int value;
    while(getline(file, line)) {
        
        Transaction transaction;
        BlockChain current;
        int i = 0;
        while(line[i] != ' ') {
            sender += line[i];
            i++;
        }
        i++;
        while(line[i] != ' ') {
            receiver += line[i];
            i++;
        }
        i++;
        while(line[i] != ' ') {
            valueStr += line[i];
            i++;
        }
        i++;
        while(i < line.length()) {
            timestamp += line[i];
            i++;
        }

        value = std::stoi(valueStr);
        transaction.sender = sender;
        transaction.receiver = receiver;
        transaction.value = value;
        current.time = timestamp;
        current.transaction = transaction;

        if (head == nullptr) {
            head = &current;
            temp = head;
        } else {
            temp->next = &current;
            temp = temp->next;
        }
        sender = receiver = valueStr = timestamp = "";
    }

    return *head;
}

void BlockChainDump(const BlockChain& blockChain, ofstream& file) {
    file << "BlockChain info:" << std::endl;
    int i = 1;
    const BlockChain* current = &blockChain;
    while (current != nullptr) {
        file << i << "." << std::endl;
        TransactionDumpInfo(current->transaction, file);
        file << "Transaction Timestamp: " << current->time;
        i++;
        current = current->next;
    }
}

void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file) {
    const BlockChain* current = &blockChain;
    while (current != nullptr) {
        if (current->next != nullptr) {
            file << TransactionHashedMessage(blockChain.transaction) << std::endl;
        } else {
            file << TransactionHashedMessage(blockChain.transaction);
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
    return true;
}


void BlockChainCompress(BlockChain& blockChain) {
    BlockChain* currentCompress = &blockChain;
    BlockChain* current = &blockChain;
    while(currentCompress != nullptr) {
        while (current->next != nullptr 
        && current->next->transaction.sender == currentCompress->transaction.sender 
        && current->next->transaction.receiver == currentCompress->transaction.receiver) {
            currentCompress->transaction.value += current->next->transaction.value;
            currentCompress->next = current->next->next;
            current = currentCompress->next;
        }
        currentCompress = currentCompress->next;
    }
}

void BlockChainTransform(BlockChain& blockChain, updateFunction function) {
        BlockChain* current = &blockChain;
        while (current != nullptr) {
            current->transaction.value = function(current->transaction.value);
            current = current->next;
        }
}