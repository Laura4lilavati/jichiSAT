
struct ISensorData{
  virtual void setData() = 0;
  virtual ISensorData* getData() = 0;
};

class ISensor{
private:
  ISensorData* m_sd;
  ISensor(); //avoid use default constructor
public:
  ISensor(ISensorData* sd) : m_sd(sd){} // initialize sensor parameters such pines to use, etc.
  virtual ISensorData* getData() = 0; //pure virtual function
};
