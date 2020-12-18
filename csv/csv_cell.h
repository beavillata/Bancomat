#ifndef CSVCELL_H
#define CSVCELL_H

#include <iostream>
#include <string>

class CSVCell {
public:
  CSVCell(const int x, const int y):
    i(x), j(y) {};
  virtual void stream(std::ostream& out) const {};
  virtual void* get() { return NULL; };
  virtual void set(void* ptr) {};
  virtual bool is(void* ptr) {return false; };

  int iget();
  double dget();
  std::string sget();

  void iset(int value);
  void dset(double value);
  void sset(std::string value);

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
  void stream(std::ostream& out) const override {
    out << data;
  };
  void* get() override {
    void* ptr = &data;
    return ptr;
  };
  void set(void* ptr) override {
    data = *(static_cast<T*>(ptr));
  };
  bool is(void* ptr) override {
    return (data == *(static_cast<T*>(ptr)));
  };

private:
  T data;
};

#endif
