#ifndef CSVDIM_H
#define CSVDIM_H

#include "csv_cell.h"
#include <vector>

class CSVDimension {
public:
  CSVDimension(const int x): i(x) {};

  void append(CSVCell* cell);
  CSVCell* cell(int index);
  int size();
  std::vector<CSVCell*> cells();

private:
  int i;
  std::vector<CSVCell*> cellsVector;
};

class CSVRow: public CSVDimension {
public:
  CSVRow(const int x): CSVDimension(x) {};
};

class CSVCol: public CSVDimension {
public:
  CSVCol(const int x, const char ext): cellType(ext), CSVDimension(x) {};
  ~CSVCol();
  char type();

private:
  char cellType;
};

#endif
