
#pragma once

#include <string>
#include <fstream>

#include "Transaction.h"

using std::string;
using std::ifstream;
using std::ofstream;


typedef unsigned int (*updateFunction)(unsigned int);


/**
*
 * BlockChain - Defining the new BlockChain Type
 *
*/
struct BlockChain {
    BlockChain* next;
    Transaction transaction;
    string time;
};


/**
 * BlockChainGetSize - returns the number of Blocks in the BlockChain
 *
 * @param blockChain - BlockChain to measure
 *
 * @return Number of Blocks in the BlockChain
*/
int BlockChainGetSize(const BlockChain& blockChain) {
    int ctr = 0;
    const BlockChain* current = &blockChain;
    while (current != nullptr) {
        ctr++;
        current = current->next;
    }
    return ctr;
}


/**
 * BlockChainPersonalBalance - returns the balance of a given person, relative to a given BlockChain
 *
 * @param blockChain - BlockChain to calculate the balance from
 * @param name - Name of the person to calculate the balance for
 *
 * @return Balance of the person
*/
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


/**
 * BlockChainAppendTransaction - creates and appends a new transaction to the BlockChain
 *
 * @param blockChain BlockChain to append the transaction to
 * @param value Value of the transaction
 * @param sender Name of the sender
 * @param receiver Name of the receiver
 * @param timestamp String that holds the time the transaction was made
*/
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


/**
 * BlockChainAppendTransaction - appends a copy of a given transaction to the BlockChain
 *
 * @param blockChain BlockChain to append the transaction to
 * @param transaction Transaction we want to append
 * @param timestamp String that holds the time the transaction was made
*/
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


/**
 * BlockChainLoad - Reads data from a file and creates a new block chain
 *
 * @param file Data file to read from
 *
 * @return BlockChain created from the file
 *
*/
BlockChain BlockChainLoad(ifstream& file) {
    BlockChain* head = nullptr;
    BlockChain* temp = nullptr;
    

    string line, sender, receiver, valueStr, timestamp;
    sender = receiver = valueStr = timestamp = "";
    int value;

    while(getline(file, line)) {
        
        Transaction* transaction = new Transaction;
        BlockChain* current = new BlockChain();
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
        transaction->sender = sender;
        transaction->receiver = receiver;
        transaction->value = value;
        current->time = timestamp;
        current->transaction = *transaction;

        if (head == nullptr) {
            head = current;
            temp = head;
        } else {
            temp->next = current;
            temp = temp->next;
        }
        sender = receiver = valueStr = timestamp = "";
    }
    return *head;
}


/**
 * BlockChainDump - Prints the data of all transactions in the BlockChain to a given file
 *
 * Data will be printed in the following format:
 *
 * BlockChain info:
 * <n>.
 * Sender Name: <name>
 * Receiver Name: <name>
 * Transaction Value: <value>
 * Transaction Timestamp: <time>
 *
 * @param blockChain BlockChain to print
 * @param file File to print to
 *
*/
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



/**
 * BlockChainDumpHashed - Prints the *hashed data* of all transactions in the BlockChain to a given file
 *
 * Data will be printed in the following format:
 * <hashed message>
 * <hashed message>
 * ...
 * <hashed message>
 *
 * @param blockChain BlockChain to print
 * @param file File to print to
 *
*/
void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file) {
    const BlockChain* current = &blockChain;
    while (current != nullptr) {
        if (current->next != nullptr) {
            file << TransactionHashMessage(blockChain.transaction) << std::endl;
        } else {
            file << TransactionHashMessage(blockChain.transaction);
        }
        current = current->next;
    }
}


/**
 * BlockChainVerifyFile - verifies that the file contains correct hashed messages of the given BlockChain
 *
 * Input file is expected to contain data in the following format:
 * <hashed message>
 * <hashed message>
 * ...
 * <hashed message>
 *
 * @param blockChain BlockChain to verify
 * @param file File to read from
 *
 * @return true if the file is valid, false otherwise
*/
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


/**
 * BlockChainCompress - Compresses the given block chain based on the transaction's data.
 * All consecutive blocks with the same sender and receiver will be compressed to one Block.
 *
 * @param blockChain BlockChain to compress
*/
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


/**
 * BlockChainTransform - Update the values of each transaction in the BlockChain
 *
 * @param blockChain BlockChain to update
 * @param function a pointer to a transform function
*/
void BlockChainTransform(BlockChain& blockChain, updateFunction function) {
        BlockChain* current = &blockChain;
        while (current != nullptr) {
            current->transaction.value = function(current->transaction.value);
            current = current->next;
        }
}
