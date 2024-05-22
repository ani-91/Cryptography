# Provenance tracking using blockchain and cryptography
The code is attempting to implement a blockchain using the C++ programming language and the OpenSSL library for cryptographic operations.


- The blockchain consists of blocks that store a list of transactions, and the transactions are verified using RSA public-key cryptography and Challenge-Response Authentication with HMAC.
- The blockchain class contains a vector of blocks, a difficulty level for the proof-of-work algorithm, and a vector of current transactions.
- Transactions can be added using the addTransaction function, which first verifies the transaction using the sender's public key and a Challenge-Response Authentication with HMAC,
and then adds it to the current transactions if it is valid.
- Blocks can be created and added to the chain using the CreateAndAddBlock function, which creates a new block with the current transactions and mines it using the proof-of-work algorithm.
- It is assumed that a block can contain any number of transactions and there can be any number of transactions between two users, the code verifies every transaction and adds it to a buffer. When all the transactions are safe and user calls CreateAndAddBlock function, it adds creates a new block with all the transactions present in the buffer. Even if one transaction is not safe, it discards all the transactions in the buffer made by the user.
- The Block class contains information about each block, including its index, previous hash, timestamp, list of transactions, hash, and nonce.
- The calculateHash function is used to calculate the hash of the block based on its index, previous hash, timestamp, nonce, and transactions.
- The mineBlock function is used to mine the block using the proof-of-work algorithm, which involves finding a nonce that results in a hash that starts with a certain number of zeros.

- The Transaction class represents a single transaction in the blockchain, and contains information about the product name, sender, receiver, shipping information, price, and timestamp.
- The hash function is used to calculate the hash of the transaction based on its amount, sender, receiver, and timestamp.

- The code also includes functions for generating RSA key pairs, generating random bit, challenges, responses, and verifying responses.
- The Blockchain class represents the entire blockchain and contains a list of blocks and the current transaction list.
- The class has methods to add a transaction to the current transaction list, verify the transaction using Challenge-Response Authentication with HMAC, the sender's private key(shared secret).
- The class also has a method to create and add a new block to the blockchain after mining it using PoW.

## To Run this code:
- To run this C++ blockchain code, you will need to have OpenSSL installed on your system.
- compile it with g++ code.cpp -o code -lssl -lcrypto
- Then after running add block and respective parameters such as orgin, pric, shipping , delivery,etc.
and then out of the 4 given options you can chose your desirable option to Add a new block  or view transactions of a particular user
- Display the blockchain or Exit.

## Output
![Screenshot 2024-05-22 210317](https://github.com/ani-91/Cryptography-Blockchain/assets/141425684/80e2d06c-087e-45c8-b118-f63509369681)
![Screenshot 2024-05-22 210449](https://github.com/ani-91/Cryptography-Blockchain/assets/141425684/f1239742-5901-4226-b7ba-e36cfa228629)
![Screenshot 2024-05-22 210510](https://github.com/ani-91/Cryptography-Blockchain/assets/141425684/a492b924-5a8d-4572-82cf-799fd2935cdd)


