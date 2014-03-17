#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define NOTE_0  524
#define NOTE_1  554
#define NOTE_2  588
#define NOTE_3  622
#define NOTE_4  660
#define NOTE_5  698
#define NOTE_6  740
#define NOTE_7  794
#define NOTE_8  830
#define NOTE_9  880
#define TIME_UNIT 200  //  ms

#define TONE_START      1
#define TONE_BATT_LOW   2
#define TONE_BATT_CRIT  3

class SoundManager  {
  private:
    uint16_t count;
    uint16_t nextupdate;
    uint8_t curr_sequence;
    uint8_t speakerpin;
    boolean loopsequence;
    boolean PlayingTone;
  public:
    SoundManager(uint8_t spkr)                          { this->speakerpin = spkr; pinMode(spkr, OUTPUT);  PlayingTone = false; };
    void StartSequence(uint8_t sequence, boolean loop);
    inline void SetTone(uint16_t frequency)             { tone(this->speakerpin,frequency); };
    inline void StopSequence()                          { curr_sequence = 0; count = 0; };
    inline void ClearTone()                             { noTone(this->speakerpin); };
    inline void PlayTone(uint16_t freq, uint16_t time)  { SetTone(freq); nextupdate = millis()+time*TIME_UNIT; };
    inline void ProcessLoop()                           { if(!loopsequence) { curr_sequence = 0; PlayingTone = false;} };
    void Update();
    inline boolean Playing()                            { return PlayingTone; };
};
