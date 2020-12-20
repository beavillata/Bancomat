#include <fstream>
#include <iostream>

#include "csv_file.h"

const char CELL_TYPE_INT = 'i',
  CELL_TYPE_DOUBLE = 'd',
  CELL_TYPE_STRING = 's',
  TOKEN_SEPARATOR = ',',
  COMMENT_HEAD = '#';

CSVFile::~CSVFile() {
  clear();
}

CSVFile* CSVFile::append(const CSVRow *row) {
  cellsVector.push_back(row->getCells());
  n++;
  rebase();

  return this;
}

CSVFile* CSVFile::insert(const CSVRow *row, const int after) {
  cellsVector.insert(cellsVector.begin() + after, row->getCells());
  n++;
  rebase();

  return this;
}

CSVFile* CSVFile::remove(const int index) {
  cellsVector.erase(cellsVector.begin() + index);
  n--;
  rebase();

  return this;
}

// ==================== GETTERS ==================== //

CSVCell* CSVFile::getCell(const int col, const int row) const {
  return cellsVector[row][col];
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

// Get i-th col
CSVCol* CSVFile::getCol(const int i) const {
  return colsVector[i];
}

// ==================== WRITE CHANGES TO FILE ==================== //

// Write database to file
void CSVFile::save() {
  std::ofstream file(path);
  for(int i = -1; i < n; i++) {
    for(int j = 0; j < m; j++) {
      if(i == -1) {
        file << colsVector[j]->getType();
      } else {
        cellsVector[i][j]->stream(file);
      }
      if(j < m - 1) {
        file << TOKEN_SEPARATOR;
      }
    }
    file << std::endl;
  }
  file.close();
}

// ==================== POINTER CLEANUP ==================== //

void CSVFile::clear() {
  for(CSVCol* column: colsVector) {
    delete column;
  }

  for(CSVRow* row: rowsVector) {
    delete row;
  }

  for(std::vector<CSVCell*> row: cellsVector) {
    for(CSVCell* cell: row) {
      delete cell;
    }
  }
}

void CSVFile::rebase() {
  rowsVector.resize(n, new CSVRow());

  for(int i = 0; i < n; i++) { // Counting rows
    rowsVector[i]->clear();
    for(int j = 0; j < m; j++) { // Counting cols
      if(i == 0) {
        colsVector[j]->clear();
      }
      rowsVector[i]->append(cellsVector[i][j]);
      colsVector[j]->append(cellsVector[i][j]);
    }
  }
}

// Reload database from file
void CSVFile::reload() {
  clear();

  n = 0; // Number of rows
  m = 0; // Number of columns

  std::ifstream file(path);
  if(!file.is_open()) { // Do we have a file?
    std::cout << "Unable to open file. Exiting..." << std::endl;
    exit(1);
  }
  CSVCol* column;
  CSVRow* row;
  std::string line;
  std::string head(1, COMMENT_HEAD);
  // Iterate through all lines in file
  n--; // Go back one line to have nice indices
  while(getline(file, line)) {
    if(line.substr(0, 1) == head) {
      // Ignore comments
      continue;
    }

    if(n >= 0) {
      row = new CSVRow();
      rowsVector.push_back(row);
    }
    // Tokenize line using comma as separator
    int j = 0; // Current column in single row loop
    for(std::string token: tokenize(line)) {
      // Build column array with the right types
      if(n < 0) { // We are on the first line (header)
        // Instantiate new column with correct index and type
        column = new CSVCol(token.c_str()[0]);
        colsVector.push_back(column); // And add it to the std::vector
        m++; // Increase column number counter
      } else { // We are parsing actual data
        column = colsVector[j];
        char type = column->getType();
        CSVCell* data;
        switch(type) {
        case CELL_TYPE_INT:
          data = new CSVData<int>(stoi(token));
          break;
        case CELL_TYPE_DOUBLE:
          data = new CSVData<double>(stod(token));
          break;
        case CELL_TYPE_STRING:
          data = new CSVData<std::string>(token);
          break;
        }
        row->append(data);
        column->append(data);
      }
      j++; // j-th column of n-th row parsed
    }
    if(n >= 0) {
      cellsVector.push_back(row->getCells());
    }
    n++; // One more row parsed, starting over
  }

  file.close();
}

void CSVFile::print() {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      std::cout << "\t";
      cellsVector[i][j]->stream(std::cout);
    }
    std::cout << std::endl;
  }
}

std::vector<std::string> CSVFile::tokenize(const std::string data) {
  int current = 0, last = 0;
  std::vector<std::string> tokens;
  std::string separator(1, TOKEN_SEPARATOR);
  while((current = data.find(separator, last)) != std::string::npos) {
    tokens.push_back(data.substr(last, current - last));
    last = current + 1;
  }
  tokens.push_back(data.substr(last, std::string::npos));
  return tokens;
}
