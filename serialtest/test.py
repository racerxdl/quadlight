#!/usr/bin/env python

import serial
import struct
import time

ser = serial.Serial("/dev/ttyUSB0", 115200, timeout=1)
time.sleep(6)
print "Sweeping Headlight"
i = 0
while i < 256:
    ser.write("\x03%s\x00\x00\x00\x00"%struct.pack("B",i))
    ser.flush()
    time.sleep(0.002)
    i+=1

i = 255
while i >= 0:
    ser.write("\x03%s\x00\x00\x00\x00"%struct.pack("B",i))
    ser.flush()
    time.sleep(0.002)
    i-=1    


print "Beeping and flashing landlight"
i = 0
while i < 6:
    ser.write("\x06%s\x00\x00\x00"%struct.pack("H", 1000));
    ser.flush()
    ser.write("\x04\x00\x00\x00\x00\x00");
    time.sleep(0.2)
    ser.write("\x07\x00\x00\x00\x00\x00");
    ser.flush()
    ser.write("\x05\x00\x00\x00\x00\x00");
    ser.flush()
    time.sleep(0.2)
    i+=1
    
ser.close()
