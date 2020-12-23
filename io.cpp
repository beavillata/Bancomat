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
  "Manage transfers", "Manage account"};

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

int getch() {
    int ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}

std::string getpass(const char *prompt, bool show_asterisk=true)
{
  const char BACKSPACE=127;
  const char RETURN=10;

  std::string password;
  unsigned char ch=0;
  ch=getch();
  ch=0;
  while((ch=getch())!=RETURN)
    {
       if(ch==BACKSPACE)
         {
            if(password.length()!=0)
              {
                 if(show_asterisk)
                 std::cout <<"\b \b";
                 password.resize(password.length()-1);
              }
         }
       else
         {
             password+=ch;
             if(show_asterisk)
                 std::cout <<'*';
         }
    }
  return password;
}

bool IO::inputPin(std::string& ref, bool positive,
  bool integer, int digits) {

  ref=getpass(" ",true); // Show asterisks -> if false it doesn't show them

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
  std::cout << std::endl;
  return true;
}

std::string IO::getDate() {
  time_t timer = time(NULL);
  char datec[26];
  strftime(datec, 26, "%Y/%m/%d %H:%M:%S", localtime(&timer));

  std::string date(datec);
  return date;
}
