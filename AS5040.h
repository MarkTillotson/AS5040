/*
 * AS5040.h - Arduino library for AMS AS5040 magnetic rotary encoder chip
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

#ifndef AS5040_h
#define AS5040_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

// mode defines
#define AS5040_QUADRATURE  0x1
#define AS5040_STEPDIR     0x2
#define AS5040_COMMUTATE   0x3
#define AS5040_COMMUTATE_4POLE 0xB

#define AS5040_10BIT  0x0
#define AS5040_9BIT   0x4
#define AS5040_8BIT   0x8
#define AS5040_7BIT   0xC

#define AS5040_WIDE_IDX  0x10

// defines for 5 bit _status value
#define AS5040_STATUS_OCF 0x10
#define AS5040_STATUS_COF 0x08
#define AS5040_STATUS_LIN 0x04
#define AS5040_STATUS_MAGINC 0x02
#define AS5040_STATUS_MAGDEC 0x01

class AS5040
{
 public:
  AS5040 (byte pinCLK, byte pinCS, byte pinDO, byte pinPROG = 0xFF) ;

  boolean begin () ;
  boolean begin (byte mode) ;
  boolean begin (byte mode, boolean reverse, unsigned int offset) ;

  unsigned int read () ; 
  byte status () ;
  boolean valid () ;
  int Zaxis () ;

 private:

  byte _pinCLK ;
  byte _pinCS ;
  byte _pinDO ;
  byte _pinPROG ;

  byte _status ;
  byte _parity ;

  byte even_parity (byte val) ;
} ;


#endif
