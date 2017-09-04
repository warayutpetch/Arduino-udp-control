#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <DS1307.h>
#include <LiquidCrystal_I2C.h>

DS1307 clock;
RTCDateTime dt;
LiquidCrystal_I2C lcd(0x20, 16, 2);
const char* ssid = "Pump Project";
const char* password = "12345678";
//const char* host = "192.168.0.1";
char message[255];
unsigned int localPort = 8080;
const byte wifi_ip[4] = {192, 168, 1, 2}; //config ipaddress of access point
const byte wifi_subnet[4] = {255, 255, 255, 0}; //config sumnet mask of access point
const byte wifi_gateway[4] = {192, 168, 1, 1}; //config gateway of access point

WiFiUDP Udp;

char m = 0;
int H = 0, M = 0, S = 0, H_off = 0, M_off = 0 , S_off = 0;
int chk = 0;

void setup() {
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(
    IPAddress(wifi_ip[0], wifi_ip[1], wifi_ip[2], wifi_ip[3]),
    IPAddress(wifi_gateway[0], wifi_gateway[1], wifi_gateway[2], wifi_gateway[3]),
    IPAddress(wifi_subnet[0], wifi_subnet[1], wifi_subnet[2], wifi_subnet[3])
  );

  pinMode(0, OUTPUT);
  digitalWrite(0, 0);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //pinMode(14, OUTPUT);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0, 0);
  lcd.print("PumpProject WIFI");
  lcd.setCursor(0, 1);
  lcd.print("192.168.1.2");
  Udp.begin(localPort);
  Serial.println("Setup done");
  delay(4000);
  lcd.clear();
  Serial.println("Initialize DS1307");;
  clock.begin();
     //clock.setDateTime(2016,12,12,20,36, 00);

  // If date not set
  if (!clock.isReady())
  {
    // Set sketch compiling time
    clock.setDateTime(__DATE__, __TIME__);
  }
  Serial.println("All Ready !");
  lcd.print("All OK!");
  delay(1000);
  lcd.clear();
}

void loop() {
  dt = clock.getDateTime();
  Showtime ();
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    //     IPAddress remoteIp = Udp.remoteIP();
    int len = Udp.read(message, 255);
    Serial.println(message);
    m = *message;
    checkcommand();
    delay(5);
    //  m = 's';
  }

  else if (chk == 3) {
    ChkOnOff();
  }

}


void checkcommand()
{ if (m == '7') {
    lcd.clear();
    delay(1000);
    digitalWrite(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("ON");
  }
  if (m == 'y') {
    delay(1000);
    digitalWrite(0, 0);
    lcd.clear();
    H = 0;
    M = 0;
    H_off = 0;
    M_off = 0;
    lcd.setCursor(0, 1);
    lcd.print("OFF");
  }

  if (m == 'O') {
    lcd.setCursor(9, 0);
    lcd.print("ON  ");
    lcd.setCursor(0, 1);
    lcd.print("00:00");
    chk = 1;
  }
  if (m == 'F') {
    lcd.setCursor(9, 0);
    lcd.print("OFF ");
    lcd.setCursor(8, 1);
    lcd.print("00:00");
    chk = 2;
  }
  if (chk == 1) {
    lcd.setCursor(0, 1);
    if (m == 'H') {
      H++;
      if (H  <= 9) {
        lcd.print("0");
        lcd.print(H);
        lcd.print(":");

      }
      if (H == 24) {
        H = 0;
        lcd.print("0");
        lcd.print(H);
        lcd.print(":");
      }
      else if (H > 9) {
        lcd.print(H);
        lcd.print(":");
      }
    }
    else if (m == 'M') {
      lcd.setCursor(3, 1);
      M++;
      if (M <= 9) {
        lcd.print("0");
        lcd.print(M);
      }
      if (M == 60) {
        M = 0;
        lcd.print("0");
        lcd.print(M);
      }
      else if (M > 9) {
        lcd.print(M);
      }

    }

    if (m == 'h') {
      lcd.setCursor(0, 1);
      H--;
      Serial.println(H);
      if (H  == -1) {
        H = 23;
        lcd.print(H);
      }
      if (H < 23 && H >= 10) {
        lcd.print(H);

      }
      if (H <= 9) {
        lcd.print("0");
        lcd.print(H);
      }
    }


    else if (m == 'm') {
      lcd.setCursor(3, 1);
      M--;
      if (M  == -1) {
        M = 59;
        lcd.print(M);
      }
      if (M < 59 && M >= 10) {
        lcd.print(M);

      }
      if (M <= 9) {
        lcd.print("0");
        lcd.print(M);
      }

      //    else if (m == 'S') {
      //
      //      lcd.setCursor(6, 1);
      //      S++;
      //      if (S <= 9) {
      //        lcd.print("0");
      //        lcd.print(S);
      //        lcd.print(":");
      //      }
      //      if (S == 60) {
      //        S = 0;
      //        lcd.print("0");
      //        lcd.print(S);
      //        lcd.print(":");
      //      }
      //      else if (S > 9) {
      //        lcd.print(S);
      //        lcd.print(":");
      //      }
      //    }
    }
  }
  //-------------------------------------------------------------------------------

  if (chk == 2) {
    lcd.setCursor(8, 1);
    if (m == 'H') {
      H_off++;
      if (H_off  <= 9) {
        lcd.print("0");
        lcd.print(H_off);
        lcd.print(":");

      }
      if (H_off == 24) {
        H_off = 0;
        lcd.print("0");
        lcd.print(H_off);
        lcd.print(":");
      }
      else if (H_off > 9) {
        lcd.print(H_off);
        lcd.print(":");
      }
    }
    else if (m == 'M') {
      lcd.setCursor(11, 1);
      M_off++;
      if (M_off <= 9) {
        lcd.print("0");
        lcd.print(M_off);
      }
      if (M_off == 60) {
        M_off = 0;
        lcd.print("0");
        lcd.print(M_off);
      }
      else if (M_off > 9) {
        lcd.print(M_off);
      }
    }

    if (m == 'h') {
      lcd.setCursor(8, 1);
      H_off--;
      Serial.println(H_off);
      if (H_off  == -1) {
        H_off = 23;
        lcd.print(H_off);
      }
      if (H_off < 23 && H_off >= 10) {
        lcd.print(H_off);

      }
      if (H_off <= 9) {
        lcd.print("0");
        lcd.print(H_off);
      }
    }


    else if (m == 'm') {
      lcd.setCursor(11, 1);
      M_off--;
      if (M_off  == -1) {
        M_off = 59;
        lcd.print(M_off);
      }
      if (M_off < 59 && M_off >= 10) {
        lcd.print(M_off);

      }
      if (M_off <= 9) {
        lcd.print("0");
        lcd.print(M_off);
      }
      //    else if (m == 'S') {
      //
      //      lcd.setCursor(14, 1);
      //      S_off++;
      //      if (S_off <= 9) {
      //        lcd.print("0");
      //        lcd.print(S_off);
      //        lcd.print(":");
      //      }
      //      if (S_off == 60) {
      //        S = 0;
      //        lcd.print("0");
      //        lcd.print(S);
      //        lcd.print(":");
      //      }
      //      else if (S_off > 9) {
      //        lcd.print(S_off);
      //        lcd.print(":");
      //      }
      //    }
    }

    if (m == '3') {
      chk = 3;
      lcd.setCursor(9, 0);
      lcd.print("Set!");
    }

  }
}


void Showtime () {
  lcd.setCursor(0, 0);
  if (dt.hour < 9) {
    lcd.print("0");
    lcd.print(dt.hour);
    lcd.print(":");
    if (dt.minute < 9) {
      lcd.print("0");
      lcd.print(dt.minute);
      lcd.print(":");
      if (dt.second <= 9) {
        lcd.print("0");
        lcd.print(dt.second);
      }
      else if (dt.second > 9) {
        lcd.print(dt.second);
      }
    }
    else if (dt.minute > 9) {
      lcd.print(dt.minute);
      lcd.print(":");
      if (dt.second <= 9) {
        lcd.print("0");
        lcd.print(dt.second);
      }
      else if (dt.second > 9) {
        lcd.print(dt.second);

      }
    }
  }
  else if (dt.hour > 9) {
    lcd.print(dt.hour);
    lcd.print(":");
    if (dt.minute < 9) {
      lcd.print("0");
      lcd.print(dt.minute);
      lcd.print(":");
      if (dt.second <= 9) {
        lcd.print("0");
        lcd.print(dt.second);
      }
      else if (dt.second > 9) {
        lcd.print(dt.second);

      }
    }
    else if (dt.minute > 9) {
      lcd.print(dt.minute); lcd.print(":");
      if (dt.second <= 9) {
        lcd.print("0");
        lcd.print(dt.second);
      }
      else if (dt.second > 9) {
        lcd.print(dt.second);

      }
    }
  }
}

void ChkOnOff() {
  if (dt.hour == H && dt.minute == M) {
    lcd.setCursor(9, 0);
    lcd.print("ON  ");
    delay(1000);
    digitalWrite(0, 1);
  }
  if (dt.hour == H_off && dt.minute == M_off) {
    lcd.setCursor(9, 0);
    lcd.print("OFF ");
    delay(500);
    digitalWrite(0, 0);
    lcd.clear();
    H = 0;
    M = 0;
    H_off = 0;
    M_off = 0;
  }

}

