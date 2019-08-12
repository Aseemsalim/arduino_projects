#define False 0
#define True 1
#define IR_Sensor_Pin 2
#define LED_Pin 13

unsigned char Repeat=False,Decoded=False;

void Decode_NEC(
                unsigned char *NEC_Data,
                unsigned char *NEC_Data_Inverse,
                unsigned char *NEC_Address,
                unsigned char *NEC_Address_Inverse
                )
{
  signed char i;
  if(!digitalRead(IR_Sensor_Pin))        // Check the  start bit
  {
      Decoded=1;
      delayMicroseconds(11250);

      if(!digitalRead(IR_Sensor_Pin))      // check repeat
      Repeat=1;
      
      else 
      {
      delayMicroseconds(2250);
      Repeat=0;
      }
      
      if(!digitalRead(IR_Sensor_Pin))       // check it is nec
      {
       // Receive 8 bit Address
       for(i=7;i>=0;i--)
       {
           delayMicroseconds(1125);;
           if(!digitalRead(IR_Sensor_Pin))
           *NEC_Address|=(0<<i);
           else
           {
           *NEC_Address|=(1<<i);
           delayMicroseconds(1125);;
           }
       }
        // Receive 8 bit Address Inverse
       for(i=7;i>=0;i--)
       {
           delayMicroseconds(1125);;
           if(!digitalRead(IR_Sensor_Pin))
           *NEC_Address_Inverse|=(0<<i);
           else
           {
           *NEC_Address_Inverse|=(1<<i);
           delayMicroseconds(1125);;
           }
       }
        // Receive 8 bit Data
       for(i=7;i>=0;i--)
       {
           delayMicroseconds(1125);;
           if(!digitalRead(IR_Sensor_Pin))
           *NEC_Data|=(0<<i);
           else
           {
           *NEC_Data|=(1<<i);
           delayMicroseconds(1125);;
           }
       }
        // Receive 8 bit Data inverse
       for(i=7;i>=0;i--)
       {
           delayMicroseconds(1125);;
           if(!digitalRead(IR_Sensor_Pin))
           *NEC_Data_Inverse|=(0<<i);
           else
           {
           *NEC_Data_Inverse|=(1<<i);
           delayMicroseconds(1125);;
           }
       }
  }
  else
    {
           *NEC_Data=0xFF;
           *NEC_Data_Inverse=0xFF;
           *NEC_Address=0xFF;
           *NEC_Address_Inverse=0xFF;
           Decoded=0;
           
    }
  }
  else
  Decoded=0;
}

void setup()
{
  Serial.begin(9600);
  pinMode(IR_Sensor_Pin,INPUT);
  pinMode(LED_Pin,OUTPUT);
  digitalWrite(LED_Pin,HIGH);
}

void loop()
{
  unsigned char NEC_Address,NEC_Address_Inverse,NEC_Data,NEC_Data_Inverse;
  for(;;)
  {
    Decode_NEC(&NEC_Address,&NEC_Address_Inverse,&NEC_Data,&NEC_Data_Inverse);
    if(Decoded==True)
    {
      delay(75);
      Serial.println(" ");
      if(Repeat==True)
         Serial.println("Repeat");
      else
      {
        Serial.print(NEC_Data,HEX);
        Serial.print(NEC_Data_Inverse,HEX);
        Serial.print(NEC_Address,HEX);
        Serial.print(NEC_Address_Inverse,HEX);
      }
      NEC_Address=0;
      NEC_Address_Inverse=0;
      NEC_Data=0;
      NEC_Data_Inverse=0;
    }
    
  }
}
        
       