#ifndef ADMIN_H
#define ADMIN_H

class Admin {
public:
  // The only accessible option. Extra logic is taken care of inside
  // this function.
  static void handle();

private:
  static std::vector<int> printCheques();
  static std::vector<int> printTransfers();
  static void handleOperations();
  static void handleAdd();
  static void handleSub();
  static void handleCheque(std::vector<int>);
  static void handleTransfer(std::vector<int>);
  static void handleManage();
};

#endif
