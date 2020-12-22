#ifndef CSVCELL_H
#define CSVCELL_H

#include <iostream>
#include <string>

class CSVCell {
public:
  CSVCell(std::string content): data(content) {};
  CSVCell(double content) { data = std::to_string(content); };
  CSVCell(int content) { data = std::to_string(content); };

  std::string sget() const;
  double dget() const { return stod(sget()); };
  int iget() const { return stoi(sget()); };

  void set(std::string);
  void set(double value) { data = std::to_string(value); };
  void set(int value) { data = std::to_string(value); };

  bool is(std::string) const;
  bool starts(std::string) const;
  bool ends(std::string) const;

  void stream(std::ostream&) const;

private:
  std::string data;
};

#endif
