#ifndef CSVFILE_H
#define CSVFILE_H

#include <string>
#include <vector>

#include "csv_dim.h"

class CSVFile {
  public:
    CSVFile(const std::string file, const std::string cipher)
      : path(file), key(cipher) { reload(); };
    ~CSVFile();

    // Unused, for now
    CSVFile& operator<<(CSVRow&);

    void reload();

    std::vector<CSVRow*> getRows() const;
    std::vector<CSVCol*> getCols() const;

    CSVRow* getRow(const int) const;
    CSVCol* getCol(const int) const;
    CSVCell* getCell(const int, const int) const;

    CSVFile* append(CSVRow*);
    CSVFile* insert(CSVRow*, const int);
    CSVFile* remove(const int);

    void rebase();
    void clear();
    void save();
    std::string crypto(std::string);

  private:
    static inline const char TOKEN_SEPARATOR = ',',
      COMMENT_HEAD = '#';

    std::string path, key;
    int n, m;

    std::vector<std::string> tokenize(const std::string);
    std::vector<CSVCol*> colsVector;
    std::vector<CSVRow*> rowsVector;
};

#endif
