#include "csv_cell.h"

int CSVCell::iget() {
  int* ptr = static_cast<int*>(get());
  return *ptr;
}

double CSVCell::dget() {
  double* ptr = static_cast<double*>(get());
  return *ptr;
}

std::string CSVCell::sget() {
  std::string* ptr = static_cast<std::string*>(get());
  return *ptr;
}

void CSVCell::iset(int value) {
  set(&value);
}

void CSVCell::dset(double value) {
  set(&value);
}

void CSVCell::sset(std::string value) {
  set(&value);
}
