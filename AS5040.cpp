/*
 * AS5040.cpp - Arduino library for AMS AS5040 magnetic rotary encoder chip
 * version 1.0 2014-03-05
 *
 * Copyright (c) 2014 Mark Tillotson.  All rights reserved.
 *
 * This file is part of "Mark's AS5040 library for Arduino"
 *
 *  "Mark's AS5040 library for Arduino" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Mark's AS5040 library for Arduino" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with "Mark's AS5040 library for Arduino".
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "AS5040.h"


AS5040::AS5040 (byte pinCLK, byte pinCS, byte pinDO, byte pinPROG) 
{
  _pinCLK  = pinCLK ;
  _pinCS   = pinCS ;
  _pinDO   = pinDO ;
  _pinPROG = pinPROG ;

  _status  = 0xFF ;  // invalid status
}

boolean AS5040::begin ()
{
  return begin (0) ;
}

boolean AS5040::begin (byte mode)
{
  return begin (mode, false, 0) ;
}

boolean AS5040::begin (byte mode, boolean reverse, unsigned int offset)
{
  int config_word =
    (reverse ? 0x8000 : 0x0000) | 
    ((offset & 0x3FF) << 5) |
    (mode & 0x1F) ;

  pinMode (_pinCLK, OUTPUT) ;  digitalWrite (_pinCLK, HIGH) ;
  if (_pinPROG != 0xFF)
  {
    pinMode (_pinPROG, OUTPUT) ; digitalWrite (_pinPROG, LOW) ;
  }
  pinMode (_pinCS, OUTPUT) ;   digitalWrite (_pinCS, HIGH) ;
  pinMode (_pinDO, INPUT_PULLUP) ;

  byte count = 0 ;
  while (read (), (_status & AS5040_STATUS_OCF) == 0)
  {
    if (count > 30)
      return false ; // failed to initialize
    delay (1) ;
    count ++ ;
  }

  if (_pinPROG == 0xFF)
  {
    // no initial program sequence, we're done already
    return true ;
  }
  digitalWrite (_pinCS, LOW) ;
  delayMicroseconds (1) ;
  digitalWrite (_pinCLK, LOW) ;
  delayMicroseconds (1) ;
  digitalWrite (_pinPROG, HIGH) ;
  delayMicroseconds (5) ;
  digitalWrite (_pinCS, HIGH) ;
  delayMicroseconds (5) ;

  unsigned int MSK = 0x8000 ;
  for (byte i = 0 ; i < 16 ; i++)
  {
    digitalWrite (_pinPROG, (config_word & MSK) ? HIGH : LOW) ;
    MSK >>= 1 ;
    digitalWrite (_pinCLK, HIGH) ;
    delayMicroseconds (1) ;
    digitalWrite (_pinCLK, LOW) ;
    delayMicroseconds (1) ;
  }
  
  digitalWrite (_pinPROG, LOW) ;
  delayMicroseconds (1) ;
  digitalWrite (_pinCS, LOW) ;
  delayMicroseconds (1) ;
  digitalWrite (_pinCS, HIGH) ; // ready for reads
  delayMicroseconds (1) ;
  digitalWrite (_pinCLK, HIGH) ;
  return true ;
}


// read position value, squirrel away status
unsigned int AS5040::read ()
{
  digitalWrite (_pinCS, LOW) ;
  unsigned int value = 0 ;
  for (byte i = 0 ; i < 10 ; i++)
  {
    digitalWrite (_pinCLK, LOW) ;
    digitalWrite (_pinCLK, HIGH) ;
    value = (value << 1) | digitalRead (_pinDO) ;
  }
  byte status = 0 ;
  for (byte i = 0 ; i < 6 ; i++)
  {
    digitalWrite (_pinCLK, LOW) ;
    digitalWrite (_pinCLK, HIGH) ;
    status = (status << 1) | digitalRead (_pinDO) ;
  }
  digitalWrite (_pinCS, HIGH) ;
  _parity = even_parity (value >> 2) ^ even_parity (value & 3) ^ even_parity (status) ;
  _status = status >> 1 ;
  return value ;
}

byte AS5040::even_parity (byte val)
{
  val = (val >> 1) ^ val ;
  val = (val >> 2) ^ val ;
  val = (val >> 4) ^ val ;
  return val & 1 ;
}


// raw status from latest read, 5 bit value
byte AS5040::status ()
{
  return _status ;
}

// indicate if latest status implies valid data
boolean AS5040::valid ()
{
  return _parity == 0 && (_status & 0x18) == 0x10 && (_status & 3) != 3 ;
}

// motion in the Z-axis, +1 means mag field increasing (magnet approaching chip),
// -1 is decreasing, 0 is stable.
int AS5040::Zaxis ()
{
  switch (_status & 0x3)
    {
    case 0: return 0 ;
    case 1: return -1 ;
    case 2: return +1 ;
    }
  return 0 ; // invalid case, must return something harmless
}
