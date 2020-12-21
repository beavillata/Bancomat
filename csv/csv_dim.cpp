#include <vector>

#include "csv_dim.h"

CSVDimension* CSVDimension::append(CSVCell* cell) {
  cellsVector.push_back(cell);
  return this;
}

void CSVDimension::clear() {
  cellsVector.clear();
}

CSVCell* CSVDimension::getCell(const int index) const {
  return cellsVector[index];
}

int CSVDimension::getSize() const {
  return cellsVector.size();
}

std::vector<CSVCell*> CSVDimension::getCells() const {
  return cellsVector;
}

char CSVCol::getType() const {
  return cellType;
}

CSVDimension& CSVDimension::operator<<(CSVCell& cell) {
  CSVCell* ptr = &cell;
  return *(append(ptr));
}

std::vector<int> CSVCol::has(void* target, const int limit,
  const int options) const {
  std::vector<int> match;

  int i = 0;
  for(CSVCell* cell: cellsVector) {
    std::string content, str;
    int dataSize, strSize;
    if(options != HAS_EXACT) {
      str = *(static_cast<std::string*>(target));
      content = *(static_cast<std::string*>(cell->get()));
    }

    switch(options) {
    case HAS_EXACT:
      if(cell->is(target)) match.push_back(i);
      break;
    case HAS_BEGIN:
      strSize = str.size();
      dataSize = content.size();
      if(strSize > dataSize) continue;

      if(content.substr(0, strSize) == str) {
        match.push_back(i);
      }
      break;
    case HAS_END:
      strSize = str.size();
      dataSize = content.size();
      if(strSize > dataSize) continue;
      
      if(content.substr(dataSize - strSize, strSize) == str) {
        match.push_back(i);
      }
      break;
    }

    int size = match.size();
    if(limit != -1 && size > limit) break;
    i++;
  }

  if(match.size() == 0) match.push_back(-1);
  return match;
}
