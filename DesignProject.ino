#include <sim900.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <I2C_RTC.h>
//#include <LiquidCrystal.h> //LCD library, only used for visualization

static DS1307 RTC; // Initialize the RTC on pins A4 and A5

SoftwareSerial shieldSerial(10,11);

// Array of hours and Light ON/OFF values( 0=OFF & 1=ON )
int array[2][24] = {
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
  {1, 1, 1, 1, 1, 1, 1, 0, 0, 0,  0,  0,  0,  0,  0,  0,   0,  0,  1,  1,  1,  1,  1,  1}
};     

//const int rs=12, en=9, d4=5, d5=4, d6=3, d7=2;  //LCD code, only used for visualization
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Declare SMS constants         
const char* PHONE_NUMBER = "+xxxxxxxxxxxx";                  //Define the phone number to send the text message to
const char* TEXT_MESSAGE_HIGH = "LetThereBeLight!";          //Lights are on text message
const char* TEXT_MESSAGE_LOW = "IWasBornInTheDark";          //Lights are off text message

byte voltPin = A0;                                           //assign voltage to input pin

void setup() {
  Serial.begin(9600);

  SIM900 sim900(&shieldSerial);

  SIM900APN access;
  access.apn = F("");
  access.username = F("");
  access.password = F("");

  if(!sim900.connectAPN(access)) {
    Serial.println(F("Failed to connect to APN."));
    return;
  }
  Serial.println(F("Connected to APN!"));

  bool gprsEnabled = sim900.enableGPRS();
  Serial.println(gprsEnabled ?
    F("GPRS was successfully enabled.") :
    F("Cannot start GPRS."));

  if(!gprsEnabled)
    return;
  Serial.println("IP Address: " + sim900.ipAddress());


  pinMode(voltPin, INPUT);              //input variable dc voltage
  pinMode(7, OUTPUT);		                //Set pin 7 as output  
  pinMode(6, OUTPUT);		                //D6 is output to relay latch
  float feedbackValue;                  //Declare a byte variable to store feedback sensor value
  byte batteryValue;	                  //Declare a byte variable to store the analog input value of battery voltage
  //lcd.begin(16, 2);                   //LCD library, only used for visualization

  // Initialize the RTC
  RTC.begin();
  RTC.setHourMode(CLOCK_H24);
  RTC.setWeek(1);
  RTC.setDate(17,10,23);
  RTC.setTime(14,00,00);
  if (!RTC.begin()) {
    Serial.println("RTC not found, Restart Required");	
    while (1);
  }
}

void loop() {
  int hour = RTC.getHours();				                     // Get the current hour from the RTC
  if(array[1][hour] == 1){
    digitalWrite(6, HIGH);  		               // Write the value in the second row of the array corresponding to the current hour to pin A6
   }
  else{
    digitalWrite(6, LOW);  			             // Write the value in the second row of the array corresponding to the current hour to pin A6
  }
  
  int readValue = analogRead(voltPin);                    //read pin A0 value
  float voltage = readValue * (12.7/1023.0);                     //calculate real world voltage
  float stopBatteryVoltage = 12.22;
  if(voltage < stopBatteryVoltage){
    digitalWrite(7, HIGH);
  }
  else{
    digitalWrite(7, LOW);
  }

  sendFeedbackMessage();
  delay(3600000);

  hour = 0;
  readValue = 0;
  voltage = 0.0;

}


void sendFeedbackMessage(){
  float feedbackValue = analogRead(A3);
  if(feedbackValue != 0){
    SIM900 sim900(&shieldSerial);
    Serial.println(sim900.sendSMS("xxxxxxxxxxx",TEXT_MESSAGE_HIGH)? "Sent" : "Not sent.");
    sim900.close(); 
    //lcd.setCursor(0, 1);
    //lcd.print(TEXT_MESSAGE_HIGH); //LCD code, only used for visualizaztion
    //delay(200);
    //lcd.clear();
  }
  else{
    SIM900 sim900(&shieldSerial);
    Serial.println(sim900.sendSMS("xxxxxxxxxxx",TEXT_MESSAGE_LOW)? "Sent" : "Not sent.");
    sim900.close();
    //lcd.setCursor(0, 1);
    //lcd.print(TEXT_MESSAGE_LOW);  //LCD code, only used for visualizaztion
    //delay(200);
    //lcd.clear();
  }
  feedbackValue = 0.0;
  return;
}



      





