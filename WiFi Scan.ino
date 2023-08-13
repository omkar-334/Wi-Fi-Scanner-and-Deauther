#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

int totalColumns = 16;
int totalRows = 2;
LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows);  

void scrollMessage(int row, String message, int delayTime, int totalColumns) {
  for (int i=0; i < totalColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int position = 0; position < message.length(); position++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(position, position + totalColumns));
    delay(delayTime);
  }
}
int signal_dBM[] = { -100, -99, -98, -97, -96, -95, -94, -93, -92, -91, -90, -89, -88, -87, -86, -85, -84, -83, -82, -81, -80, -79, -78, -77, -76, -75, -74, -73, -72, -71, -70, -69, -68, -67, -66, -65, -64, -63, -62, -61, -60, -59, -58, -57, -56, -55, -54, -53, -52, -51, -50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40, -39, -38, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24, -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1};
int signal_percent[] = {0, 0, 0, 0, 0, 0, 4, 6, 8, 11, 13, 15, 17, 19, 21, 23, 26, 28, 30, 32, 34, 35, 37, 39, 41, 43, 45, 46, 48, 50, 52, 53, 55, 56, 58, 59, 61, 62, 64, 65, 67, 68, 69, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 90, 91, 92, 93, 93, 94, 95, 95, 96, 96, 97, 97, 98, 98, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
int strength =0;
int percentage =0;
void setup(){
  lcd.init();          
  lcd.backlight();
  Serial.begin(115200);
  Serial.println("");


  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  //ESP has tendency to store old SSID and PASSword and tries to connect
  delay(100);

  Serial.println("WiFi Network Scan Started");
}

void loop(){

  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  
  if (n == 0){ 
    Serial.println("No Networks Found");
    lcd.setCursor(0, 0);
    lcd.print("No Networks Found");}
  else
  {
    lcd.setCursor(0, 0);
    lcd.print(n);
    lcd.println(" Networks found");
    String message,adder;
    message="";
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      // SSID - Servece Set Identifier - Network name
      // RSSI - Received Signal Strength Indicator 
      Serial.print(": ");
      Serial.print(i + 1);  //Sr. No
      Serial.print(" (");
      Serial.print(WiFi.SSID(i));
      adder=WiFi.SSID(i);
      message.concat(adder);
      message.concat("---");
      strength =WiFi.RSSI(i);
      Serial.print(" - ");
      for (int x = 0; x < 100; x = x + 1) {
      if (signal_dBM[x] == strength) {
        // Print the received signal strength in percentage
        percentage = signal_percent[x];
        Serial.print(percentage);
        Serial.print("%");
        break;
      }
    }
      Serial.print(") MAC:");
      Serial.print(WiFi.BSSIDstr(i));
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" Unsecured":" Secured");
      delay(10);
    }
    scrollMessage(1, message, 400, totalColumns);
  }
  Serial.println("");
  // Wait a bit before starting New scanning again
}