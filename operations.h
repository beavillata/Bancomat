#ifndef OPERATIONS_H
#define OPERATIONS_H

class Operations {
public:
  static void printBalance();
  static void printMovements();
  static void handleWithdraw();
  static void handleDeposit();
  static void handleTransfer();

private:
  static void addMovement(std::string, double, std::string, int);
  static std::string getDate();
};

#endif
