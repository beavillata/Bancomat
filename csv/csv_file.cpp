#include "csv_file.h"

/*  By instantiating a CSVFile object one can control rows, columns
 *  and individual cells of a .dat file (a ciphered .csv under the hood).
 *
 *  Rows can be added, removed and the database can be written back to
 *  disk and automatically (de)ciphered to make it transparent to the user.
 *
 *  Internally data is kept track of by means of two vectors (rows and columns)
 *  pointing to the same CSVCell objects but organized in two different ways.
 *
 *  When one of the two vectors is altered the data contained in the other
 *  isn't, and they go out of alignment. To fix this, rebase() has to be called.
 *
 *  All objects are kept in the heap and garbage collected.
 */

// Add a row to the database and return an instance of this database.
CSVFile* CSVFile::append(CSVRow* row) {
  rowsVector.push_back(row);
  // Need this to keep track of the added row in the cols vector as well.
  rebase();
  // Enable concatenation
  return this;
}

// Insert a row after another one (indexed by _after_) and return
// an instance of this database.
CSVFile* CSVFile::insert(CSVRow* row, const int after) {
  // Make sure we are not trying to append a row instead.
  if(after < n) {
    rowsVector.insert(rowsVector.begin() + after, row);
    rebase();
  } else {
    std::cout << "Unable to insert row: index out of bounds." << std::endl;
  }
  return this;
}

// Remove a row indexed by _index_ and return an instance of this database.
CSVFile* CSVFile::remove(const int index) {
  // Make sure the row actually exists.
  if(index < n) {
    rowsVector.erase(rowsVector.begin() + index);
    rebase();
  } else {
    std::cout << "Unable to remove row: index out of bounds." << std::endl;
  }
  return this;
}

// Get cell object from _i_-th row and _j_-th column.
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

  // Iterate through each row...
  for(CSVRow* row: rowsVector) {
    int j = 0;
    // ...and then through each column (cell) in that row.
    for(CSVCell* cell: row->getCells()) {
      // First stream the cell content's to ss.
      cell->stream(ss);
      // Then if we are not on the last column add the TOKEN_SEPARATOR
      // termination character.
      if(j < m - 1) ss << TOKEN_SEPARATOR;
      j++;
    }
    // Lastly add a newline as row termination character.
    ss << std::endl;
  }

  // Cipher our completed stream and insert the result into the file.
  file << crypto(ss.str());
  file.close();
}

// Rebuild cols vector after adding new line
void CSVFile::rebase() {
  // How many rows do we have now?
  n = rowsVector.size();
  int j = 0;
  // Iterate through each column.
  for(CSVCol* col: colsVector) {
    // First, empty the column from all the old cells.
    // This is needed as removing a line from the database
    // could mess up the alignment.
    col->clear();
    // Iterate through each row once per column...
    for(CSVRow* row: rowsVector) {
      // ...and append the _j_-th cell of each row to the _j_-th column.
      col->append(row->getCell(j));
    }
    j++;
  }
}

// (Re)load database from file
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
  // ...and decipher them. Of course the key should be the same used
  // for ciphering.
  out << crypto(in.str());
  std::string line;
  // Iterate through all lines in file.
  while(getline(out, line)) {
    // Instantiate new row to contain this line's data.
    CSVRow* row = new CSVRow();
    rowsVector.push_back(row);
    // Now working on each cell.
    // First, tokenize the line using TOKEN_SEPARATOR.
    for(std::string token: tokenize(line)) {
      // Count how many columns, need to do this only once...
      if(n == 0) {
        // ...also instantiate them.
        colsVector.push_back(new CSVCol());
        // They will be empty at this point.
        m++; // Keep track of how many columns are in this file.
      }
      // Instantiate cell and keep track of it...
      CSVCell* data = new CSVCell(token);
      // The cell is only kept track of by the rows vector, for now.
      row->append(data);
    }
    n++; // One more row parsed, starting over.
  }
  // Remember the empty columns we instantated before?
  // This is where they get set up...
  rebase();
  // Done.
  file.close();
}

/*  We are using the super-simple XOR cipher
 *  where each character is XOR-ed with a character from our key.
 *  As the message could be larger than the key, we take the
 *  message character index modulo-ed with the size of the key.
 *  It's like ciphering with a circular buffer of size key.size().
 *
 *  The best part about the XOR cipher is it's power 2 nilpotency.
 */

std::string CSVFile::crypto(std::string value) {
  for(int i = 0; i < value.size(); ++i) {
    value[i] ^= key[i % key.size()];
  }
  return value;
}

std::vector<std::string> CSVFile::tokenize(const std::string data) {
  std::vector<std::string> tokens;
  // TOKEN_SEPARATOR can only be 1 character long. Discard the rest.
  std::string separator(1, TOKEN_SEPARATOR);

  int current = 0, last = 0;
  // Split the string into tokens and exclude the TOKEN_SEPARATOR char.
  while((current = data.find(separator, last)) != std::string::npos) {
    tokens.push_back(data.substr(last, current - last));
    // Next index to start searching from.
    last = current + 1;
  }
  // Add the last token in the line (all the way to the end).
  tokens.push_back(data.substr(last, std::string::npos));
  return tokens;
}

// ==================== HEAP CLEANUP ==================== //

void CSVFile::clear() {
  for(CSVCol* column: colsVector) {
    // DOESN'T call CSVCell's destructor.
    // We let CSVRow's destructor do that, otherwise we'd have a NULLPTR.
    delete column;
  }

  // This will also delete all CSVCell in heap
  for(CSVRow* row: rowsVector) {
    // Calls CSVCell's destructor.
    delete row;
  }
}

// Unused but could be useful for inserting rows in an elegant fashion.
CSVFile& CSVFile::operator<<(CSVRow& row) {
  CSVRow* ptr = &row;
  return *(append(ptr));
}

// Goodbye.
CSVFile::~CSVFile() {
  clear();
}
