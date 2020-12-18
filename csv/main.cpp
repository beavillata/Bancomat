#include "csv_file.h"

#include <string>
#include <iostream>

int main() {
  CSVFile* file = new CSVFile("test.csv");

  file->print();

  std::string username("aaa");
  int row = file->col(0)->has(&username);
  if(row > -1) {
    std::cout << file->row(row)->cell(1)->sget() << std::endl;
  }

  delete file;
  return 0;
}
