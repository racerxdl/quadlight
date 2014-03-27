#include "SoundManager.h"

void SoundManager::StartSequence(uint8_t sequence, boolean loop)  {
  if(curr_sequence != sequence)  {
    count = 0;
    loopsequence = loop;
    curr_sequence = sequence;
    nextupdate = millis();
    PlayingTone = true;
  }
}

void SoundManager::Update()  {
  if(nextupdate <= millis())  {
    switch(curr_sequence)  {
       case TONE_START:    //  Start Music
         switch(count)  {
           case 0: PlayTone(NOTE_2,1);  count++;  break;
           case 1: PlayTone(NOTE_4,2);  count++;  break;
           case 2: PlayTone(NOTE_5,2);  count++;  break;
           case 3: PlayTone(NOTE_7,1);  count++;  break;
           case 4: PlayTone(NOTE_5,2);  count++;  break;
           case 5: PlayTone(NOTE_4,2);  count++;  break;
           case 6: PlayTone(NOTE_2,2);  count++;  break;
           case 7: PlayTone(NOTE_4,1);  count++;  break;
           case 8: ClearTone(); ProcessLoop(); count = 0;      break;
         }
       break;
       case TONE_BATT_LOW:
         switch(count)  {
           case 0:  PlayTone(800,5); count++; break;
           case 1:  ClearTone(); nextupdate = millis()+5*TIME_UNIT; count ++; break;
           case 2: ClearTone(); ProcessLoop(); count = 0;      break;
         }
       break; 
       case TONE_BATT_CRIT:
         switch(count)  {
           case 0:  PlayTone(800,2); count++; break;
           case 1:  ClearTone(); nextupdate = millis()+1*TIME_UNIT; count ++; break;
           case 2:  ClearTone(); ProcessLoop(); count = 0;      break;
         }
       break; 
       case TONE_UNSTABLE:
         switch(count)  {
            case 0:  PlayTone(NOTE_4,5); count++; break;
            case 1:  PlayTone(NOTE_9,5); count++; break;
            case 2:  PlayTone(NOTE_4,5); count++; break;
            case 3:  PlayTone(NOTE_9,5); count++; break; 
            case 4:  ClearTone(); ProcessLoop(); count = 0;      break;
         }
    }
  }
}
