/*
   TimeNTP_ESP8266WiFi.ino
   Example showing time sync to NTP time source

   This sketch uses the ESP8266WiFi library
*/
#include <WEMOS_Matrix_LED.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

MLED mled(5); //set intensity=5

const char ssid[] = "ssid";  //  your network SSID (name)
const char pass[] = "password";       // your network password

int sunit, munit, hunit, valm = 0, valh = 0, ledstats, i;

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

const int timeZone = 2;     // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
//const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)


WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

void setup()
{
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
  delay(250);
  Serial.println("TimeNTP Example");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  for (int i=0;i<8;i++){
  mled.dot(0, i);
  mled.display();
  delay(150);
  }
  

  for (int i=7;i>-1;i--){
  mled.dot(7, i);
  mled.display();
  delay(150);
  }
  
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()
{
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();
    }
  }


}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();

  sunit = second() % 10;
  munit = minute() % 10; //sets the variable munit and hunit for the unit digits
  hunit = hour() % 10;

  //seconds units
  if (sunit == 1 || sunit == 3 || sunit == 5 || sunit == 7 || sunit == 9) {
    mled.dot(1, 0);
    mled.display();
  } else {
    mled.dot(1, 0, 0);
    mled.display();
  }
  if (sunit == 2 || sunit == 3 || sunit == 6 || sunit == 7) {
    mled.dot(2, 0);
    mled.display();
  } else {
    mled.dot(2, 0, 0);
    mled.display();
  }
  if (sunit == 4 || sunit == 5 || sunit == 6 || sunit == 7) {
    mled.dot(3, 0);
    mled.display();
  } else {
    mled.dot(3, 0, 0);
    mled.display();
  }
  if (sunit == 8 || sunit == 9) {
    mled.dot(4, 0);
    mled.display();
  } else {
    mled.dot(4, 0, 0);
    mled.display();
  }

  //seconds
  if ((second() >= 10 && second() < 20) || (second() >= 30 && second() < 40) || (second() >= 50 && second() < 60))  {
    mled.dot(1, 1);
    mled.display();
  } else {
    mled.dot(1, 1, 0);
    mled.display();
  }
  if (second() >= 20 && second() < 40)  {
    mled.dot(2, 1);
    mled.display();
  } else {
    mled.dot(2, 1, 0);
    mled.display();
  }
  if (second() >= 40 && second() < 60) {
    mled.dot(3, 1);
    mled.display();
  } else {
    mled.dot(3, 1, 0);
    mled.display();
  }

  //minutes units
  if (munit == 1 || munit == 3 || munit == 5 || munit == 7 || munit == 9) {
    mled.dot(1, 3);
    mled.display();
  } else {
    mled.dot(1, 3, 0);
    mled.display();
  }
  if (munit == 2 || munit == 3 || munit == 6 || munit == 7) {
    mled.dot(2, 3);
    mled.display();
  } else {
    mled.dot(2, 3, 0);
    mled.display();
  }
  if (munit == 4 || munit == 5 || munit == 6 || munit == 7) {
    mled.dot(3, 3);
    mled.display();
  } else {
    mled.dot(3, 3, 0);
    mled.display();
  }
  if (munit == 8 || munit == 9) {
    mled.dot(4, 3);
    mled.display();
  } else {
    mled.dot(4, 3, 0);
    mled.display();
  }

  //minutes
  if ((minute() >= 10 && minute() < 20) || (minute() >= 30 && minute() < 40) || (minute() >= 50 && minute() < 60))  {
    mled.dot(1, 4);
    mled.display();
  } else {
    mled.dot(1, 4, 0);
    mled.display();
  }
  if (minute() >= 20 && minute() < 40)  {
    mled.dot(2, 4);
    mled.display();
  } else {
    mled.dot(2, 4, 0);
    mled.display();
  }
  if (minute() >= 40 && minute() < 60) {
    mled.dot(3, 4);
    mled.display();
  } else {
    mled.dot(3, 4, 0);
    mled.display();
  }

  //hour units
  if (hunit == 1 || hunit == 3 || hunit == 5 || hunit == 7 || hunit == 9) {
    mled.dot(1, 6);
    mled.display();
  } else {
    mled.dot(1, 6, 0);
    mled.display();
  }
  if (hunit == 2 || hunit == 3 || hunit == 6 || hunit == 7) {
    mled.dot(2, 6);
    mled.display();
  } else {
    mled.dot(2, 6, 0);
    mled.display();
  }
  if (hunit == 4 || hunit == 5 || hunit == 6 || hunit == 7) {
    mled.dot(3, 6);
    mled.display();
  } else {
    mled.dot(3, 6, 0);
    mled.display();
  }
  if (hunit == 8 || hunit == 9) {
    mled.dot(4, 6);
    mled.display();
  } else {
    mled.dot(4, 6, 0);
    mled.display();
  }

  //hour
  if (hour() >= 10 && hour() < 20)  {
    mled.dot(1, 7);
    mled.display();
  } else {
    mled.dot(1, 7, 0);
    mled.display();
  }
  if (hour() >= 20 && hour() < 24)  {
    mled.dot(2, 7);
    mled.intensity=0;
    mled.display();
  } else {
    mled.dot(2, 7, 0);
    mled.intensity=8;
    mled.display();
  }
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  //  Serial.print(":");
  //  if (digits < 10)
  //    Serial.print('0');
  //  Serial.print(digits);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
