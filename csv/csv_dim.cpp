#include <vector>

#include "csv_dim.h"

void CSVDimension::append(CSVCell* cell) {
  cellsVector.push_back(cell);
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

CSVCol::~CSVCol() {
  for(CSVCell* cell: cells()) {
    delete cell;
  }
}

char CSVCol::type() {
  return cellType;
}

int CSVCol::has(void* target) {
  int i = 0;
  for(CSVCell* cell: cellsVector) {
    if(cell->is(target)) {
      return i;
    }
    i++;
  }
  return -1;
}
