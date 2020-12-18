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

protected:
  std::vector<CSVCell*> cellsVector;

private:
  int i;
};

class CSVRow: public CSVDimension {
public:
  CSVRow(const int x)
    : CSVDimension(x) {};
};

class CSVCol: public CSVDimension {
public:
  CSVCol(const int x, const char ext)
    : cellType(ext), CSVDimension(x) {};
  ~CSVCol();
  char type();
  int has(void* target);

private:
  char cellType;
};

#endif
