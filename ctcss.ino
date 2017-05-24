// Demo version on 2x16 LCD display

#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,10,9,8,7);


//////////////////  PARAMETERS WHICH CAN BE FINETUNED FOR OPTIMAL DECODING  /////////////////////////////////////////////////////////

//  Decoding of a CTCSS in presence of a signal properly modulated with a CTCSS subtone is no problem and can be easily implemented.
//  The challenge is however NOT TO DECODE in a random manner CTCSS in case of no RF signal, when only noise is present from the dicriminator output.
//  The sketch below includes a waveform analysis performed during the sampling period, to determine if a 'clean' CTCSS is present or not.
//  Some latitude is however permitted to take into account flutter, short spikes, interaction with voice modulation  etc. and still indicate a
//  stable and reliable CTCSS tone.

//  The parameters below allow to finetune the detection in accordance with CTCSS tone(s) to be detected.

//  If parameters are set too stringent, decoding will not be assured in a realiable manner in presence of voice signals
//  If parameters are not set 'tight' enough, stray decoding of 'FRIEND' CTCSS wil occur in presence of noise (signal from discriminator output)

//  the sketch allows decode of 'FRIEND' and 'FOE' CTCSS

//  Adjust the audio level fed to LPF and signal conditioner (making square waves out of sine) to a point where a reliable detection of highest desired CTCSS is possible, 
//  and in the noise the number of 'valid decodes' is 0 or 1, sometimes 2, but does not exceed 2.  Increasing further the audio level (valid decodes > 2) will only increase 
//  the risk of random decodes.  Normally, in preesence of only noise, maximum 1 stray decode of FRIEND CTCSS will occur per hour. 


volatile unsigned long ctcssBand = 10;  // This is +/- % allowed error in waveform format detected, to filter out noise (ideally , waveform should be a pure square wave)
                                        // Do not set too low or decoding will not be possible when voice signals are present

float validdecodes = 50;                // This is the % of valid CTCSS waveforms decoded in one sampling period, required to eliminate spikes & random decode in noise

int FmaxCtcss = 150;                    // Highest CTCSS frequency to decode. This will set a spikes filter and prevent false decode in noise  
int FminCtcss = 62;                     // Lowest CTCSS frequency to decode. This will set a spikes filter and prevent false decode in noise  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


byte ctcssGood = 0;                    // debug counter valid # FRIEND decodes
byte ctcssFriend = 0;                  // When friend ctcss decoded, goes to 1

volatile unsigned long counter = 0;
volatile unsigned long oddEven = 0;
volatile unsigned long startStopOk = 0;
volatile unsigned long secondLastStartStopOk = 0;
volatile unsigned long startStopTime = 0;
volatile unsigned long lastOddTime = 0;
volatile unsigned long secondLastOddTime = 0;
volatile unsigned long lastEvenTime = 0;
volatile unsigned long numPeriodesOk = 0;
volatile unsigned long totalTimeOk = 0;
volatile unsigned long periodTimeMinus = 0;
volatile unsigned long periodTimePlus = 0;
volatile unsigned long periodTime = 0;

float result = 0;
float freq = 0.0;

volatile unsigned long low_passF  = 0;
volatile unsigned long high_passF = 0;

////////////////////////////////////////////////////////////////////////////////

void setup()

   {                                                           // START OF setup

pinMode(13, OUTPUT);  
digitalWrite(13, LOW);

lcd.begin(16, 2);

// Recompute some variables

ctcssBand = ctcssBand * 100;
validdecodes = validdecodes / 100;

low_passF =  500000  / FmaxCtcss;                                  // Spikes filter
high_passF = 1000000 / FminCtcss;                                  // Spikes filter



   }                                                             // END OF setup



////////////////////////////////////////////////////////////////////////////////

void loop()   {                                                            // START OF loop

freq = getFrequency(100); 

// copy 7EQ

lcd.setCursor(0, 0);
lcd.print("F: ");
if (freq < 100) lcd.print(" ");
if (freq < 10) lcd.print(" ");

lcd.print (freq,2);

lcd.print(" Hz");

lcd.setCursor(13, 0);
lcd.print("v");  // show # valid waveforms
if (numPeriodesOk < 10) lcd.print(" ");
lcd.print (numPeriodesOk);

//

lcd.setCursor (0, 1);
// Too low but 
if (freq < 65.8) {
      lcd.print("NO CTCSS");
      digitalWrite(13, LOW);  // visual indication FRIEND CTCS
      ctcssFriend = 0;        //reset friend ctcss status

//DO SOMETHING
    }
//-----------------------------------------

else if ((freq > 66.00) && (freq < 68.00))
{
lcd.print("CT:  67.0");
//DO SOMETHING
}

//-----------------------------------------

else if ((freq > 68.30) && (freq < 70.30))
{
lcd.print("CT:  69.3");
//DO SOMETHING
}

else if ((freq > 70.90) && (freq < 72.90))
{
lcd.print("CT  71.9");
//DO SOMETHING 
}

else if ((freq > 73.40) && (freq < 75.40))
{
lcd.print("CT  74.4");
//DO SOMETHING 
}
else if ((freq > 76.00) && (freq < 78.00))
{
lcd.print("CT  77.0");
//DO SOMETHING 
}

else if ((freq > 78.70) && (freq < 80.70)){


        lcd.print("CT  79.7");
        digitalWrite(13, HIGH);
        
         if (ctcssFriend == 0){ 
            ctcssFriend = 1;
            ++ctcssGood;
            if(ctcssGood == 100) ctcssGood = 0; 
            }
   
 //DO SOMETHING WITH FRIEND DECODED
}
else if ((freq > 81.50) && (freq < 83.50))
{
lcd.print("CT  82.5");
//DO SOMETHING 
}


else if ((freq > 84.40) && (freq < 86.40))

lcd.print("CT:  85.4");
//DO SOMETHING


else if ((freq > 87.50) && (freq < 89.50))
{
lcd.print("CT  88.5");
//DO SOMETHING 
}

else if ((freq > 90.50) && (freq < 92.50))
{
lcd.print("CT:  91.5");
//DO SOMETHING
}


else if ((freq > 93.8) && (freq < 95.80))
{
lcd.print("CT  94.8");
//DO SOMETHING
}

else if ((freq > 96.40) && (freq < 98.4))
{
lcd.print("CT:  97.4");
//DO SOMETHING
}



else if ((freq > 99.00) && (freq < 101.00))
{
lcd.print("CT: 100.0");
//DO SOMETHING
}


else if ((freq > 102.50) && (freq < 104.50))
{
lcd.print("CT 103.5");
//DO SOMETHING 
}


else if ((freq > 106.20) && (freq < 108.20))
{
lcd.print("CT: 107.2");
//DO SOMETHING
}




else if ((freq > 109.90) && (freq < 111.90))
{
lcd.print("CT 110.9");
//DO SOMETHING 
}

else if ((freq > 113.80) && (freq < 115.80))
{
lcd.print("CT: 114.8");
//DO SOMETHING
}



else if ((freq > 117.80) && (freq < 119.80))
{
lcd.print("CT 118.8Hz");

//DO SOMETHING 
}

else if ((freq > 122.00) && (freq < 124.00))
{
lcd.print("CT 123.0");


//DO SOMETHING 
}



else if ((freq > 126.20) && (freq < 128.40))
{
lcd.print("CT: 127.3");
//DO SOMETHING
}

else if ((freq > 130.80) && (freq < 132.80))
{
lcd.print("CT: 131.8");
//DO SOMETHING
}

else if ((freq > 135.50) && (freq < 137.50))
{
lcd.print("CT: 136.5");
//DO SOMETHING
}

else if ((freq > 140.30) && (freq < 142.30))
{
lcd.print("CT: 141.3");
//DO SOMETHING
}

else if ((freq > 145.20) && (freq < 147.20))
{
lcd.print("CT: 146.2");
//DO SOMETHING
}

else if ((freq > 150.40) && (freq < 152.40))
{
lcd.print("CT: 151.4");
//DO SOMETHING
}

else if ((freq > 156.70) && (freq < 158.70))
{
lcd.print("CT: 157.7");
//DO SOMETHING
}

// NON-STANDARD
else if ((freq > 158.80) && (freq < 160.80))
{
lcd.print("CT: 159.8");
//DO SOMETHING
}

else if ((freq > 161.20) && (freq < 163.20))
{
lcd.print("CT: 162.2");
//DO SOMETHING
}

// NON-STANDARD
else if ((freq > 164.50) && (freq < 166.50))
{
lcd.print("CT: 165.5");
//DO SOMETHING
}

else if ((freq > 166.90) && (freq < 168.90))
{
lcd.print("CT: 167.9");
//DO SOMETHING
}

// NON-STANDARD
else if ((freq > 170.30) && (freq < 172.30))
{
lcd.print("CT: 171.3");
//DO SOMETHING
}

else if ((freq > 172.80) && (freq < 17.80))
{
lcd.print("CT: 173.8");
//DO SOMETHING
}

//NON-STANDARD
else if ((freq > 176.00) && (freq < 178.50))
{
lcd.print("CT: 177.3");
//DO SOMETHING
}

else if ((freq > 178.9) && (freq < 180.90))
{
lcd.print("CT: 179.9");
//DO SOMETHING
}

//NON-STANDARD
else if ((freq > 182.50) && (freq < 184.50))
{
lcd.print("CT: 183.5");
//DO SOMETHING
}
else if ((freq > 185.20) && (freq < 187.20))
{
lcd.print("CT: 186.2");
//DO SOMETHING
}

//NON-STANDARD
else if ((freq > 188.90) && (freq < 190.90))
{
lcd.print("CT: 189.9");
//DO SOMETHING
}

else if ((freq > 191.80) && (freq < 198.80))
{
lcd.print("CT: 192.8");
//DO SOMETHING
}

//NON-STANDARD
else if ((freq > 195.60) && (freq < 197.60))
{
lcd.print("CT: 196.6");
//DO SOMETHING
}

//NON-STANDARD
else if ((freq > 198.50) && (freq < 200.50))
{
lcd.print("CT: 199.5");
//DO SOMETHING
}

else if ((freq > 202.50) && (freq < 204.50))
{
lcd.print("CT: 203.5");
//DO SOMETHING
}

else if ((freq > 205.50) && (freq < 207.50))
{
lcd.print("CT: 206.5");
//DO SOMETHING
}

else if ((freq > 209.7) && (freq < 211.70))
{
lcd.print("CT: 210.7");
//DO SOMETHING
}
else if ((freq > 217.10) && (freq < 219.10))
{
lcd.print("CT: 218.1");
}

else if ((freq > 224.70) && (freq < 226.70))
{
lcd.print("CT: 225.7");
//DO SOMETHING
}

else if ((freq > 228.10) && (freq < 230.10))
{
lcd.print("CT: 229.1");
//DO SOMETHING
}

else if ((freq > 232.60) && (freq < 234.60))
{
lcd.print("CT: 233.6");
//DO SOMETHING
}

else if ((freq > 240.80) && (freq < 242.80))
{
lcd.print("CT: 241.8");
//DO SOMETHING
}

else if ((freq > 249.30) && (freq < 251.30))
{
lcd.print("CT: 250.3");
//DO SOMETHING
}

else if ((freq > 253.10) && (freq < 255.10))
{
lcd.print("CT: 254.1");
//DO SOMETHING
}

else if (freq > 255.10)
{
lcd.print("TOO HIGH       ");
//DO SOMETHING
}



else  {                        // No known CTCSS or noise
      lcd.setCursor (0, 1);
      lcd.print("NO CTCSS");
      
  
      digitalWrite(13, LOW);  // visual indication FRIEND CTCS
      ctcssFriend = 0;        //reset friend ctcss status
       }


/////  DO EVERY LOOP

// print counter #  interrupts
      lcd.setCursor (13, 1); 
      lcd.print("c"); 

       if ((counter-1)/2 < 10) lcd.print(" ");    
       lcd.print ((counter-1)/2) ;     // debug counter # interrupts in one sampling period
       
// print counter # valid friend decodes (Counts till 99 and rolls-over to 00)

      lcd.setCursor (9, 1);
          lcd.print(" "); 
          if (ctcssGood  < 10) lcd.print("0");
          lcd.print (ctcssGood,DEC);
 
   }                                                              // END OF loop


////////////////////////////////////////////////////////////////////////////////



float getFrequency(unsigned int sampleTime)   

   {                                           // START OF getFrequency FUNCTION

   counter = 1;
   totalTimeOk = 0;
   numPeriodesOk = 0;
   attachInterrupt(0, interruptHandlerChange, CHANGE);      
   delay(sampleTime);
   detachInterrupt(0);

   if (totalTimeOk < 40000) {
      result = 0.0;
      }
    else if ((totalTimeOk / numPeriodesOk) < 4000){            // digital filter Low Pass
      result = 0.0;
      }
    else if ((totalTimeOk / numPeriodesOk) > 16000){           // digital filter High Pass
      result = 0.0;
      }  
    else if (numPeriodesOk <= (((counter-1)/2)*validdecodes))   // check how many valid waves in one sample
      {
      result = (0.0);
      }


   else
      {
      result = (1000700.0 * (float)(numPeriodesOk))/(float)(totalTimeOk);
      
// NOTE: 1000700.0 is the value found by me. The theoretic value is "1.000.000,0"
// Start with this value and check the precision against a good frequency meter.
       }
                                                                                                  
   return result;
    }                                             // END OF getFrequency FUNCTION


////////////////////////////////////////////////////////////////////////////////

void interruptHandlerChange()

   {                                  // START OF interruptHandlerChange ROUTINE

   startStopTime = micros();

   if (counter == 1)                  // if counter is 1 : we have to initialize
      {  
      oddEven = 1;
      startStopOk = 2;   
      secondLastStartStopOk = 2;
      lastOddTime = startStopTime;
      secondLastOddTime = startStopTime;
      lastEvenTime = startStopTime;               
      }

   if (oddEven == 1)                    // lets pretend : odd is a rising change
      {
      oddEven = 2;
      startStopOk = 1;

      if (((startStopTime - lastEvenTime) < low_passF))      // Spikes filter
         {
         startStopOk = 2;
         }

      if (((startStopTime - lastOddTime) < (low_passF * 2)))       
         {
         startStopOk = 2;
         secondLastStartStopOk = 2;
         }

      if (((startStopTime - lastOddTime) > high_passF))     // Spikes filter
         {
         startStopOk = 2;
         secondLastStartStopOk = 2;
         }

      lastOddTime = startStopTime;
      }

   else                               // lets pretend : even is a falling change
      {
      oddEven = 1;

      if (((startStopTime - lastOddTime) < low_passF))
         {
         startStopOk = 2;
         }

      if ((startStopOk == 1) && (secondLastStartStopOk == 1)) {

         if (numPeriodesOk == 0)
            {  
            numPeriodesOk++;
            totalTimeOk = totalTimeOk + lastOddTime;
            totalTimeOk = totalTimeOk - secondLastOddTime;
            }

         else
            {
            periodTime = totalTimeOk * ctcssBand;  //  Wave form analysis 
            periodTimeMinus = (totalTimeOk * 10000);
            periodTimePlus = periodTimeMinus;
            periodTimeMinus = (periodTimeMinus - periodTime);
            periodTimePlus = (periodTimePlus + periodTime);
            periodTimeMinus = (periodTimeMinus / numPeriodesOk);
            periodTimePlus = (periodTimePlus / numPeriodesOk); 
            periodTime = (lastOddTime - secondLastOddTime);
            periodTime = (periodTime * 10000);

            if ((periodTime > periodTimeMinus) && (periodTime < periodTimePlus))
               {
               numPeriodesOk++;
               totalTimeOk = totalTimeOk + lastOddTime;
               totalTimeOk = totalTimeOk - secondLastOddTime;
               }
                         
             else {
                if (numPeriodesOk < 3) {
                numPeriodesOk = 1;
                totalTimeOk = lastOddTime - secondLastOddTime;
                }
             }
           } 
         }

      if ( startStopOk == 1 )
         {
         secondLastOddTime = lastOddTime;
         secondLastStartStopOk = startStopOk;
         }

      lastEvenTime = startStopTime;   
      }
 
   counter++;  // debug good decodes

   }                                    // END OF interruptHandlerChange ROUTINE

////////////////////////////////////////////////////////////////////////////////
