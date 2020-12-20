#ifndef CSVFILE_H
#define CSVFILE_H

#include <string>
#include <vector>

#include "csv_dim.h"

class CSVFile {
  public:
    CSVFile(const std::string file): path(file) {
      reload();
    };
    ~CSVFile();

    void reload();

    std::vector<CSVRow*> getRows() const;
    std::vector<CSVCol*> getCols() const;

    CSVRow* getRow(const int) const;
    CSVCol* getCol(const int) const;
    CSVCell* getCell(const int, const int) const;

    CSVFile* append(const CSVRow*);
    CSVFile* insert(const CSVRow*, const int);
    CSVFile* remove(const int);

    void rebase();
    void clear();
    void save();
    void close();
    void print();

  private:
    std::string path;

    int n, m;

    std::vector<std::string> tokenize(const std::string);
    void clearCols();
    void clearRows();
    std::vector<std::vector<CSVCell*>> cellsVector;
    std::vector<CSVCol*> colsVector;
    std::vector<CSVRow*> rowsVector;
};

#endif
