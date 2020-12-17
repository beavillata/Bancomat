#ifndef USER_H
#define USER_H

class user{
public:
  user(){};
  user(int[],int[],double);

  void setNC(int[]);
  void setPIN(int[]);
  void setSALDO(double);

  int* getNC();
  int* getPIN();
  double getSALDO();

  //void stampa();

private:
  int *numerocarta[13];
  int *PIN[5];
  double SALDO;
};

#endif
