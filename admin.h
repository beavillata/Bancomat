#ifndef ADMIN_H
#define ADMIN_H

class Admin {
public:
  static void handle();
  static void handleOperations();
  static void handleAdd();
  static void handleSub();
  static void handleCheque();
  static void handleTransfer();
  static void printCheques();
  static void printTransfers();
};

#endif
