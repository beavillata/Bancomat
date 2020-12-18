#include <vector>

#include "csv_dim.h"
#include "csv_cell.h"

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
