#ifndef LOGIN_H
#define LOGIN_H

class user{
public:
  user(){};
  user(int[],int[]);

  void setNC(int[]);
  void setPIN(int[]);

  int* getNC();
  int* getPIN();

  //void stampa();

private:
  int numerocarta[4];
  int PIN[4];
};

#endif
