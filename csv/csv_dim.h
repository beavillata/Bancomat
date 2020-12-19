#ifndef CSVDIM_H
#define CSVDIM_H

#include "csv_cell.h"

class CSVDimension {
public:
  CSVDimension(const int x): i(x) {};

  void append(CSVCell*);
  CSVCell* cell(int);
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
  std::vector<int> has(void*, int);

private:
  char cellType;
};

#endif
