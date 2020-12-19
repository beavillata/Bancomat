#ifndef DATABASE_H
#define DATABASE_H

#include <memory>

#include "csv/csv_file.h"

class Database {

public:
  static inline std::unique_ptr<CSVFile>
    credentials{new CSVFile("persistent/credentials.csv")};
  static inline std::unique_ptr<CSVFile>
    accounts{new CSVFile("persistent/accounts.csv")};
};

#endif
