#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library for LCD
// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,16,2) for 16x2 LCD.

/* SDA LCD A4, SCL A5
 *  D12 -> GPIO2 esp
 *  D3 -> GPIO0 (INGET KASI VOLTAGE DIVIDER, BIAR 5V NYA ARDUINO JADI 3.3v, inget pas booting jgn langsung di pasang biar ga masuk mode program)
 *  D2 -> servo
 *  D13 -> push button (1 pin push buttn ke D13, lagi 1 ke 5v arduino, terus D13 ini kasi pull down resistor 10K (sambung resistor di D13 -> ground)
 */

Servo servo;
Servo servo2;
int angle = 90; // angle closed
int angle2 = 180; // angle opened
int angle3 = 90;
int angle4 = 0;
int pushButton = 13; 
int espGate = 12; //to gpio2 esp
int espGateState; //gate state dari server
int espGateStateLast;
int buttonState;
int start=1; 

void writeLCD(int writeTrue);

void setup() {
  servo.attach(2); //D2 ke signal servo
  servo2.attach(4);
  pinMode(3, OUTPUT); // D3 to gpio0 esp, biar esp nya aware state garasi di local
  
  /* biar semisal garasinya kebuka blm sempurna dan tiba tiba sistem mati, 
  jadi dia either lanjut nge buka sampe full/di tutup sampe full */
//  if (servo.read() > 120) { 
//    servo.write(angle2);
//  }
//  else {
//    servo.write(angle);
//  }
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  buttonState = digitalRead(pushButton); // baca state push button nya
  if (buttonState == HIGH) {
    digitalWrite(3, HIGH); //pull up pin D3 untuk ngasi tau esp bahwa state garasi di local di ubah manual sama tombolnya
    lcd.clear();
    if (espGateState == LOW) { //kondisi untuk tampilan lcd, klo last nya low kan lagi tutup berarti, jadi selanjutnya buka
      lcd.setCursor(0,0);
      lcd.print("Opening garage");
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("Closing gate");
    }
    lcd.setCursor(0,1);
    lcd.print("Please wait");
    
    delay(3000); // delay 2 detik an, tujuannya biar esp nya bisa baca, biar ga langsung di pull low lagi
    digitalWrite(3, LOW); // abis 3 detik, baru pull low
  }
  
  espGateState = digitalRead(espGate); // baca state pin GPIO2 di esp, jadi ini nge baca state garasi di server
  Serial.println(espGateState);
  if(start == 1 || espGateState != espGateStateLast) { // bandingin statenya, klo ada perubahan state baru masuk sini
    start = 0;
    if (espGateState == HIGH ) {
      for (int i=angle; i<=angle2; i++) {
        servo.write(i); //open
        delay(4);
      }
      
      for (int i=angle3; i>=angle4; i--) {
        servo2.write(i); //open
        delay(4);
      }

      writeLCD(1);
    }
    else if (espGateState == LOW) {
      for (int i=angle2; i>=angle; i--) {
        servo.write(i); //close
        delay(4);
      }

      for (int i=angle4; i<=angle3; i++) {
        servo2.write(i); //close
        delay(4);
      }

      writeLCD(0);
    }
    espGateStateLast = espGateState; // set last state nya jadi state yg terbaru
    delay(500);
  }
}
void writeLCD(int writeTrue){
  if(writeTrue){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Garasi terbuka");
  }
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Garasi tertutup");
  }
}
