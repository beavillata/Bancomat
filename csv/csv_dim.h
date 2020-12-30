#ifndef CSVDIM_H
#define CSVDIM_H

#include "csv_cell.h"

/*  CSVDimension is a parent class of both CSVRow and CSVCol.
 *  While CSVCol is much more powerful than CSVRow, both support some
 *  basic operations that are provided by this class
 */
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
  // has() and first() can be used with std::string, int or double.
  // By default there is no limit to the amount of matches it looks for and
  // wants the cell's content to be exactly the _target_ value.
  std::vector<int> has(std::string, int limit = -1,
    int options = HAS_EXACT) const;
  std::vector<int> has(double target, int limit = -1,
    int options = HAS_EXACT) const { return has(std::to_string(target)); };
  std::vector<int> has(int target, int limit = -1,
    int options = HAS_EXACT) const { return has(std::to_string(target)); };

  bool first(std::string, int&, int options = HAS_EXACT) const;
  bool first(double target, int& dest, int options = HAS_EXACT) const {
    return first(std::to_string(target), dest, options);
  };
  bool first(int target, int& dest, int options = HAS_EXACT) const {
    return first(std::to_string(target), dest, options);
  };

  static inline const int HAS_EXACT = 0,
    HAS_BEGIN = 1,
    HAS_END = 2;
};

class CSVRow: public CSVDimension {
public:
  ~CSVRow();
};

#endif
