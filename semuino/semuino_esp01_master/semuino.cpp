/**
 * @file semuino.cpp
 * 
 * @brief Module de gestion du semuino
 */
#include "semuino.h"
#include "master.h"
#include "i2c_cmn.h"
#include "timer.h"

#include <RTClib.h>

//#define SET_TIME
#define SET_DATE_STR "01/03/2024"
#define SET_TIME_STR __TIME__

#define START_HOUR  (6)  ///< Heure de la montee en lumiere (au max au bout d'1h)
#define END_HOUR    (21)  ///< Heure de debut de la baisse de luminosite (OFF au bout d'1h) 

#define POWER_MAX         255
#define POWER_MIN         0
#define BOOST_POWER_MAX   255

#define ST_LED_P1   (0x01)
#define ST_LED_P2   (0x02)
#define ST_LED_P3   (0x04)

#define MAX_MODE  3         ///< Nombre de modes
DS1307 rtc;
bool g_blink_1s=false;
bool g_blink_2s=false;
int g_power=POWER_MAX;        ///< Puissance
bool g_boost=false;           ///< BOOST (au milieu de la journée)

#define MAGIC1  (0x55)
#define MAGIC2  (0xAA)
typedef struct
{
  unsigned char magic1;
  unsigned char mode;
  unsigned char states;
  unsigned char magic2;
} T_CONFIG;

bool g_mode_config=false;
T_CONFIG g_config=
{
  MAGIC1,
  0,
  0x07,
  MAGIC2
};

T_OUT _out;
T_IN _in;

Timer tmrTask(3000,false);

/**
 * @brief Sauvegarde la configuration en EEPROM
 * 
 * - mode
 * - etat des LEDs ouhaité lors du fonctionnement
*/
void _save_config(void)
{
  master_write_eep(0,g_config.magic1);
  master_write_eep(1,g_config.mode);
  master_write_eep(2,g_config.states);
  master_write_eep(3,g_config.magic2);  
}

/**
 * @brief Chargement de la configuration en EEPROM et reinit en cas d'incohérence
 * 
 * - mode
 * - etat des LEDs ouhaité lors du fonctionnement
*/
void _load_config(void)
{
  g_config.magic1=master_read_eep(0);
  g_config.mode=master_read_eep(1);
  g_config.states=master_read_eep(2);
  g_config.magic2=master_read_eep(3);

  if ( (g_config.magic1!=MAGIC1) || (g_config.magic2!=MAGIC2) || (g_config.mode>MAX_MODE) )
  {
    g_config.magic1=MAGIC1;
    g_config.mode=0;
    g_config.states=0;
    g_config.magic2=MAGIC2;
    
    _save_config();  
  }
}

/**
 * @brief Lecture de l'heure
 * @param[out] hour Heure
 * @param[out] minute Minute
 * @param[out] second Seconde
*/
void getTime(int &hour,int &minute, int &second)
{
  DateTime now = rtc.now();
  hour=now.hour();
  minute=now.minute();
  second=now.second();
}

/**
 * @brief Retourne la puissance des LEDs en fonction de l'heure
 * @param[in] h Heure
 * @param[in] m Minute
 * @return -1 signifie OFF et 255 à fond
*/
int getPower(int h,int m)
{
  if (h==START_HOUR)
  {    
    //int res=m*255/59;
    int res=map(m,0,59,POWER_MIN,POWER_MAX);
    return (res&0xFF);
  }
  else if (h==END_HOUR)
  {
    //int res=(59-m)*255/60;
    int res=map(59-m,0,59,POWER_MIN,POWER_MAX);
    return (res&0xFF);
  }
  else if ( (h>START_HOUR) && (h<END_HOUR) )
  {
    return POWER_MAX;
  }
  else
  {
    return -1;
  }
}

/**
 * @brief Gestion du mode intelligent (en fonction de l'heure pour simuler aube et crépuscule)
*/
void manageMode_smart(void)
{
  if ( g_power==-1 )
  {    
    /// @remark Si power -1, tout est coupé (c'est la nuit)
    _out.ctrl=0;
  }
  else
  {
    _out.ctrl=0;
    /// @remark Sinon on alimente le RGB 5V
    _out.ctrl|=CTRL_5V;
    
    if (g_power==POWER_MAX)
    {
      /// @remark Au maximum, on allume systematiquement les LEDs 12V choisies
      if ((g_config.states&ST_LED_P1)==ST_LED_P1)
        _out.ctrl|=CTRL_P1;
      if ((g_config.states&ST_LED_P2)==ST_LED_P2)
        _out.ctrl|=CTRL_P2;
      if ((g_config.states&ST_LED_P3)==ST_LED_P3)
        _out.ctrl|=CTRL_P3;
        
      /// @remark, lors du boost, on met en blanc les deux étages RGBs choisis et à la puissance max du boost sinon en rouge horticole à la puissance actuelle
      if ((g_config.states&ST_LED_P1)==ST_LED_P1)
      {
        _out.ctrl|=CTRL_RGB1;
        _out.modeRGB1=g_boost==true?RGB_MOD_WHITE:RGB_MOD_GROW;
      }        
                 
      if ((g_config.states&ST_LED_P2)==ST_LED_P2)
      {
        _out.ctrl|=CTRL_RGB2;
        _out.modeRGB2=g_boost==true?RGB_MOD_WHITE:RGB_MOD_GROW;
      }        

      _out.level=g_boost==true?BOOST_POWER_MAX:g_power;
    }
    else
    {
      /// @remark Lors des montées et descente, on éteint les 12V et on applique la luminosité des RGBs souhaitee
      _out.ctrl=0;
      _out.ctrl|=CTRL_5V;

      if ((g_config.states&ST_LED_P1)==ST_LED_P1)      
      {
        _out.ctrl|=CTRL_RGB1;
        _out.modeRGB1=RGB_MOD_WHITE;
      }        
        
      if ((g_config.states&ST_LED_P2)==ST_LED_P2)
      {
        _out.ctrl|=CTRL_RGB2;
        _out.modeRGB2=RGB_MOD_WHITE;
      }        

      _out.level=g_power;
    }
  }
}

/**
 * @brief Tache de config
*/
void taskConfig(void)
{  
  _out.ctrl=0;
  if ((g_config.states&ST_LED_P1)==ST_LED_P1)
    _out.ctrl|=CTRL_P1;
  if ((g_config.states&ST_LED_P2)==ST_LED_P2)
    _out.ctrl|=CTRL_P2;
  if ((g_config.states&ST_LED_P3)==ST_LED_P3)
    _out.ctrl|=CTRL_P3;  
}

/**
 * @brief Tache du SEMUINO
*/
void taskSemuino(void)
{
  int h,m,s;

  getTime(h,m,s);
  g_power=getPower(h,m);

  // Periode de BOOST... (LEDs RGB blanches au lieu de Rouge horticole)
  if ( (h>=12) && (h<=13) )
    g_boost=true;
  else
    g_boost=false;

  Serial.print("Time: ");
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print("; mode: ");
  Serial.print(g_config.mode);
  Serial.print("; power: ");
  Serial.println(g_power);
  
  switch (g_config.mode)
  {
    case 0:   /// All OFF
    default:
    {
      g_config.mode=0;
      
      _out.ctrl=0;
      _out.level=0;
         
      break;
    }
    case 1: ///< Smart mode
    {
      manageMode_smart();
      
      break;
    }
    case 2: /// Manual mode
    {
      _out.level=0;
      
      _out.ctrl=0;
      if ((g_config.states&ST_LED_P1)==ST_LED_P1)
        _out.ctrl|=CTRL_P1;
      if ((g_config.states&ST_LED_P2)==ST_LED_P2)
        _out.ctrl|=CTRL_P2;
      if ((g_config.states&ST_LED_P3)==ST_LED_P3)
        _out.ctrl|=CTRL_P3;
      
      break;
    }
  }
}

/**
 * @brief Detection de la pression sur l'interrupteur de selection du mode
 * @return true si presse
*/
bool inputInterrupteur()
{
  if ((_in.inputs&INP_SELECTOR)==INP_SELECTOR)
  {
    while (1)
    {
      delay(200);
      master_loop();
      master_get_in_values(&_in);

      if ((_in.inputs&INP_SEL_LONG)==INP_SEL_LONG)
      {
        Serial.println("Config mode");
        if (g_mode_config==false)
        {
          for (int i=0;i<10;i++)
          {
            _out.leds|=OUP_GREEN;
            master_set_out_values(&_out);
            master_loop();
            delay(100);
            _out.leds&=~OUP_GREEN;
            master_set_out_values(&_out);
            master_loop();
            delay(100);
          }
          g_mode_config=true;
        }
        else
        {
          _save_config();
          g_mode_config=false;
          
          Serial.println("Exit Config");

          _out.leds&=~OUP_GREEN;
          while ((_in.inputs&INP_SELECTOR)==INP_SELECTOR)
          {
            delay(200);
            master_set_out_values(&_out);
            master_loop();
            master_get_in_values(&_in);
          }
        }
        
        break;
      }
      else if ((_in.inputs&INP_SELECTOR)==0)
      {
        if (g_mode_config==false)
        {
          Serial.println("Ch. mode");
          g_config.mode=g_config.mode+1;
          if (g_config.mode>MAX_MODE)
            g_config.mode=0;
  
          //g_config.states=1;
          _save_config();
        }
        else
        {
          Serial.println("Ch. config");
          g_config.states=g_config.states+1;
          if (g_config.states>0x07)
            g_config.states=0;          
        }
        
        break;
      }
    }
  }
    
  return false;
}

void semuino_init(void)
{
  g_power=POWER_MAX;
  g_boost=false;
  
  _load_config();
  
  _out.ctrl=0;
  _out.level=0;
  _out.modeRGB1=0;
  _out.modeRGB2=0;
  _out.modeRGB3=0;  
  _out.leds=0;

  master_set_out_values(&_out);

  rtc.begin();
  
#ifdef SET_TIME
  Serial.print("Set datetime ");
  Serial.print(SET_DATE_STR);
  Serial.print(" ");
  Serial.println(SET_TIME_STR);

  int year, month, day, hour, minute, second;
  sscanf(SET_DATE_STR, "%02d/%02d/%04d", &day, &month, &year);
  sscanf(SET_TIME_STR, "%02d:%02d:%02d", &hour, &minute, &second);
  DateTime newDte(year, month, day, hour, minute, second);
  rtc.adjust(newDte);
#endif

  char tmp[22];
  DateTime now = rtc.now();
  sprintf(tmp, "%02d/%02d/%04d %02d:%02d:%02d", now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.println(tmp);

  delay(500);

  /// Si RTC non reglee, on bloque en defaut au demarrage !
  if (now.year()<2024)
  {
    Serial.println("Date incorrecte !!!");
    while (1)
    {
      /// @todo Afficher Date incorrecte !
      //digitalWrite(PIN_LED_GREEN,HIGH);
      delay(100);
      //digitalWrite(PIN_LED_GREEN,LOW);
      //delay(100);
    }
  }

  Serial.print("Mode: ");
  Serial.println(g_config.mode);
  Serial.print("States: ");
  Serial.println(g_config.states);

  tmrTask.start();  
}

int cycles=0;

void semuino_loop(void)
{
  if (tmrTask.tick()==true)
  {
    if (g_mode_config==false)
    {
      taskSemuino();
      
      g_blink_1s=!g_blink_1s;    
      if (g_blink_1s==true)
        g_blink_2s=!g_blink_2s;      
    }
    else
    {
      taskConfig();      
    }
  }

  inputInterrupteur();

  master_set_out_values(&_out);
  if (cycles++>100)
  {
    master_loop(); 
    cycles=0;
  }
  master_get_in_values(&_in);  
}
