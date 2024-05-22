#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

using namespace std;

// The transaction class represents a single transaction in the blockchain
class Transaction
{
public:
    int number;
    string product_name;
    string from_user;
    string to_user;
    string shipping;
    double price;
    time_t timestamp;

    Transaction(int number, string product_name, string from_user, string to_user, string shipping, double price)
    {
        this->number = number;
        this->product_name = product_name;
        this->from_user = from_user;
        this->to_user = to_user;
        this->shipping = shipping;
        this->price = price;
        this->timestamp = time(0);
    }

    string hash() const
    {
        stringstream ss;
        ss << fixed << setprecision(2) << price << from_user << to_user << timestamp;
        return ss.str();
    }
};

// Block class to store information about each block in the blockchain
class Block
{
public:
    int index;
    string previous_hash;
    time_t timestamp;
    vector<Transaction> transactions;
    string hash;
    int nonce;

    Block(int index, string previous_hash, vector<Transaction> transactions)
    {
        this->index = index;
        this->previous_hash = previous_hash;
        this->transactions = transactions;
        this->timestamp = time(0);
        this->nonce = 0;
        this->hash = calculateHash();
    }

    string calculateHash() const
    {
        stringstream ss;
        ss << index << previous_hash << timestamp << nonce;
        for (const auto &tx : transactions)
        {
            ss << tx.hash();
        }
        string input = ss.str();
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char *)input.c_str(), input.length(), hash);
        stringstream ss2;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            ss2 << hex << setw(2) << setfill('0') << (int)hash[i];
        }
        return ss2.str();
    }

    void mineBlock(int difficulty)
    {
        string target(difficulty, '0');
        while (hash.substr(0, difficulty) != target)
        {
            nonce++;
            hash = calculateHash();
        }
        cout << "Block mined: " << hash << endl;
    }
};

// The blockchain class represents the entire blockchain
class Blockchain
{
public:
    vector<Block> chain;
    int difficulty;
    vector<Transaction> current_transactions;

    Blockchain(int difficulty)
    {
        this->difficulty = difficulty;
        chain.emplace_back(Block(0, "", current_transactions));
    }

    Block &latestBlock()
    {
        return chain.back();
    }

    void CreateAndAddBlock()
    {
        int index = chain.size();
        string previous_hash = latestBlock().hash;
        Block new_block(index, previous_hash, current_transactions);
        new_block.mineBlock(difficulty);
        chain.push_back(new_block);
        current_transactions.clear();
    }

    void addTransaction(Transaction transaction)
    {
        current_transactions.push_back(transaction);
    }

    void viewUser(string user) const
    {
        // Convert user input to lowercase
        for (char &c : user)
        {
            c = tolower(c);
        }
        cout << "Transactions involving user " << user << ":" << endl;
        bool found = false;
        for (const auto &block : chain)
        {
            for (const auto &tx : block.transactions)
            {
                // Convert transaction users to lowercase and compare
                string fromUserLower = tx.from_user;
                string toUserLower = tx.to_user;
                transform(fromUserLower.begin(), fromUserLower.end(), fromUserLower.begin(), ::tolower);
                transform(toUserLower.begin(), toUserLower.end(), toUserLower.begin(), ::tolower);
                if (fromUserLower == user || toUserLower == user)
                {
                    cout << "- " << tx.product_name << " from " << tx.from_user << " to " << tx.to_user << " on " << ctime(&tx.timestamp);
                    found = true;
                }
            }
        }
        if (!found)
        {
            cout << "No transactions found for user " << user << "." << endl;
        }
    }

    // Generate a random challenge
    string generateChallenge() const
    {
        unsigned char challenge[SHA256_DIGEST_LENGTH];
        RAND_bytes(challenge, SHA256_DIGEST_LENGTH);
        stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            ss << hex << setw(2) << setfill('0') << (int)challenge[i];
        }
        return ss.str();
    }

    // Create a response using HMAC
    string createResponse(const string &challenge, const string &sharedSecret, const int bit) const
    {
        unsigned char hmacResult[SHA256_DIGEST_LENGTH];
        HMAC(EVP_sha256(), sharedSecret.c_str(), sharedSecret.length(), (const unsigned char *)challenge.c_str(), challenge.length(), hmacResult, nullptr);
        stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            ss << hex << setw(2) << setfill('0') << (int)hmacResult[i];
        }
        ss << bit; // Include the bit in the response
        return ss.str();
    }

    // Verify the response
    bool verifyResponse(const string &challenge, const string &response, const string &sharedSecret, const int bit) const
    {
        string expectedResponse = createResponse(challenge, sharedSecret, bit);
        return response == expectedResponse;
    }
};

int main()
{
    int difficulty = 4;
    Blockchain blockchain(difficulty);
    string sharedSecret = "shared_secret";

    while (true)
    {
        int choice;
        cout << "Enter your choice: " << endl;
        cout << "1. Add a new block" << endl;
        cout << "2. View user transactions" << endl;
        cout << "3. Display the entire blockchain" << endl;
        cout << "4. Exit" << endl;

        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            cout << "Enter transaction details:" << endl;
            int number;
            string product_name, from_user, to_user, shipping;
            double price;
            cout << "Number: ";
            cin >> number;
            cout << "Product name: ";
            cin >> product_name;
            cout << "From user: ";
            cin >> from_user;
            cout << "To user: ";
            cin >> to_user;
            cout << "Shipping: ";
            cin >> shipping;
            cout << "Price: ";
            cin >> price;

            Transaction transaction(number, product_name, from_user, to_user, shipping, price);
            blockchain.addTransaction(transaction);

            // Step 1: Generate a random challenge and send it to the user (Alice)
            string challenge = blockchain.generateChallenge();

            // Step 2: Generate a random bit (0 or 1)
            int bit = rand() % 2;

            // Step 3: Alice creates a response using HMAC with the challenge and the bit received from Bob
            string response = blockchain.createResponse(challenge, sharedSecret, bit);

            // Step 4: Verify the response
            bool responseVerified = blockchain.verifyResponse(challenge, response, sharedSecret, bit);
            if (responseVerified)
            {
                cout << "Response verified. Adding block to the blockchain..." << endl;
                blockchain.CreateAndAddBlock();
            }
            else
            {
                cout << "Response verification failed. Block not added to the blockchain." << endl;
            }
            break;
        }
        case 2:
        {
            string user;
            cout << "Enter the username to view transactions: ";
            cin >> user;
            blockchain.viewUser(user);
            break;
        }
        case 3:
        {
            cout << "Displaying the entire blockchain:" << endl;
            for (const auto &block : blockchain.chain)
            {
                cout << "Block #" << block.index << endl;
                cout << "Hash: " << block.hash << endl;
                cout << "Previous Hash: " << block.previous_hash << endl;
                cout << "Timestamp: " << ctime(&block.timestamp);
                cout << "Transactions:" << endl;
                for (const auto &tx : block.transactions)
                {
                    cout << "- Number: " << tx.number << endl;
                    cout << "  Product Name: " << tx.product_name << endl;
                    cout << "  From User: " << tx.from_user << endl;
                    cout << "  To User: " << tx.to_user << endl;
                    cout << "  Shipping: " << tx.shipping << endl;
                    cout << "  Price: " << tx.price << endl;
                    cout << "  Timestamp: " << ctime(&tx.timestamp) << endl;
                }
                cout << endl;
            }
            break;
        }
        case 4:
        {
            exit(0);
            break;
        }
        default:
        {
            cout << "Invalid choice" << endl;
        }
        }
    }
    return 0;
}

