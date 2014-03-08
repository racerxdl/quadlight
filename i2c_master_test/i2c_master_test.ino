#include <Wire.h>

/*
    A Light Control for Multirotors
    Copyright (C) 2014  Lucas Teske

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// This will turn on and off the landing light in 1sec intervals

void setup()
{
  Wire.begin();
  delay(6000);
}

byte x = 0;

void loop()
{
  Wire.beginTransmission(6);
  Wire.write("\x04"); 
  Wire.endTransmission();
  delay(1000);
  Wire.beginTransmission(6);
  Wire.write("\x05");
  Wire.endTransmission();
  delay(1000);
}
