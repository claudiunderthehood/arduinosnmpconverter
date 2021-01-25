#include <Wire.h>
//#include <AD9959.h>

static float Gain_A1;
static float GainP_A2;
static float ampAz_A3;
static float ampPAz_A4;
static float ampEl_A5;
static float ampPEl_A6;
static bool process = false;

const int csbPin = 7;
const int IO_UpdatePin = 2;
const int resetPin = 10;
const int SDIO3Pin = 3;

//AD9959 myDDS = AD9959(resetPin, IO_UpdatePin, csbPin, SDIO3Pin);

long testFreq = 70; //MHz
long fs = 500; // ref clk in MHz
long testFrequencyTuningWord = (((pow(2,32))*testFreq)/fs);



void setup() {
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600); 
  //myDDS.initialize();

}


bool exitM1(int i, int howMany)
{
  return (i <howMany);
}

bool exitM3(char c, int i, int howMany)
{
  return (c!='~' && i <howMany);
}

void receiveEvent(int howMany){

   
    int headerLen = 2;
   
    char *singleValue = (char *)malloc(15+1 * sizeof(char));
    char *header= (char *)malloc(2+1 * sizeof(char));
    
    int i =0;
    while(exitM1(i,headerLen))
    {
      char c= Wire.read(); 
      header[i] = c;
      i++;
      Serial.println(c);
    }
    
    header[i]='\0';

   if (strcmp("~S",header)==0)
    {
      Serial.println("START");
      process = true;
      Gain_A1=GainP_A2=ampAz_A3=ampPAz_A4=ampEl_A5=ampPEl_A6=0;
      return;
    }

    if (process==false) return;
    
    
    int x = 0;
    while(exitM1(i,howMany))
    {
      char c= Wire.read(); 
      singleValue[x]=c;
      x++;
      i++;
      Serial.println(c);
    }
   
    singleValue[x]='\0';
 
    if (strcmp("A1",header)==0)
        Gain_A1= atof(singleValue);

    if (strcmp("A2",header)==0)
        GainP_A2= atof(singleValue);

    if (strcmp("A3",header)==0)
        ampAz_A3= atof(singleValue);

    if (strcmp("A4",header)==0)
        ampPAz_A4= atof(singleValue);

    if (strcmp("A5",header)==0)
        ampEl_A5= atof(singleValue);

    if (strcmp("A6",header)==0)
        ampPEl_A6= atof(singleValue);

    if (strcmp("~E",header)==0)
    {
      
      Serial.println(Gain_A1,4);
      Serial.println(GainP_A2,4);
      Serial.println(ampAz_A3,4);
      Serial.println(ampPAz_A4,4);
      Serial.println(ampEl_A5,4);
      Serial.println(ampPEl_A6,4);
      Serial.println("END");
      process =false;
      return;
    }
    
    

}
    


void loop() {

    /*myDDS.channelSel(0);
    myDDS.writeFreq(testFrequencyTuningWord);
    myDDS.writeAmp(Gain_A1);
    myDDS.writePhase(0);
    delay(1000);

    myDDS.channelSel(1);
    myDDS.writeFreq(testFrequencyTuningWord);
    myDDS.writeAmp(sqrt(ampAz_A3));
    myDDS.writePhase(ampPAz_A4);
    delay(1000);

    myDDS.channelSel(2);
    myDDS.writeFreq(testFrequencyTuningWord);
    myDDS.writeAmp(sqrt(ampEl_A5));
    myDDS.writePhase(ampPEl_A6);
    delay(1000);
    */
   

}
