#include "NMEAGPS.h"
#include <NeoSWSerial.h>
#include <LiquidCrystal.h>

#define RXPIN 3
#define TXPIN 4

NMEAGPS gps;
gps_fix fix;
NeoSWSerial gps_port(RXPIN, TXPIN);

const int buzzer = 13; 
const int rs = 12, en = 11, d4 = 5, d5 = 7, d6 = 6, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Uncomment one of these at a time.
NeoSWSerial SIM900(9,10);            // use this for really sending a message
//HardwareSerial & SIM900 = Serial; // use this for testing... SIM900 commands visible on Serial Monitor

char textmessage[160];
void setup()
{

  lcd.begin(16, 2);
  // Print a message to the LCD.
      lcd.print("EMERGENCY!"); 
  Serial.begin( 9600 );  
  Serial.println( F("unixbox911 test") );

  SIM900  .begin( 9600 );
  delay(100);
  SIM900.print("AT+CMGF=1\r"); // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
 
  gps_port.begin( 9600 );
}

void loop()
{
  while (gps.available( gps_port )) {
    fix = gps.read();
    doSomeWork( fix );
  }


} // loop


void doSomeWork( gps_fix & fix )
{
  if (fix.valid.location) {
    // Only do this when the GPS has a good lat/lon

    char message[40];
    strcpy_P( message, (const char *) F("http://www.google.com/maps/place/") );
    dtostrf( fix.latitude (), 4, 6, &message[ strlen(message) ] );
    strcat_P( message, (const char *) F(",") );
    dtostrf( fix.longitude(), 4, 6, &message[ strlen(message) ] );


    
    
    
    Serial.println(message);
    Serial.println();

    Serial.print( F("Connected Sats ::: ") );
    if (fix.valid.satellites)
      Serial.println( fix.satellites );
    Serial.println();

    Serial.println(message);
    Serial.println();
      
    SIM900.listen();
    SIM900INIT(message);
    SIM900STOP();
    gps_port.listen();
  }
}
  

void SIM900INIT( char msg[] )
{



  tone(buzzer, 1000); 
  delay(1000);        
  noTone(buzzer);     
  delay(1000);    

SIM900.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM900.println("AT+CMGS=\"+919114151322\"\r"); // Replace x with mobile number
  delay(1000);
  SIM900.println("HELP SMITA!");
  SIM900.println("GMAPS:");
  SIM900.println(msg);// The SMS text you want to send
  SIM900.println("Her Location will be sent to you Frequently. Please Follow!");
  delay(100);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("LOCATION SENT");

  lcd.print("Message sent!");




    SIM900.println( F("AT+SAPBR=3,1,\"Contype\",\"GPRS\"") );
    delay(1000);

    SIM900.println( F("AT+SAPBR=3,1,\"APN\",\"Vodafone\"") );
    delay(1000);
    
    SIM900.println( F("AT+SAPBR=1,1") );
    delay(3500);
    
    SIM900.println( F("AT+SAPBR=2,1") );
    delay(3500); 
  
    SIM900.println( F("AT+HTTPTERM") );
    delay(1000);
   
    SIM900.println( F("AT+HTTPINIT") );
    delay(1000);
    
    SIM900.println( F("AT+HTTPPARA=\"CID\",1") );
    delay(1000);
  
    SIM900.println( F("AT+HTTPPARA=\"URL\",\"your.domain/server.php?latitude=21.49&longitude=83.90\"") );
    delay(1000);

  
    SIM900.println( F("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"") );
    delay(1000);
    SIM900.print  ( strlen(msg) );
    SIM900.print  ( F( "AT+HTTPDATA=") );
    SIM900.print  ( strlen(msg) );
    SIM900.println( F(",2000") );
    delay(1000);  

    
    
    Serial.println( SIM900.println(msg) );
    delay(1000); 
    
    Serial.println(SIM900.println( F("AT+HTTPACTION=1") ));
    delay(5000); 


    
}


void SIM900STOP()
{
  SIM900.println("AT+HTTPTERM");
  delay(1000);
}

void ShowSerialData()
{
  while(SIM900.available()!=0)
    Serial.write(SIM900.read());
}
