#ifndef CSVDIM_H
#define CSVDIM_H

#include "csv_cell.h"

class CSVDimension {
public:
  CSVDimension* append(CSVCell*);

  // Unused, for now
  CSVDimension& operator<<(CSVCell&);

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
  std::vector<int> has(void*, const int limit = -1,
    const int options = HAS_EXACT) const;

  static inline const int HAS_EXACT = 0,
    HAS_BEGIN = 1,
    HAS_END = 2;


private:
  char cellType;
};

#endif
