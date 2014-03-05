#include <AS5040.h>

// CHANGE THESE AS APPROPRIATE
#define CSpin   2
#define CLKpin  3
#define DOpin   4

// OUTPUT PINS
#define enc_a  6
#define enc_b  7
#define enc_z  8


AS5040 enc (CLKpin, CSpin, DOpin) ;

// Set mode to quadrature (A + B + index), monitor via serial IF
void setup ()
{
  Serial.begin (115200) ;   // NOTE BAUD RATE
  if (!enc.begin ())
    Serial.println ("Error setting up AS5040") ;
}

void loop ()
{
  Serial.print (enc.read (), HEX) ;
  Serial.print ("   ") ;
  Serial.print (enc.status (), BIN) ;
  Serial.print ("   ") ;
  Serial.print (enc.valid () ? "OK" : "Fault") ;
  Serial.print ("   ") ;
  Serial.println (enc.Zaxis ()) ;
  delay (400) ;
}
