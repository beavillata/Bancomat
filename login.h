#ifndef LOGIN_H
#define LOGIN_H

#include "user.h"

class Login {
public:
  static bool login(std::string, std::string);
  static bool logged();
  static void logout();

  static User* user();

private:
  static inline User* current = NULL;
};

#endif
