#include "csv_file.h"

#include <string>
#include <iostream>

int main() {
  CSVFile* file = new CSVFile("test.csv");

  file->print();

  file->cell(2, 0)->sset("ciao");

  file->print();

  file->save();

  std::string in("98437598435937");

  for(CSVCell* cell: file->col(0)->cells()) {
    if(cell->sget() == in) {
      std::cout << "Si" << std::endl;
      break;
    }
  }

  delete file;
  return 0;
}
