#include "LedManager.h"


void LedManager::InitLeds()  {
  pinMode(HEADLIGHT, OUTPUT);
  pinMode(LANDING, OUTPUT);
  
  LandingOff();
  UpdateHeadlight(0);
  
  pinMode(L1R, OUTPUT);
  pinMode(L1B, OUTPUT);
  pinMode(L1G, OUTPUT);
  
  pinMode(L2R, OUTPUT);
  pinMode(L2B, OUTPUT);
  pinMode(L2G, OUTPUT);
  
  pinMode(L3R, OUTPUT);
  pinMode(L3B, OUTPUT);
  pinMode(L3G, OUTPUT);
  
  pinMode(L4R, OUTPUT);
  pinMode(L4B, OUTPUT);
  pinMode(L4G, OUTPUT);
  
  UpdateLeds();
}

void LedManager::UpdateLeds()  {
  digitalWrite(L1R, leds[0] & (1 << 0));
  digitalWrite(L1B, leds[0] & (1 << 1));
  digitalWrite(L1G, leds[0] & (1 << 2));
  
  digitalWrite(L2R, leds[1] & (1 << 0));
  digitalWrite(L2B, leds[1] & (1 << 1));
  digitalWrite(L2G, leds[1] & (1 << 2));
  
  digitalWrite(L3R, leds[2] & (1 << 0));
  digitalWrite(L3B, leds[2] & (1 << 1));
  digitalWrite(L3G, leds[2] & (1 << 2));
  
  digitalWrite(L4R, leds[3] & (1 << 0));
  digitalWrite(L4B, leds[3] & (1 << 1));
  digitalWrite(L4G, leds[3] & (1 << 2));
}
