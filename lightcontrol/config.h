#ifndef  QUADLIGHT_CONFIG
#define QUADLIGHT_CONFIG
// Lights config
// The arm lights is made to has three colors
// R for Red
// G for Green
// B for Blue
// You can use both RGB Leds and three separeted leds.
// This config use arduino number pins

// Arm 1 lights.
#define L1R 8
#define L1B 9
#define L1G 10

// Arm 2 lights.
#define L2R 11
#define L2B 12
#define L2G 13

// Arm 3 lights.
#define L3R A0
#define L3B A1
#define L3G A2

// Arm 4 lights.
#define L4R 4
#define L4B 6
#define L4G 7

#define SPEAKER 3         //  The Speaker output. Must be a PWM Output
#define LANDING 2         //  The Landing Light Output
#define HEADLIGHT 5       //  The Headlight output. 
#define HEADLIGHT_PWM     //  If the headlight output is a PWM one, you can control the brightness
#define HEADLIGHT_MID 127 //  Used only if HEADLIGHT_PWM isnt defined.
                          //  This is used on non Headlight PWM mode for setting the mid point.
                          /// Below the midpoint, the light is off, over is on.
#define I2C_ADDRESS 0x52

// Notes for Start Music
#define NOTE_0  524
#define NOTE_1 554
#define NOTE_2  588
#define NOTE_3 622
#define NOTE_4  660
#define NOTE_5  698
#define NOTE_6 740
#define NOTE_7  794
#define NOTE_8 830
#define NOTE_9  880
#define TIME_UNIT 200  //  ms

// Version definition

#define QUADLIGHT_VERSION 0x01
#define QUADLIGHT_PROTOCOL 0x00


// Protocol

#define QLP_INFO 0
#define QLP_UPDATEARM 1
#define QLP_UPDATELED 2
#define QLP_HEADLIGHT 3
#define QLP_LANDING_ON 4
#define QLP_LANDING_OFF 5
#define QLP_SOUND 6
#define QLP_NOSOUND 7
#define QLP_PLAYSONG 8
#define QLP_UNKNOWN 255

#endif
