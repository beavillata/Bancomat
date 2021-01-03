# Unito Bank
Program developed to handle an ATM. It is possible to login as a customer to withdraw or deposit money to the account or login as an admin fixing any problem or inconvenience.

**Customers** are able to:
* Know the current balance;
* View account movements;
* Withdraw;
* Deposit;
* Transfers between accounts (from users of the same bank or external banks);

**Admin** can:

* Check the current amount of money in the ATM machine;
* Withdraw money deposited by the users;
* Add money to permit withdraws by customers;
* Validate cheques;
* Validate transfers towards external banks;
* Enter customer accounts to fix problems;
* Reactivate blocked accounts;

**Control conditions** have been added to admit only valid transactions:
* If the pin inserted is wrong for three consecutive times the account is blocked and must be reactivated by an admin;
* It is impossible to withdraw more money than the ones currently on the account;
* It is impossible to withdraw, deposit or transfer negative amounts of money;
* The card number format is checked and it has to be a valid format (16 characters integer type);
* The pin must be of 5 characters integer type;

## Getting Started

### Prerequisites
* [GNU C++ Compiler](https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html)
* Make

### Installation

Download the repository from the project's page:
```
git clone
```
### Instructions
Compile the project from the terminal with:
```
make
```
The Makefile is responsible to manage the dependencies and will create the object files.
To clear the directory from this files between one make and another you can use:
```
make clean
```
After the execution of the compiled files
you will be direct to the login page.
Here the program will ask the card number and the pin of the user. It will then check if these are in the database and if they are correct.
The admin is able to login with a special account *(Card Number = 9999999999999999 Pin = 99999)*. This account is flagged by the *ID = 0*, giving a different menu after the authentication.

The user login page gives the following options to the customers:

0. **Logout**: exit from the user account and go back to the login page;
1. **Balance**: prints the current user's balance;
2. **Movements**: prints a table with the movements of the account. It displays:  
   * Beneficiary: if the movement was a transfer to another user it contains its card number. If it was a withdraw or a deposit it contains "SELF";
   * Amount: amount of the movement;
   * Date: displays date and time of the movement;
   * Type: displays the nature of the movement;
3. **Withdraw**: dispenses banknotes and coins for the amount requested by the user;
4. **Deposit**: it is possible to deposit cheques or cash. If the cheque option is selected the number of it is requested. It is then saved in the database;
5. **Transfer**: there are two kinds of transfers permitted:
   * Internal: if the Card Number of the beneficiary is in the database it permits the automatic exchange between two users;
   * External: it requires the name of the external bank and it has to be approved. For this reason the movement is saved on the external.csv and will then be checked and eventually allowed by the admin.


The admin login has the following options:

0. **Logout**: exit from the user account and go back to the login page;
1. **Balance**: prints the current ATM's balance;
2. **Take Cash**: allows the admin to withdraw cash from the ATM;
3. **Add Cash**: allows the admin to deposit cash to the ATM;
4. **Manage cheques**: the admin is responsible to validate a cheque, checking if it exists and if the number is correct. After that he can decide to *Approve* or *Refuse* them.
5. **Manage transfers**: when transfers to external banks are performed they need to be validated, checking if the bank exists and if the card number is correct. The admin is responsible to do that and he can then decide to *Approve* or *Refuse* them.
6. **Manage account**: the admin is able to enter customer's account by indicating the Card Number in order to fix problems or check movements;
7. **Reactivate account**: when a user tries to login but uses a wrong pin for three consecutive times the account is automatically blocked. The admin is able to reactivate it under request of the customer.

## Security

### Encryption

A system to store customer's data has been developed. To preserve those we elaborated an encryption method based on the XOR cipher.
The *csv* files are converted to *dat* using a secret key to make them unreadable.
For educative reasons the *encrypter* file is included between the file and it is possible to convert files to read them. Normally this must be avoided.

To encrypt or decrypt a file is possible to execute the following command line:
```
make encryption
```
this will generate an executable file that can be called by:
```
./encrypter
```
The program will ask you the name of the file you want to encrypt/decrypt and then if the conversion you want to do is from *dat* to *csv* or from *csv* to *dat*.
After the program ends a new file will be available.

## Password

To protect the system from brute force attacks a user can only insert a wrong password for three consecutive times. After that his account will be blocked and an admin will be required to unlock it. When a successful login is performed the count of wrong attempts is cleared.

Moreover, pin characters are hidden while typing to make the login process safer. Since this method requires interaction with the output system, full compatibility on windows machines is not ensured.

## UML

### Use case diagram

![alt text](https://github.com/hbar-boi/Bancomat/blob/master/Use_case_diagram.png?raw=true)

### Class

## Authors
* [Filippo Miserocchi](https://github.com/hbar-boi)
* [Beatrice Villata](https://github.com/beavillata)
* [Lorenzo Occelli](https://github.com/LoryOccy)
