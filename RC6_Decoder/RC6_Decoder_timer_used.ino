#define IR_RECV_PIN 2
#define RESET_TIMER TCCR2B=0
#define RELOAD_TIMER TCNT2=156
#define ENABLE_TIMER TCCR2B=2
unsigned int ir_decoded_result;
void IR_Receive_Enable(byte ir_recv_pin)
{
  pinMode(ir_recv_pin,INPUT); // make ir recv pin as input
  TCCR2A=0;                   // Set timer2 in normal mode
  TCCR2B=0x02;                // Fosc/8 prescaling 
  TCNT2=156;                  // Interrupt at ((0.0625*8)*(256-156))~=50us
  TIMSK2=1;                   // Timer2 overflow interrupt enabled 
  SREG=0x80;                  // Global interrupt is enabled   
}

// ISR for receiving ir codes //
ISR(TIMER2_OVF_vect)
{
  RESET_TIMER;
  static unsigned int timer_count=0;
  unsigned int result=0;
  timer_count++;
  if(timer_count>=2500)
  {
    timer_count=0;
    ir_decoded_result=0;
  }
  signed char i;
  if(!digitalRead(IR_RECV_PIN))        // Check the leading start bit
  { 
      delayMicroseconds(3580);
  if(!digitalRead(IR_RECV_PIN))        // check the short start bit
  {
       delayMicroseconds(5593);  // Leave field bits and toggle bits
       // Receive RC6
       for(i=15;i>=0;i--)
       {
           if(digitalRead(IR_RECV_PIN))
           result|=(1<<i);
           else
           result|=(0<<i);
           delayMicroseconds(895);
       }       
  }
  else                            // if error result = null value
  {
      result=0;
  }
} 
    if(result)
    ir_decoded_result=result;
    RELOAD_TIMER;
    ENABLE_TIMER;   
}
void setup() 
{
  Serial.begin(9600);
  IR_Receive_Enable(IR_RECV_PIN);
}

void loop() 
{
  if(ir_decoded_result)
  {
   Serial.println(ir_decoded_result,HEX); 
  }
  delay(100);
}
