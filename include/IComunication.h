
class IComunication{
private:
  IComunication();
public:
  IComunication(){
    //built the infrastucture for com like wifi.begin();
  }
  virtual void sendData(int* jsonData) = 0; //int is just a way to compile for now
  //please change that INT!!!!
  virtual int* receiveData() = 0;
};
