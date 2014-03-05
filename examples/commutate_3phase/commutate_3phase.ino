#include <AS5040.h>

// CHANGE THESE AS APPROPRIATE
#define CSpin   2 
#define CLKpin  3
#define DOpin   4
#define PROGpin 5

// OUTPUT PINS
#define hall_u  6
#define hall_v  7
#define hall_w  8

AS5040 enc (CLKpin, CSpin, DOpin, PROGpin) ;

void setup ()
{
  Serial.begin (115200) ;   // NOTE BAUD RATE
  if (!enc.begin (AS5040_COMMUTATE))
    Serial.println ("Error setting up AS5040") ;
}

void loop ()
{
  byte u = digitalRead (hall_u) ;
  byte v = digitalRead (hall_v) ;
  byte w = digitalRead (hall_w) ;
  Serial.print (u) ; Serial.print (v) ; Serial.println (w) ;
  delay (100) ;
}
