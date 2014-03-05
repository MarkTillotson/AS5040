#include <AS5040.h>

// CHANGE THESE AS APPROPRIATE
#define CSpin   2
#define CLKpin  3
#define DOpin   4
#define PROGpin 5

// OUTPUT PINS
#define enc_step 6
#define enc_dir  7
#define enc_z    8

AS5040 enc (CLKpin, CSpin, DOpin, PROGpin) ;


// Set mode to step + direction + index, monitor via serial IF
void setup ()
{
  Serial.begin (115200) ;   // NOTE BAUD RATE
  if (!enc.begin (AS5040_STEPDIR | AS5040_8BIT))  // set to 8 bits (7 to 10 is the range)
    Serial.println ("Error setting up AS5040") ;
}

void loop ()
{
  Serial.print (enc.read (), HEX) ;   // MONITOR SERIAL DATA
  Serial.print ("   ") ;
  Serial.print (enc.status (), BIN) ;
  Serial.print ("   ") ;
  Serial.print (enc.valid () ? "OK" : "Fault") ;
  Serial.print ("   ") ;
  Serial.println (enc.Zaxis ()) ;
  delay (400) ;
}
