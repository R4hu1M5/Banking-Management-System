#Banking Management System
- Rahul Murali Shankar (IMT2017033)

#Run Instructions:
* Run 'gcc -o record record.c' and './record' to generate the files and initial values. The initial values are printed on the terminal to use later
* Open two terminals, run 'gcc -o server -pthread server.c' and './server' on one, and 'gcc -o client client.c' and './client' on the other (in that order)
* After the connection is established, follow the UI and use the login details provided by the output of record.c

#Note:
There is a getchar() on lines 103, 133 and 244 of server.c to test the locking mechanisms of the code (uncomment them to test it). To test the locking, run two clients on two different terminals connected to the same server and try to obtain two write locks (this should fail). To release the write lock on the file, press Enter on the server terminal.

#Files:
* user.h - defines all the header files and user structs
* record.c - Creates the files where the data is stored and adds the initial records to the files (Note that there is only one admin and no function to create another. Creating admins should not be a part of the user input)
* client.c - Contains the client side code of the system
* server.c - Contains the server side code of the system

#Contents of structs:
* Normal User (normal\_user) - Account Number (int account\_num), Username (char[20] username), Password (char[20] password), Account Balance (float balance), Status (int is\_deleted) to indicate whether an account is active or deleted
* Joint User (joint\_user) - Account Number (int account\_num), Username of account holder 1 (char[20] username1), Username of account holder 2 (char[20] username2), Password (char[20] password), Account Balance (float balance), Status (int is\_deleted) to indicate whether an account is active or deleted
* Administrator (admin) - Account Number (int account\_num), Username (char[20] username), Password (char[20] password)

#Functions:
* Normal and Joint Users - Deposit money, Withdraw money, Check balance, Change password, View account information
* Administrators - Add account, Modify account, Delete account, Search account
