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

    std::vector<CSVRow*> rows();
    std::vector<CSVCol*> cols();

    CSVRow* row(const int);
    CSVCol* col(const int);
    CSVCell* cell(const int, const int);

    void append(CSVRow*);
    void insert(CSVRow*, const int);
    void remove(int index);

    void save();
    void close();
    void print();

  private:
    std::string path;

    int n, m;

    std::vector<std::string> tokenize(const std::string);
    std::vector<CSVCol*> colsVector;
    std::vector<CSVRow*> rowsVector;
};

#endif
