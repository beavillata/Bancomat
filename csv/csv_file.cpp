#include <fstream>
#include <iostream>
#include <sstream>

#include "csv_file.h"

CSVFile* CSVFile::append(CSVRow* row) {
  rowsVector.push_back(row);
  rebase();
  return this;
}

CSVFile* CSVFile::insert(CSVRow* row, const int after) {
  if(after < n) {
    rowsVector.insert(rowsVector.begin() + after, row);
    rebase();
  } else {
    std::cout << "Unable to insert row: index out of bounds." << std::endl;
  }
  return this;
}

CSVFile* CSVFile::remove(const int index) {
  if(index < n) {
    rowsVector.erase(rowsVector.begin() + index);
    rebase();
  } else {
    std::cout << "Unable to remove row: index out of bounds." << std::endl;
  }
  return this;
}

CSVCell* CSVFile::getCell(const int i, const int j) const {
  return rowsVector[i]->getCell(j);
  // Or colsVector[j]->getCell(i); but preference is given to rows...
}

// Get all rows
std::vector<CSVRow*> CSVFile::getRows() const {
  return rowsVector;
}

// Get i-th row
CSVRow* CSVFile::getRow(const int i) const {
  return rowsVector[i];
}

// Get all cols
std::vector<CSVCol*> CSVFile::getCols() const {
  return colsVector;
}

// Get j-th col
CSVCol* CSVFile::getCol(const int j) const {
  return colsVector[j];
}

// Write database to file
void CSVFile::save() {
  std::ofstream file(path);
  std::stringstream ss;

  for(CSVRow* row: rowsVector) {
    int j = 0;
    for(CSVCell* cell: row->getCells()) {
      cell->stream(ss);
      if(j < m - 1) ss << TOKEN_SEPARATOR;
      j++;
    }
    ss << std::endl;
  }

  file << crypto(ss.str());
  file.close();
}

// Rebild cols and rows vector after adding new line
void CSVFile::rebase() {
  n = rowsVector.size();
  int j = 0;
  for(CSVCol* col: colsVector) {
    col->clear();
    for(CSVRow* row: rowsVector) {
      col->append(row->getCell(j));
    }
    j++;
  }
}

// Reload database from file
void CSVFile::reload() {
  // First, clean up heap
  clear();
  // Reset size
  n = 0; // Number of rows
  m = 0; // Number of columns

  std::ifstream file(path);
  if(!file.is_open()) { // Do we have a file?
    std::cout << "Unable to open file. Exiting..." << std::endl;
    exit(1);
  }
  // Extract file content to decrypt...
  std::stringstream in, out;
  in << file.rdbuf();
  out << crypto(in.str());
  std::string line;
  std::string head(1, COMMENT_HEAD);
  // Iterate through all lines in file
  while(getline(out, line)) {
    // Ignore comments
    if(line.substr(0, 1) == head) continue;
    // Instantiate new row to contain this line's data
    CSVRow* row = new CSVRow();
    rowsVector.push_back(row);
    for(std::string token: tokenize(line)) {
      // Count how many columns, need to do this only once
      if(n == 0) {
        // Also instantiate them
        colsVector.push_back(new CSVCol());
        m++;
      }
      // Instantiate cell and keep track of it...
      CSVCell* data = new CSVCell(token);
      row->append(data);
    }
    n++; // One more row parsed, starting over
  }
  // This is where we set up the columns as well...
  rebase();
  file.close();
}

std::string CSVFile::crypto(std::string value) {
  std::string fullKey(key);
  // XOR cipher works if key is at least as long as message
  while(fullKey.size() < value.size()) {
    fullKey += key;
  }
  for(std::string::size_type i = 0; i < value.size(); ++i) {
    value[i] ^= fullKey[i % fullKey.size()];
  }
  return value;
}

std::vector<std::string> CSVFile::tokenize(const std::string data) {
  std::vector<std::string> tokens;
  std::string separator(1, TOKEN_SEPARATOR);

  int current = 0, last = 0;
  while((current = data.find(separator, last)) != std::string::npos) {
    tokens.push_back(data.substr(last, current - last));
    last = current + 1;
  }
  tokens.push_back(data.substr(last, std::string::npos));
  return tokens;
}

// ==================== HEAP CLEANUP ==================== //

void CSVFile::clear() {
  for(CSVCol* column: colsVector) {
    delete column;
  }

  // This will also delete all CSVCell in heap
  for(CSVRow* row: rowsVector) {
    delete row;
  }
}

CSVFile& CSVFile::operator<<(CSVRow& row) {
  CSVRow* ptr = &row;
  return *(append(ptr));
}

CSVFile::~CSVFile() {
  clear();
}
