#ifndef OPERATIONS_H
#define OPERATIONS_H

class Operations {
public:
  static void printBalance();
  static void printMovements();
  static void initWithdraw();
  static void initDeposit();

private:
  static void addOperation(std::string, double, std::string);
};

#endif
