#ifndef CSVDIM_H
#define CSVDIM_H

#include "csv_cell.h"

class CSVDimension {
public:
  CSVDimension* append(CSVCell*);
  int getSize() const;
  void clear();

  CSVCell* getCell(const int) const;
  std::vector<CSVCell*> getCells() const;

protected:
  std::vector<CSVCell*> cellsVector;
};

class CSVRow: public CSVDimension {

};

class CSVCol: public CSVDimension {
public:
  CSVCol(const char ext)
    : cellType(ext) {};
  char getType() const;
  std::vector<int> has(void*, const int) const;

private:
  char cellType;
};

#endif
