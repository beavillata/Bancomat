#include "csv_cell.h"

// Get content as std::string.
std::string CSVCell::sget() const {
  return data;
}

// Set content as string.
void CSVCell::set(std::string value) {
  data = value;
}

// Check that cell's content is the same as _target_.
bool CSVCell::is(std::string target) const {
  return (target == data);
}

// Check that cell's content starts with _target_...
bool CSVCell::starts(std::string target) const {
  int head = target.size();
  int content = data.size();

  // ...of course this can't be possible if _target_
  // is longer than the content.
  if(head > content) return false;

  return (data.substr(0, head) == target);
}

// Check that cell's content ends with _target_...
bool CSVCell::ends(std::string target) const {
  int tail = target.size();
  int content = data.size();

  // ...of course this can't be possible if _target_
  // is longer than the content.
  if(tail > content) return false;

  return (data.substr(content - tail, tail) == target);
}

// Insert cell's content into some stream that is passed to this function.
// Useful for printing stuff.
void CSVCell::stream(std::ostream& out) const {
  out << data;
}
