#include <vector>

#include "csv_dim.h"

void CSVDimension::append(CSVCell* cell) {
  cellsVector.push_back(cell);
}

void CSVDimension::clear() {
  cellsVector.clear();
}

CSVCell* CSVDimension::cell(int index) {
  return cellsVector[index];
}

int CSVDimension::size() {
  return cellsVector.size();
}

std::vector<CSVCell*> CSVDimension::cells() {
  return cellsVector;
}

char CSVCol::type() {
  return cellType;
}

std::vector<int> CSVCol::has(void* target, int limit) {
  std::vector<int> match;
  int i = 0;
  for(CSVCell* cell: cellsVector) {
    if(cell->is(target)) {
      match.push_back(i);
    }
    int size = match.size();
    if(limit != -1 && size > limit) {
      break;
    }
    i++;
  }

  if(match.size() == 0) {
    match.push_back(-1);
  }
  return match;
}
