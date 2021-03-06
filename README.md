Quadcopter Light System
=========

This is a project for a lightcontrol for Multirotors.
For now, its designed for Quadcopters (4 arms), but can be easily adapted for more rotors (or less).


So basicly it does for now:

- Landing Light Control (ON/OFF)
- RGB LED Light Control for each arm
- Intensity controlled Heading Light
- Frequency Controlled Buzzer Sound
- Serial Communication for controlling. 
- I2C Communication to make easier to interact with Multiwii Boards with 328p (Now working with i2c master test and modified multiwii board)
- Sound Manager for Buzzer. So you can preset a few tone sequences and just enable it from serial
- Three songs on song manager: The Start Song, Battery Low, Battery Critical.

TODO:
- Configurable for other multirotors type
- More songs for SongManager
- Fix SongManager problem that happens with multiwii modified code.

Current Protocol (v1.0):
- Each message through serial is fixed at 3 bytes. The first byte is CMD, the others are data.
- The data is specific for each CMD. The available CMDs are:

**QLP_INFO** *(0x00)*:
-	Data: Nothing
-	Response:	(QLP_INFO,QUADLIGHT_VERSION,QUADLIGHT_PROTOCOL)
-	Description: Anwser the master which version and protocol the board is.

**QLP_UPDATEARM** *(0x01)*:
-	Data: (ARM,LEDS,0x00)
-	Response: Nothing
-	Description: Updates the arm ARM with LEDS data.

**QLP_UPDATELED** *(0x02)*:
-	Data: (ARM,LED,VAL)
-	Response: Nothing
-	Description: Updates the LED in ARM with VAL
	
**QLP_HEADLIGHT** *(0x03)*
-	Data: (INTENSITY,0x00,0x00)
-	Response: Nothing
-	Description: Sets the intensity of headlight
	
**QLP_LANDING_ON** *(0x04)*
-	Data: Nothing,
-	Response: Nothing
-	Description: Turn on the Landing Light

**QLP_LANDING_OFF** *(0x05)*
-	Data: Nothing,
-	Response: Nothing
-	Description: Turn off the Landing Light
	
**QLP_SOUND** *(0x06)*
-	Data: (FREQ_LSB,FREQ_MSB,0x00)
-	Response: Nothing
-	Description: Turn on buzzer at FREQ

**QLP_NOSOUND** *(0x07)*
-	Data: Nothing
-	Response: Nothing
-	Description: Turn off the buzzer

**QLP_PLAYSOUND** *(0x08)*
-   Data: (SONG_NUMBER,LOOP,0x00)
-   Response: Nothing
-   Description: Play the song SONG_NUMBER (in loop if LOOP > 0)
