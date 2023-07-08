#ifndef BTN_HEADER_INCLUDED
#define BTN_HEADER_INCLUDED

#include <arduino.h>

#define FILTER_BTN_SHORT_US		(2000UL)
#define FILTER_BTN_LONG_US		(4000000UL)

class Btn
{
  private:
    unsigned long t0;
    bool rawState;
    bool inv;
    int pin;
    bool filteredState;
    bool longState;
    bool risingEdge;
    bool fallingEdge;

  public:
    Btn()
    {
      this->pin=-1;
      this->inv=false;
      t0=micros();
      rawState=false;
      filteredState=false;
      longState=false;
      risingEdge=false;
      fallingEdge=false;
    }

    void begin(int pin,int mode,bool inv=false)
    {
      this->pin=pin;
      pinMode(pin,mode);

      this->inv=inv;
      rawState=inv;
      filteredState=inv;
      longState=false;
      risingEdge=false;
      fallingEdge=false;

      t0=micros();
    }

    void reset(void)
    {
      longState=false;
      risingEdge=false;
      fallingEdge=false;
      t0=micros();
    }

    void loop(void)
    {
      if (pin<0)
    	return;

      bool newState=digitalRead(pin)==HIGH?true:false;
      if (inv==true)
    	newState=!newState;

      unsigned long t=micros();
      unsigned long delta;
      if (t>=t0)
    	delta=t-t0;
      else
    	delta=0xFFFFFFFF-t0+t;

      if (newState==rawState)
      {
    	if (rawState==true)
    	{
    	  if (delta>FILTER_BTN_SHORT_US)
    	  {
    		if (filteredState!=rawState)
    		  risingEdge=true;

    		filteredState=rawState;
    	  }

    	  if (delta>FILTER_BTN_LONG_US)
    		longState=true;
    	}
      }
      else
      {
    	rawState=newState;

    	if (rawState==false)
    	{
    	  if (filteredState==true)
    		fallingEdge=true;

    	  filteredState=false;
    	}

    	longState=false;
    	risingEdge=false;
    	fallingEdge=false;

    	t0=micros();
      }
    }

    bool isPressed()
    {
      return filteredState;
    }

    bool isRising(void)
    {
      bool ret=risingEdge;
      risingEdge=false;
      return ret;
    }

    bool isFalling(void)
    {
      bool ret=fallingEdge;
      fallingEdge=false;
      return ret;
    }

    bool isLongPressed(void)
    {
      return longState;
    }
};

#endif
