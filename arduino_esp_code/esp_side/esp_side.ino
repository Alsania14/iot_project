#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

int gateState;
int gateLastState;
int waitForWifiConnect;
int wifiLastStatus;

/* GPIO0 -> D3 (INGET KASI VOLTAGE DIVIDER, BIAR 5V NYA ARDUINO JADI 3.3v, KARENA INI BAKALAN NGE BACA SIGNAL DARI ARDUINO NYA)
 * GPIO2 -> D12 (kalo yang ini ga perlu, soalnya ini ngirim signal ke arduino nya)
 * vcc -> sumber 3.3v
 * gnd -> gnd (inget buatin common ground, klo keduanya make supply yg beda)
 * 
 * 
 */ 

void setup() {
  Serial.begin(115200);            
  WiFi.begin("Laravel 2", "pENgUKuh@PeGUyanGAn?!421060");   
  pinMode(2, OUTPUT); // set gpio2 output untuk ngasi tau state garasi di server ke arduino
  pinMode(0, INPUT); // set gpio0 output untuk ngasi tau state garasi di server ke arduino
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Waiting for connection");
    waitForWifiConnect++;
    if (waitForWifiConnect == 20){ //klo dh 20x nge retry, keluar dari loop
      break;
    }
  }
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) { 
    if (wifiLastStatus != 0) { // kondisi untuk semisal koneksinya putus, utk mastiin server sama local nya sync
        wifiLastStatus = 0;
          if (gateLastState == 0) { //yg di cek last state nya, klo last nya close, di server post untuk close, sebaliknya buat yg open jg
            HTTPClient http;
            http.begin("http://iot.alindeveloper.xyz/api/closegate");      
            http.POST("");  
            http.end();
          }
          else {
            HTTPClient http;   
            http.begin("http://iot.alindeveloper.xyz/api/opengate");      
            http.POST("");  
            http.end();
          }
        }
     
     HTTPClient http;   
     http.begin("http://iot.alindeveloper.xyz/api/readgate"); // baca state garasi di server
     int httpCode = http.POST("");  
     String payload = http.getString();

    if (httpCode == 200) { // pastiin respon nya 200
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload); // parse body dari hasil post readgate di servernya
      gateState = root["state_right_now"];
     
      Serial.print("http response: ");
      Serial.println(httpCode);  
      Serial.print("gate state from server: ");
      Serial.println(gateState);

      if (gateState == 1) {
        digitalWrite(2, HIGH); // klo readgate nya 1, pull pin gpio2 high, jadi ntar ini yg buat ngasi tau arduino nya state garasi di servernya
      }
      else if (gateState == 0) {
        digitalWrite(2, LOW);
      }
      gateLastState = gateState; // set last state nya jadi yg di server
    }
    else {
      Serial.println("Not 200 response from server");
      Serial.print("http response: ");
      Serial.println(httpCode);
    }
    http.end();
  } 
  
  else {
    wifiLastStatus = 1; //semisal wifinya dc, set ini jadi 1, biar bisa masuk ke yg paling pertama itu, mastiin kondisi di server sync saama yg di local
    Serial.println("Error in WiFi connection");
  }

  Serial.print("Button pressed: ");
  Serial.println(digitalRead(0)); // nge read state pin D3 di arduino, tujuannya biar esp nya juga aware sama kondisi garasi di localnya
   if (digitalRead(0) == HIGH) { 
    if (gateState == 1 || gateLastState == 1){ // make 2 gini tujuannya klo cmn gateState yg di pake kan misal putus ini ga akan berubah, jadi last nya di pake jg
      if (WiFi.status() != WL_CONNECTED) {
        digitalWrite(2, LOW); // klo misal ga ada koneksi, ubah state gpio2 nya, misal sebelumnya 1 (HIGH atau open), sekarang ubah jadi 0 (LOW atau close)
      }
      else { // klo ada koneksi, ubah state di servernya jg, klo yg ini kan dari open -> close, jadi close gate nya
        HTTPClient http; 
        http.begin("http://iot.alindeveloper.xyz/api/closegate");      
        http.POST("");  
        http.end();
      }
      gateLastState = 0; // set last state nya jadi close
    }

    else if (gateState == 0 || gateLastState == 0) {
       if (WiFi.status() != WL_CONNECTED) {
        digitalWrite(2, HIGH);
      }
      else {
        HTTPClient http;   
        http.begin("http://iot.alindeveloper.xyz/api/opengate");      
        http.POST("");  
        http.end();
      }
      gateLastState = 1;
    }
  }
  Serial.print("Gate state: ");
  Serial.println(gateState);
  Serial.print("Gate last state: ");
  Serial.println(gateLastState);
  Serial.println("=========================");
  delay(2000);  
}
