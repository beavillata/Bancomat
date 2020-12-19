#include <fstream>
#include <iostream>

#include "csv_file.h"

const char CELL_TYPE_INT = 'i',
  CELL_TYPE_DOUBLE = 'd',
  CELL_TYPE_STRING = 's',
  TOKEN_SEPARATOR = ',',
  COMMENT_HEAD = '#';

CSVFile::~CSVFile() {
  for(CSVCol* column: colsVector) {
    delete column;
  }

  for(CSVRow* row: rowsVector) {
    delete row;
  }
}

// Get all rows
std::vector<CSVRow*> CSVFile::rows() {
  return rowsVector;
}

void CSVFile::append(CSVRow *row) {
  rowsVector.push_back(row);
  n++;
}

void CSVFile::insert(CSVRow *row, const int after) {
  rowsVector.insert(rowsVector.begin() + after, row);
  n++;
}

void CSVFile::remove(const int index) {
  rowsVector.erase(rowsVector.begin() + index);
  n--;
}

CSVCell* CSVFile::cell(const int col, const int row) {
  return colsVector[col]->cell(row);
}

// Get i-th row
CSVRow* CSVFile::row(const int i) {
  return rowsVector[i];
}

// Get all cols
std::vector<CSVCol*> CSVFile::cols() {
  return colsVector;
}

// Get i-th col
CSVCol* CSVFile::col(const int i) {
  return colsVector[i];
}

// Write database to file
void CSVFile::save() {
  std::ofstream file(path);
  for(int i = -1; i < n; i++) {
    for(int j = 0; j < m; j++) {
      if(i == -1) {
        file << colsVector[j]->type();
      } else {
        rowsVector[i]->cell(j)->stream(file);
      }
      if(j < m - 1) {
        file << TOKEN_SEPARATOR;
      }
    }
    file << std::endl;
  }
  file.close();
}

// Reload database from file
void CSVFile::reload() {
  n = 0; // Number of rows
  m = 0; // Number of columns

  std::ifstream file(path);
  if(!file.is_open()) { // Do we have a file?
    std::cout << "Unable to open file. Exiting..." << std::endl;
    exit(1);
  }
  std::string line;
  std::string head(1, COMMENT_HEAD);
  // Iterate through all lines in file
  n--; // Go back one line to have nice indices
  while(getline(file, line)) {
    if(line.substr(0, 1) == head) {
      continue;
    }
    int j = 0; // Current column in single row loop
    // Tokenize line using comma as separator
    for(std::string token: tokenize(line)) {
      // Build column array with the right types
      if(n < 0) { // We are on the first line (header)
        // Instantiate new column with correct index and type
        CSVCol* column = new CSVCol(m, token.c_str()[0]);
        colsVector.push_back(column); // And add it to the std::vector
        m++;
      } else { // We are parsing actual data
        CSVCol* current = colsVector[j]; // Select current column
        CSVCell* cell;
        char type = current->type();
        switch(type) {
        case CELL_TYPE_INT:
          cell = new CSVData<int>(j, n, stoi(token));
          break;
        case CELL_TYPE_DOUBLE:
          cell = new CSVData<double>(j, n, stod(token));
          break;
        case CELL_TYPE_STRING:
          cell = new CSVData<std::string>(j, n, token);
          break;
        }
        current->append(cell);
      }
      j++; // j-th column of n-th row parsed
    }
    n++; // One more row parsed, starting over
  }
  // Parsing the file generated our columns, now
  // generate rows
  for(int i = 0; i < n; i++) {
    CSVRow* current = new CSVRow(i);
    for(CSVCol* column: colsVector) {
      current->append(column->cell(i));
    }
    rowsVector.push_back(current);
  }

  file.close();
}

void CSVFile::print() {
  for(CSVRow* row: rowsVector) {
    for(CSVCell* cell: row->cells()) {
      cell->stream(std::cout);
      std::cout << "\t";
    }
    std::cout << std::endl;
  }
}

std::vector<std::string> CSVFile::tokenize(std::string data) {
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
