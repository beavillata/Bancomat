#ifndef CSVCELL_H
#define CSVCELL_H

#include <iostream>
#include <string>

class CSVCell {
public:
  CSVCell(const int x, const int y):
    i(x), j(y) {};
  virtual void print() const {};
  virtual void value(const void* ptr) const {};

  int row();
  int col();

private:
  int i, j;
};

template <typename T>
class CSVData: public CSVCell {
public:
  CSVData(const int x, const int y, const T value):
    CSVCell(x, y), data(value) {};
  void print() const override {
    std::cout << data;
  };
  void value(const void* ptr) const override {
    ptr = &data;
  };

private:
  const T data;
};

#endif
