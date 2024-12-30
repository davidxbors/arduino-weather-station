#include <LiquidCrystal.h>
#include <DHT.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int dht_sensor_pin = 13;
#define DHTTYPE DHT11
DHT tmp_sensor(dht_sensor_pin, DHTTYPE);

const int mq_135_pin = 0;

void setup() {
  // init serial
  Serial.begin(9600);
  Serial.println("Weather Station initialising...");

  // init lcd
  lcd.begin(16, 2);
  lcd.print("Weather Station");

  // init dht
  tmp_sensor.begin();
}

void tmp_exit_procedure() {
  for (;;) {
    Serial.println("Temperature sensor has a problem...");
    delay(5000);
  }
}

void log_values(float tmpc, float hum, float hic, int airq) {
  // log in serial all recorded values
  Serial.print("Temperature: ");
  Serial.println(tmpc);

  if (tmpc > 25.00) {
    Serial.println("It's hot!!!");
  }

  Serial.print("Humidity: ");
  Serial.println(hum);

  Serial.print("Heat Index: ");
  Serial.println(hic);

  Serial.print("Air quality: ");
  Serial.println(airq);
}

void display_to_lcd(float tmpc, float hum, float hic, int airq) {
  // weather station message + tmp message
  lcd.setCursor(0, 1);
  lcd.print("Tmp:");
  lcd.print(tmpc);
  lcd.print("       ");
  delay(2000);
  // scroll one row
  lcd.setCursor(0, 0);
  lcd.print("Tmp: ");
  lcd.print(tmpc);
  lcd.print("       ");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(hum);
  delay(2000);
  // scroll one row
  lcd.setCursor(0, 0);
  lcd.print("Hum: ");
  lcd.print(hum);
  lcd.setCursor(0, 1);
  lcd.print("Heat index: ");
  lcd.print(hic);
  delay(2000);
  // scroll one row
  lcd.setCursor(0, 0);
  lcd.print("Heat index: ");
  lcd.print(hic);
  lcd.setCursor(0, 1);
  lcd.print("AirQ: ");
  if (airq < 300) {
    lcd.print("Good");
  } else if (airq < 550) {
    lcd.print("OK!!");
  } else if (airq < 700) {
    lcd.print("ALC!");
  } else {
    lcd.print("GAS!");
  }
  lcd.print("       ");
  delay(2000);
}

void loop() {
  // read temp in celsius
  float tmpc = tmp_sensor.readTemperature();
  // read humidity
  float hum = tmp_sensor.readHumidity();

  // check if any read failed
  if (isnan(hum) || isnan(tmpc))
    tmp_exit_procedure();

  // compute heat index
  float hic = tmp_sensor.computeHeatIndex(tmpc, hum, false);

  // read air quality
  int airq = analogRead(mq_135_pin);

  // log all recorded values
  log_values(tmpc, hum, hic, airq);

  // output values to lcd display
  display_to_lcd(tmpc, hum, hic, airq);
}
