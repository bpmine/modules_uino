#ifndef DATA_BUILDER_HEADER_INCLUDED
#define DATA_BUILDER_HEADER_INCLUDED

class Data;
class Oya;
class Pump;
class Slave;
class DateTime;
class DataBuilder
{
  private:
    Data *pData;

    void set(Slave *slave);

  public:
    DataBuilder(Data *pData,bool raz=false);

    void set(Pump *pump);
    void set(Oya *oya);
    void set(int day,int month,int year,int h,int m,int s);
    void set(DateTime *pDt);
};

#endif
