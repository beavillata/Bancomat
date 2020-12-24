#include <iomanip>
#include <climits>
#include <cstdlib>

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "io.h"

const std::string IO::XOR_KEY("MOMENTOANGOLARE");

std::unique_ptr<CSVFile>
  IO::credentials{new CSVFile("persistent/credentials.dat", XOR_KEY)};
std::unique_ptr<CSVFile>
  IO::accounts{new CSVFile("persistent/accounts.dat", XOR_KEY)};
std::unique_ptr<CSVFile>
  IO::movements{new CSVFile("persistent/movements.dat", XOR_KEY)};
std::unique_ptr<CSVFile>
  IO::external{new CSVFile("persistent/external.dat", XOR_KEY)};

const std::string IO::TO_SELF("SELF"),
  IO::COORDINATE_SEPARATOR(" < "),
  IO::MOVEMENT_DEPOSIT_CASH("DEPOSIT: CASH"),
  IO::MOVEMENT_DEPOSIT_CHEQUE("DEPOSIT: CHEQUE"),
  IO::MOVEMENT_WITHDRAWAL("WITHDRAWAL"),
  IO::MOVEMENT_TRANSFER("TRANSFER"),
  IO::MOVEMENT_OK("OK"),
  IO::MOVEMENT_PENDING("PENDING"),
  IO::MOVEMENT_REFUSED("REFUSED");

const std::string IO::CURRENCY("EUR");

const std::vector<std::string> IO::OPTIONS_MAIN = {"Logout",
  "Balance", "Movements", "Withdraw", "Deposit", "Transfer"};

const std::vector<std::string> IO::OPTIONS_DEPOSIT = {"Cancel",
  "Cash deposit", "Cheque deposit"};

const std::vector<std::string> IO::OPTIONS_ADMIN = {"Logout",
  "Balance", "Take cash", "Add cash", "Manage cheques",
  "Manage transfers", "Manage account", "Reactivate accounts"};

const std::vector<std::string> IO::OPTIONS_CHEQUE = {"Cancel",
  "Approve cheque", "Refuse cheque"};

const std::vector<std::string> IO::OPTIONS_TRANSFER = {"Cancel",
  "Approve transfer", "Refuse transfer"};

int IO::prompt(std::vector<std::string> options) {
  int size = options.size();
  for(int i = 0; i < size; i++) {
    std::cout << std::left << "[" << i + 1 << "] " <<
      std::setw(15) << options[i];
    if(i % 3 == 2 && i != size - 1) std::cout << std::endl;
  }
  std::cout << std::endl << "Select option: ";
  std::string selected;
  int ret;
  if(inputNumber(selected, true, true, 1)) ret = stoi(selected) - 1;
  else ret = -1;
  std::cout << std::endl;
  return ret;
}

bool IO::inputNumber(std::string& ref, bool positive,
  bool integer, int digits) {
  std::cin >> ref;
  // Check that first char is a digit first, to use strtod.
  if(ref.empty() ||
    std::isspace(ref[0]) ||
    std::isalpha(ref[0])) return false;

  char* remainder;
  // Attempt to cast string to double, remainder becomes the position
  // of the non-number part in the string. If this value is zero all string
  // is number.
  double number = strtod(ref.c_str(), &remainder);
  if(*remainder > 0) return false;

  // Do we have a positive number?
  if(number <= 0 && positive) return false;

  // Do we have an integer?
  int decimalPosition = ref.find(".");
  bool decimalFound = (decimalPosition != std::string::npos);
  if(integer && decimalFound) return false;

  // Does our number have the right amount of digits?
  if(!decimalFound) decimalPosition = ref.size();
  if(digits != -1 && decimalPosition != digits) return false;
  return true;
}

int IO::getObfuscated() {
  int digit;

  struct termios termOld, termNew; //termios -> terminal interface
  //what we want to do is modify the terminal attributes (ex: interrupting the echoing)
  //for this we recall the data structure tcgetattr:

  //TCGETATTR(obj, termios) = gets the parameters associated with the object
  //and stores them in the termios structure
  tcgetattr(STDIN_FILENO, &termOld); //STDIN_FILENO -> descriptor of input standard file
  //I must get trace of the changes to the object
  termNew = termOld;
  //ICANON is responsible to decide if the terminal is operating
  //in canonical mode (ICANON set) or not (ICANON not set)

  //ECHO: when is set characters are echoed back, if it is clear input is not echoed

  //c_lflag is responsible to control the functions and it is composed using OR

  //I am setting both ICANON and ECHO to clear!
  termNew.c_lflag &= ~(ICANON | ECHO); //

  //effects on the terminal are not effective until tcsetattr() is called
  //tcsetattr(object, optional_actions, termios);
  //the optional_actions element is setted on TCSANOW -> that means that the change will
  //occurr immediately!
  tcsetattr(STDIN_FILENO, TCSANOW, &termNew);
  digit = getchar();
  //When I finish I want to restore everything back to normal:
  tcsetattr(STDIN_FILENO, TCSANOW, &termOld);

  return digit;
}

bool IO::inputPin(std::string& ref) {
  const char BACKSPACE = 127, RETURN = 10;

  unsigned char digit = 0;
  digit = getObfuscated();
  digit = 0;
  // Store digits till RETURN pressed
  while((digit = getObfuscated()) != RETURN) {
    // Handle backspace
    //I have to add this because I changed ICANON to noncanonical and it doesn't
    //evaluate the backspaces anymore when in this mode
    if(digit == BACKSPACE) {
      if(ref.length() != 0) {
        std::cout <<"\b \b"; //\b moves the cursor back. I have to also add a space after it
        //to delete the text
        ref.resize(ref.length() - 1);
      }
    } else {
      ref += digit;
      std::cout << "*";
    }
  }

  char* remainder;
  double number = strtod(ref.c_str(), &remainder);

  int decimalPosition = ref.find(".");
  bool decimalFound = (decimalPosition != std::string::npos);

  if(*remainder > 0 || number <= 0 ||
    decimalFound || ref.size() != 5) return false;

  return true;
}

void IO::printRow(std::vector<cell> data, bool header) {
  for(cell element: data) {
    if(element.align == IO::ALIGN_LEFT) std::cout << std::left;
    else std::cout << std::right;

    std::cout << std::setw(element.width) << element.content;
  }
  std::cout << std::endl;
  if(header) std::cout << std::endl;
}

std::string IO::getDate() {
  time_t timer = time(NULL);
  char datec[26];
  strftime(datec, 26, "%Y/%m/%d %H:%M:%S", localtime(&timer));

  std::string date(datec);
  return date;
}
