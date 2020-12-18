#include "csv_file.h"

#include <string>
#include <iostream>

int main() {
  std::string path("test.csv");
  CSVFile c(path);
  c.save();
  return 0;
}
