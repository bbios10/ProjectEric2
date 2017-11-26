#include <Arduino.h>
#include <ToneDetector.h>
#include <DS3231.h>
#include   <TimerOne.h>
int measurePin = A3; 
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
int noiseval,dustval; 
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
    int i=0;
    int t=0;
    int a=0;
    uint16_t tFrequency;
    String warn,warn1; 
void outputFrequencyAsBar(uint16_t aFrequency);
// Pin 13 has an LED connected on most Arduino boards.
const int LED_PIN = 13;

bool ON = false;
// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);
//
String batdau;
String ketthuc;
String noisevalue;
String dustvalue;
//
char command;
String string;
int stage;
int value ;
String Incomedata[8];
boolean manu = false;
#include <string.h>


void setup() {
    setToneDetectorControlDefaults();  
  setToneDetectorMatchValues(1400, 1700);  
  pinMode(LED_PIN, OUTPUT);
  // Setup Serial connection
  Serial.begin(9600);
  // Uncomment the next line if you are using an Arduino Leonardo
  while (!Serial) {}
  
  // Initialize the rtc object
  rtc.begin();
  
  // The following lines can be uncommented to set the date and time
  rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(22, 36, 50);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(20, 11, 2017);   // Set the date to January 1st, 2014

  Timer1.initialize(1000000);
  Timer1.attachInterrupt (Xuli);       
}

void loop() {
  if(ON == true) Frequency();
  //
  // put your main code here, to run repeatedly:
if (Serial.available() > 0) 
    {string = "";}
    
    while(Serial.available() > 0)
    {
      command = ((byte)Serial.read());
      
      if(command == ':')
      {
        break;
      }
      
      else
      {
        string =string+ command;
      }
      
      delay(1);      
    } 
       if(string != "")
       {
       value = 0;
    Incomedata[0] = string.charAt(0);
    Incomedata[1] = string.charAt(1);
    Incomedata[2] = string.charAt(2);
    Incomedata[3] = string.charAt(3);
    Incomedata[4] = string.charAt(4);
    Incomedata[5] = string.charAt(5);
    Incomedata[6] = string.charAt(6);
    Incomedata[7] = string.charAt(7);
    //Serial.println(string);

    if(Incomedata[0]== "A")
    {
    noisevalue = Incomedata[1]+Incomedata[2]+Incomedata[3]+Incomedata[4];
    noiseval = noisevalue.toInt();
        
     //println(DoOn);    
    dustvalue = Incomedata[5]+Incomedata[6]+Incomedata[7];
    dustval = dustvalue.toInt();
    Serial.println(dustval);
    Serial.println(noiseval);    
     //println(DoBui);
    }
    else
    {
    batdau = Incomedata[0]+ Incomedata[1] + ":" +Incomedata[2]+ Incomedata[3] + ":00" ;
    ketthuc = Incomedata[4]+ Incomedata[5] + ":" +Incomedata[6]+ Incomedata[7] + ":00" ;

    Serial.println(batdau);
    Serial.println(ketthuc);
    }
  //  delay(1000);
    string ="";
    }
    }   
    void Frequency()
{       noise();
        //dust();
        senddata();  
 /* Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured); 
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage); 
  Serial.print(" - Dust Density: ");
  Serial.println(dustDensity); */
       // delay(100);                               
  }
  void noise()
  {
    //Serial.println(tFrequency);        
        for(t=0; t < 10000; t =t+100)
        { 
          if( ON == false) break;          
          tFrequency = readSignal();
         // delayMicroseconds(samplingTime);              
        //delayMicroseconds(deltaTime);  
        //delayMicroseconds(sleepTime);       
          if (tFrequency > noiseval) 
            { 
            //Serial.println(tFrequency); 
            //Serial.println('~');
             i=i+1;
            }
        //  delay(1);
         }             
               if(i>2)
              {
                 //Serial.println("LED ON");
                  warn = "1";
//                  digitalWrite(LED_PIN,HIGH);
                  delay(500);
//                  digitalWrite(LED_PIN,LOW);
                  i=0;                
              }
           else
              { warn = "0";
                 //Serial.println("LED OFF");
                 //digitalWrite(LED_PIN,LOW);              
                 delay(1);
        }
        //delayMicroseconds(samplingTime); 
        //voMeasured = analogRead(measurePin);// read the dust value             
        //delayMicroseconds(deltaTime);  
        //delayMicroseconds(sleepTime);        
        //Serial.print(voMeasured);   
        calcVoltage = voMeasured * (3.3 / 1024);
        dustDensity = 0.17 * calcVoltage - 0.1;
        delay(100);
        //Serial.println(dustDensity);
        if (dustDensity > dustval)
          {
            warn1 = "1";
            delay(1);            
            } 
            else
            {
              warn1 = "0";
              delay(1);              
              } 
    }
    
   /* void dust()
    {
        //delayMicroseconds(samplingTime); 
       // voMeasured = analogRead(measurePin); // read the dust value         
        //delayMicroseconds(deltaTime);  
       // delayMicroseconds(sleepTime);
                       
        calcVoltage = voMeasured * (3.3 / 1024);
 
        // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
       // Chris Nafis (c) 2012
        dustDensity = 0.17 * calcVoltage - 0.1;
          if (dustDensity > 0.5)
          {
            warn1 = "1";            
            } 
            else
            {
              warn1 = "0";              
              }
      }
      */
      void senddata()
      {
        Serial.print('#');                
        Serial.print(warn);
        Serial.print('+');
        Serial.print(warn1);
        Serial.print('~');        
        delay(10);
        }

  void Xuli()
{
   // Send Day-of-Week
  //Serial.print(rtc.getDOWStr());
  //Serial.print(" ");
  // Send date
  //Serial.print(rtc.getDateStr());
  //Serial.print(" -- ");
  // Send time
  //Serial.println(rtc.getTimeStr());

  String a = rtc.getTimeStr();
  
    if (a == batdau)
  {
    ON = true;   
        
    }
     else if (a == ketthuc)
  {
    ON = false;  
   }
   //println(ON);
} 

     
     
     

  
  
   

