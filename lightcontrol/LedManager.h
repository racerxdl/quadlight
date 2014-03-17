#include "./config.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class LedManager  {
  private:
    char leds[4] = {0,0,0,0}; 
  public:
    LedManager()  {};
    void InitLeds();
    void UpdateLeds();
    inline void UpdateHeadlight(uint8_t value)  {
      #ifdef HEADLIGHT_PWM
       analogWrite(HEADLIGHT, value);
      #else
        if(value >= HEADLIGHT_MID)
          digitalWrite(HEADLIGHT, HIGH);
        else
          digitalWrite(HEADLIGHT,LOW);
      #endif 
    }
    inline void UpdateLed(uint8_t arm, uint8_t led, uint8_t val) { if(arm >= 0 && arm < 4) leds[arm] = (leds[arm] & ~(1<<led)) | ((!!val) << led); };
    inline void UpdateARMLED(uint8_t arm, uint8_t data) { if(arm >= 0 && arm < 4)  leds[arm] = data; };
    inline void LandingOn()  {  digitalWrite(LANDING, HIGH); }
    inline void LandingOff() {  digitalWrite(LANDING, LOW);  }
    inline void ClearLeds()  {  for(int i=0;i<4;i++) leds[i] = 0; UpdateLeds(); };
    inline void ShiftLedLeft(uint8_t arm)  { if(arm >= 0 && arm < 4) leds[arm] <<= 1;  }
    inline void ShiftLedRight(uint8_t arm) { if(arm >= 0 && arm < 4) leds[arm] >>= 1;  }
};
