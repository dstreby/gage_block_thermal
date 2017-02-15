#include "Arduino.h"

// Thermistor pin(s)
#define THERMISTORPIN A0 
#define THERMISTORPIN_2 A1
// rThermistor nominal resistance
#define R_NOMINAL 10000      
// Temperature for thermistor nominal resistance (C)
#define T_NOMINAL 25   
// Thrmistor beta coefficient (3000 - 4000 typical)
#define BCOEFFICIENT 3950
// Voltage Divider R2
#define R2 10000
// Averaging sample size
#define AVG_SAMPLES 5
// Averaging sample interval (ms)
#define AVG_TIME 20
// Sampling interval (ms) (must be greater than AVG_SAMPLES * AVG_TIME)
#define INTERVAL 200

int r1_vout[AVG_SAMPLES];
int r1_vout_2[AVG_SAMPLES];

void setup(void) {
  Serial.begin(9600);
}
 
void loop(void) {
  uint8_t i;
  float r1_vout_avg, r1, temp;
  float r1_vout_avg_2, r1_2, temp_2;

  // take N samples for smoothing
  for (i=0; i< AVG_SAMPLES; i++) {
   r1_vout[i] = analogRead(THERMISTORPIN);
   r1_vout_2[i] = analogRead(THERMISTORPIN_2);
   delay(AVG_TIME);
  }

  // average all the samples out
  r1_vout_avg = 0;
  r1_vout_avg_2 = 0;
  for (i=0; i< AVG_SAMPLES; i++) {
     r1_vout_avg += r1_vout[i];
     r1_vout_avg_2 += r1_vout_2[i];
  }
  r1_vout_avg /= AVG_SAMPLES;
  r1_vout_avg_2 /= AVG_SAMPLES;

  // Calculate thermistor R
  // r1 = R2 * (( 1023 / r1_vout_avg ) - 1 );
  // r1_2 = R2 * (( 1023 / r1_vout_avg_2 ) - 1 );
	// r_2 = (Vout * r_1) / (Vin - Vout)
	r1 = ( r1_vout_avg * R2 ) / ( 1023 - r1_vout_avg );
	r1_2 = ( r1_vout_avg_2 * R2 ) / ( 1023 - r1_vout_avg_2 );


  // Use Steinhart formula to calculate temperature (K)
  // It's not super accurate, but it works.
  temp = r1 / R_NOMINAL;                  // (R/Ro)
  temp = log(temp);                       // ln(R/Ro)
  temp /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  temp += 1.0 / (T_NOMINAL + 273.15);     // + (1/To)
  temp = 1.0 / temp;                      // Invert

  // Do it again for the second thermistor
  temp_2 = r1_2 / R_NOMINAL;              // (R/Ro)
  temp_2 = log(temp_2);                   // ln(R/Ro)
  temp_2 /= BCOEFFICIENT;                 // 1/B * ln(R/Ro)
  temp_2 += 1.0 / (T_NOMINAL + 273.15);   // + (1/To)
  temp_2 = 1.0 / temp_2;                  // Invert

  /*
  // Convert temperature from K to C  
  temp -= 273.15;
  temp_2 -= 273.15;
  // Convert temperature from C to F
  temp = (1.8 * temp) + 32;
  temp_2 = (1.8 * temp_2) + 32;
  */
  
  // Report the temps back over serial
	Serial.print(millis());
  Serial.print("\t");
  Serial.print(temp);
  Serial.print("\t");
  Serial.print(temp_2);
  Serial.print("\t");
  Serial.print(abs(temp - temp_2));
  Serial.print("\n");
 
  delay(INTERVAL - (AVG_TIME * AVG_SAMPLES));
}
