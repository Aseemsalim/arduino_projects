unsigned long count=0,Capture_Value;
float Frequency;
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);

void setup() {
  Serial.begin(9600);
  ACSR=ACSR&0xFB;       // Analog trigger off
  TCCR1A=TCCR1A&0xFC;   // Timer 1 working in normal mode WGM13 and WGM12= 0
  TCCR1B=TCCR1B&0xE7;   // Timer 1 working in normal mode WGM10 and WGM11= 0
  TCCR1B=TCCR1B|0x80;   // Input noise canceller is on DB7 bit
  TCCR1B=TCCR1B&0xBF;   // Falling edge triggering is used
  TCCR1B=TCCR1B&0xF9;   // |
  TCCR1B=TCCR1B|0x01;   // Timer 1 Clock source = CLK
  PRR=PRR&0xF7;         // No power reduction mode in Timer 1
  TIMSK1=TIMSK1|0x01;    // Timer overflow interupt is enabled
  TIMSK1=TIMSK1|0x20;    // Input capture interrupt is enabled
  SREG=SREG|0x80;       // Global Interrupt is enabled
}
ISR(TIMER1_OVF_vect)
{
    count++;
}
ISR(TIMER1_CAPT_vect)
{
  TCNT1=0;
  Capture_Value=ICR1|(count*0x10000);
  Frequency=1000000/(Capture_Value*0.0625);   
  count=0;
}
void loop() {
  Serial.println(Frequency,DEC);
  delay(1000);
}
