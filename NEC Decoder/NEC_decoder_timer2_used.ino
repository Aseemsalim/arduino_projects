#define  IR_RECV_PIN 2
#define RESET_TIMER TCCR2B=0
#define RELOAD_TIMER TCNT2=100
#define ENABLE_TIMER TCCR2B=2
unsigned long ir_decoded_result;
void IR_Receive_Enable(byte ir_recv_pin )
{
  pinMode(ir_recv_pin,INPUT); // make ir recv pin as input
  TCCR2A=0;                   // Set timer2 in normal mode
  TCCR2B=0x02;                // Fosc/8 prescaling 
  TCNT2=156;                  // Interrupt at ((0.0625*8)*(256-156))~=50us
  TIMSK2=1;                   // Timer2 overflow interrupt enabled 
  SREG=0x80;                  // Global interrupt is enabled   
}

ISR(TIMER2_OVF_vect)
{
  RESET_TIMER;
  static unsigned int timer_count=0;
  unsigned long result=0;
  timer_count++;
  if(timer_count>=2500)
  {
    timer_count=0;
    ir_decoded_result=0;
  }
  signed char i;
  unsigned int NEC_Address=0,NEC_Data=0;
  if(!digitalRead(IR_RECV_PIN))        // Check the  start bit
  {
        delayMicroseconds(11250);
        delayMicroseconds(2250);
      
      if(!digitalRead(IR_RECV_PIN))       // check it is nec or not
      {
       // Start of reception
       
       // Receive 16 bit Address
       for(i=0;i<=15;i++)
       {
           delayMicroseconds(1125);;
           if(!digitalRead(IR_RECV_PIN))
            NEC_Address|=(0<<i);
           else
           {
            NEC_Address|=(1<<i);
           delayMicroseconds(1125);;
           }
       }
        // Receive 16 bit Data
       for(i=0;i<=15;i++)
       {
           delayMicroseconds(1125);;
           if(!digitalRead(IR_RECV_PIN))
           NEC_Data|=(0<<i);
           else
           {
           NEC_Data|=(1<<i);
           delayMicroseconds(1125);;
           }
       }
       //end of reception
       
       // address and data joining to form single result
       result=NEC_Data;
       result<<=16;
       result|=NEC_Address;
       // end of result joining 
      }
  else
     result=0;       // if error received return null value
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
