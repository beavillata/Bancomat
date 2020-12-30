#include <vector>

#include "csv_dim.h"

// Append a cell at the end of this dimension (column or row that is).
CSVDimension* CSVDimension::append(CSVCell* cell) {
  cellsVector.push_back(cell);
  return this;
}

// Get the cell indexed by _index_ belonging to this dimension.
CSVCell* CSVDimension::getCell(const int index) const {
  return cellsVector[index];
}

// Get all the cells
std::vector<CSVCell*> CSVDimension::getCells() const {
  return cellsVector;
}

int CSVDimension::getSize() const {
  return cellsVector.size();
}

/*  This takes a _target_ string and looks for a match
 *  (depending on _options_) in a column. It returns a vector of indices
 *  that refer to the rows where there was a match. The possible options
 *  include HAS_EXACT, where a match is only valid if that cell's value
 *  is exactly _target_, HAS_BEGIN which is happy with only a match at the
 *  beginning of the cell's content and HAS_END which works the same way but
 *  for the content's ending.
 *
 *  If no matches are found the vector returned will be empty.
 *  _limit_, if specified, allows to stop searching after the provided
 *  number of matches has been found.
 */
std::vector<int> CSVCol::has(std::string target, const int limit,
  const int options) const {
  std::vector<int> match;
  int i = 0;
  for(CSVCell* cell: cellsVector) {
    switch(options) {
    case HAS_EXACT:
      if(cell->is(target)) match.push_back(i);
      break;
    case HAS_BEGIN:
      if(cell->starts(target)) match.push_back(i);
      break;
    case HAS_END:
      if(cell->ends(target)) match.push_back(i);
      break;
    }
    int size = match.size();
    if(limit != -1 && size >= limit) break;
    i++;
  }
  return match;
}

/*  This is useful when only looking for a single row. It works the same way as
 *  has() but only looks for one match. If it finds something, it returns true,
 *  otherwise false. The index of the found (matching) row will be placed in
 *  _dest_.
 */
bool CSVCol::first(std::string target, int& dest, const int options) const {
  std::vector<int> result = has(target, 1, options);
  if(result.size() == 0) return false;

  dest = result[0];
  return true;
}

// Remove all pointers that are being kept track of.
void CSVDimension::clear() {
  cellsVector.clear();
}

CSVDimension& CSVDimension::operator<<(CSVCell& cell) {
  CSVCell* ptr = &cell;
  return *(append(ptr));
}

// ==================== HEAP CLEANUP ==================== //

// If we are deleting a row we also need to take care of the cells.
CSVRow::~CSVRow() {
  for(CSVCell* cell: cellsVector) {
    delete cell;
  }
}
