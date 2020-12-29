#ifndef ADMIN_H
#define ADMIN_H

class Admin {
public:
  static void handle();

private:
  static bool printCheques();
  static bool printTransfers();
  static void handleOperations();
  static void handleAdd();
  static void handleSub();
  static void handleCheque();
  static void handleTransfer();
  static void handleManage();
};

#endif
