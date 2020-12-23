#ifndef CSVCELL_H
#define CSVCELL_H

#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>

class CSVCell {
public:
  CSVCell(std::string content): data(content) {};
  CSVCell(double content) {
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed << content;
    data = ss.str();
  };
  CSVCell(int content) { data = std::to_string(content); };

  std::string sget() const;
  double dget() const { return stod(sget()); };
  int iget() const { return stoi(sget()); };

  void set(std::string);
  void set(double value) {
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed << value;
    data = ss.str();
  };
  void set(int value) { data = std::to_string(value); };

  bool is(std::string) const;
  bool starts(std::string) const;
  bool ends(std::string) const;

  void stream(std::ostream&) const;

private:
  std::string data;
};

#endif
