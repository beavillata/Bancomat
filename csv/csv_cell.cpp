#include "csv_cell.h"

std::string CSVCell::sget() const {
  return data;
}

void CSVCell::set(std::string value) {
  data = value;
}

bool CSVCell::is(std::string target) const {
  return (target == data);
}

bool CSVCell::starts(std::string target) const {
  int head = target.size();
  int content = data.size();

  if(head > content) return false;

  return (data.substr(0, head) == target);
}

bool CSVCell::ends(std::string target) const {
  int tail = target.size();
  int content = data.size();

  if(tail > content) return false;

  return (data.substr(content - tail, tail) == target);
}

void CSVCell::stream(std::ostream& out) const {
  out << data;
}
