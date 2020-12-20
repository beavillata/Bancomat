#ifndef CSVCELL_H
#define CSVCELL_H

#include <iostream>
#include <string>

class CSVCell {
public:

  virtual void stream(std::ostream&) const {};
  virtual void* get() { return NULL; };
  virtual void set(void*) {};
  virtual bool is(void*) const { return false; };

  int iget();
  double dget();
  std::string sget();

  void iset(int);
  void dset(double);
  void sset(std::string);
};

template <typename T>
class CSVData: public CSVCell {
public:
  CSVData(const T value): data(value) {};
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
  bool is(void* ptr) const override {
    return (data == *(static_cast<T*>(ptr)));
  };

private:
  T data;
};

#endif
