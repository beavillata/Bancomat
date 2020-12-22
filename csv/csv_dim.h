#ifndef CSVDIM_H
#define CSVDIM_H

#include "csv_cell.h"

class CSVDimension {
public:
  CSVDimension* append(CSVCell*);
  CSVCell* getCell(const int) const;
  std::vector<CSVCell*> getCells() const;

  int getSize() const;

  void clear();
  // Unused, for now
  CSVDimension& operator<<(CSVCell&);

protected:
  // Has to be accessible from CSVRow and CSVCol
  std::vector<CSVCell*> cellsVector;
};

class CSVCol: public CSVDimension {
public:
  std::vector<int> has(std::string, int limit = -1,
    int options = HAS_EXACT) const;
  std::vector<int> has(double target, int limit = -1,
    int options = HAS_EXACT) const { return has(std::to_string(target)); };
  std::vector<int> has(int target, int limit = -1,
    int options = HAS_EXACT) const { return has(std::to_string(target)); };

  static inline const int HAS_EXACT = 0,
    HAS_BEGIN = 1,
    HAS_END = 2;
};

class CSVRow: public CSVDimension {
public:
  ~CSVRow();
};

#endif
