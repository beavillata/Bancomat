#ifndef ADMIN_H
#define ADMIN_H

class Admin {
public:
  static void handleOperations();
  static void handle();
  static void addInSafetyBox();
  static void subInSafetyBox();
  static void checkCheque();
  static void checkTransfer();
  static void printChequeToCheck();
  static void printTransferToCheck();
};

#endif
