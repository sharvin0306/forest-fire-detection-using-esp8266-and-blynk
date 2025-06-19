#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Blynk Configuration (using your notification template)
#define BLYNK_TEMPLATE_ID "YOUR blynk Tempelate ID"
#define BLYNK_TEMPLATE_NAME "you tempelate name"
#define BLYNK_AUTH_TOKEN "your blynk AUTH TOKEN"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WIFI SSID";  
char pass[] = "WIFI PASSWORD"; 

BlynkTimer timer;
int flag = 0;

#define MQ2 A0
#define GREEN D5
#define RED D6
#define buzzer D7
int MQ2_Val = 0;

WidgetLED led1(V1);
WidgetLED led2(V3);

void sendSensor() {
  MQ2_Val = analogRead(MQ2);
  
  if (MQ2_Val > 125 && flag == 0) {  // Changed threshold to 125 as requested
    Serial.println("Gas Detected!");  
    Blynk.logEvent("gas_alert","Danger! Gas Level: " + String(MQ2_Val));
    
    // Visual and sound alerts
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(buzzer, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("     Alert!    ");
    led1.on();
    led2.off();
    
    flag = 1;
  }
  else if (MQ2_Val <= 112) {
    flag = 0;
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print("     Normal    ");
    led1.off();
    led2.on();
  }

  // Update display and Blynk
  lcd.setCursor(0, 0);
  lcd.print("Gas Level: ");
  lcd.print(MQ2_Val);
  lcd.print("   ");
  
  Blynk.virtualWrite(V0, MQ2_Val);
  Blynk.virtualWrite(V2, MQ2_Val);
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  Welcome To");
  lcd.setCursor(0, 1);
  lcd.print("Gas Alert System");
  delay(3000);
  lcd.clear();
  
  Serial.begin(9600);  
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  Blynk.begin(auth, ssid, pass); 
  delay(2000);
  timer.setInterval(1000L, sendSensor);  // Check every second
}

void loop() {
  Blynk.run();
  timer.run();
}