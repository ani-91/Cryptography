
The code is attempting to implement a blockchain using the C++ programming language and the OpenSSL library for cryptographic operations.

- The blockchain consists of blocks that store a list of transactions, and the transactions are verified using RSA public-key cryptography and Challenge-Response Authentication with HMAC.
- The blockchain class contains a vector of blocks, a difficulty level for the proof-of-work algorithm, and a vector of current transactions.
- Transactions can be added using the addTransaction function, which first verifies the transaction using the sender's public key and a Challenge-Response Authentication with HMAC,
and then adds it to the current transactions if it is valid.
- Blocks can be created and added to the chain using the CreateAndAddBlock function, which creates a new block with the current transactions and mines it using the proof-of-work algorithm.

- The Block class contains information about each block, including its index, previous hash, timestamp, list of transactions, hash, and nonce.
- The calculateHash function is used to calculate the hash of the block based on its index, previous hash, timestamp, nonce, and transactions.
- The mineBlock function is used to mine the block using the proof-of-work algorithm, which involves finding a nonce that results in a hash that starts with a certain number of zeros.

- The Transaction class represents a single transaction in the blockchain, and contains information about the product name, sender, receiver, shipping information, price, and timestamp.
- The hash function is used to calculate the hash of the transaction based on its amount, sender, receiver, and timestamp.

- The code also includes functions for generating RSA key pairs, generating random bit, challenges, responses, and verifying responses.
- The Blockchain class represents the entire blockchain and contains a list of blocks and the current transaction list.
- The class has methods to add a transaction to the current transaction list, verify the transaction using Challenge-Response Authentication with HMAC, the sender's private key(shared secret).
- The class also has a method to create and add a new block to the blockchain after mining it using PoW.

To Run this code:
- To run this C++ blockchain code, you will need to have OpenSSL installed on your system.
- Then after running add block and respective parameters such as orgin, pric, shipping , delivery,etc.
and then out of the 4 given options you can chose your desirable option to Add a new block  or view transactions of a particular user
- Display the blockchain or Exit.
