#pragma once

class IComunication{
public:
  virtual void sendData(int* jsonData) = 0; //int is just a way to compile for now
  //please change that INT!!!!
  virtual int* receiveData() = 0; //change int* to json data
};
