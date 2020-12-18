#include "csv_file.h"

#include <string>
#include <iostream>

int main() {
  std::string path("test.csv");
  CSVFile* file = new CSVFile(path);
  std::string ciao("ticotic");
  file->cell(1, 0)->sset(ciao);
  CSVRow* row = new CSVRow(6);
  for(int i = 0; i < 6; i++) {
    row->append(new CSVData<std::string>(1, 1, "bubu"));
  }
  file->append(row);
  file->save();
  delete file;
  return 0;
}
