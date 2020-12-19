#ifndef CSVDIM_H
#define CSVDIM_H

#include "csv_cell.h"

class CSVDimension {
public:
  void append(CSVCell*);
  int size();
  void clear();
  
  CSVCell* cell(int);
  std::vector<CSVCell*> cells();

protected:
  std::vector<CSVCell*> cellsVector;
};

class CSVRow: public CSVDimension {

};

class CSVCol: public CSVDimension {
public:
  CSVCol(const char ext)
    : cellType(ext) {};
  char type();
  std::vector<int> has(void*, int);

private:
  char cellType;
};

#endif
