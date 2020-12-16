#ifndef USER_H
#define USER_H

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
  int numerocarta[13];
  int PIN[5];
};

#endif
