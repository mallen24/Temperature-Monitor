#include <Process.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MAX31865.h>

LiquidCrystal_I2C lcd(0x27,16,2);  

Adafruit_MAX31865 thermo = Adafruit_MAX31865(10, 11, 12, 13);

#define RREF      430.0
#define RNOMINAL  100.0

void setup() {
  Bridge.begin();
  Serial.begin(9600); 
  lcd.begin();                    // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print(" Turning On ");
  lcd.setCursor(0,1);
  lcd.print(" ... ");
  delay(500);
  lcd.clear();
  thermo.begin(MAX31865_3WIRE);  // set to 2WIRE 3WIRE or 4WIRE as necessary
}

void loop() {
  float temp = thermo.temperature(RNOMINAL, RREF);
  temp = 1.8 * temp + 32;
  
    Process p;
    //http://192.168.1.253/jobpoint?key=temp&stage=c&value=
    char cmd[] = "";
    char tempstr[4];
    dtostrf(temp, -3, 2, tempstr);
    char fullcmd[150];
    strcpy(fullcmd, strcat(cmd, tempstr));
    char endc[] = "";
    Serial.println(strcat(fullcmd, endc));
    p.begin("curl");     
    p.addParameter(strcat(fullcmd, endc));
    p.run();
    while (p.available()>0) {
      char c = p.read();
      Serial.print(c);
    }
    Serial.flush();  

    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print("Temp:");
    lcd.print(temp,1);
    lcd.print((char) 223);
    lcd.print("F");
    delay(30000);
}