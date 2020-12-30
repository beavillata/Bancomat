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
    // We don't want to have more than 2 decimal points in our .dat files.
    // This cleans up the double passed.
    ss << std::setprecision(2) << std::fixed << content;
    data = ss.str();
  };
  CSVCell(int content) { data = std::to_string(content); };

  // Unfortunately we don't have return type overload in C++...
  // We do it the OpenGL way.
  std::string sget() const;
  double dget() const { return stod(sget()); };
  int iget() const { return stoi(sget()); };

  // set() can be used with std::string, int and double.
  void set(std::string);
  void set(double value) {
    std::stringstream ss;
    // We don't want to have more than 2 decimal points in our .dat files.
    // This cleans up the double passed.
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
