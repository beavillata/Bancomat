#include <vector>

#include "csv_dim.h"

CSVDimension* CSVDimension::append(CSVCell* cell) {
  cellsVector.push_back(cell);
  return this;
}

CSVCell* CSVDimension::getCell(const int index) const {
  return cellsVector[index];
}

std::vector<CSVCell*> CSVDimension::getCells() const {
  return cellsVector;
}

int CSVDimension::getSize() const {
  return cellsVector.size();
}

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
  if(match.size() == 0) match.push_back(-1);
  return match;
}

void CSVDimension::clear() {
  cellsVector.clear();
}

CSVDimension& CSVDimension::operator<<(CSVCell& cell) {
  CSVCell* ptr = &cell;
  return *(append(ptr));
}

CSVRow::~CSVRow() {
  for(CSVCell* cell: cellsVector) {
    delete cell;
  }
}
