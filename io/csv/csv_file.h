#ifndef CSVFILE_H
#define CSVFILE_H

#include "csv_dim.h"
#include <string>
#include <vector>

class CSVFile {
  public:
    CSVFile(const std::string file): path(file) {
      reload();
    };
    ~CSVFile();

    void reload();

    std::vector<CSVRow*> rows();
    std::vector<CSVCol*> cols();

    CSVRow* row(int row);
    CSVCol* col(int col);

    void append(CSVRow* row);
    void insert(CSVRow* row, int after);
    void remove(int index);

    void save();
    void close();
    void print();

  private:
    std::string path;

    int n, m;

    std::vector<std::string> tokenize(const std::string data);
    std::vector<CSVCol*> colsVector;
    std::vector<CSVRow*> rowsVector;
};

#endif
