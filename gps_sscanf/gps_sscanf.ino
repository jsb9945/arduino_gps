#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <stdio.h>

SoftwareSerial gpsSerial(D5, D6);

char gps_string[500];
char cnt = 0;
char checksum_cnt = 0;
int gga = 0;          // header GPGGA
char cunt = 0;
char header[6];
float gtime;
float lat;
char north_south;
float lon;
char east_west;
int fix;
uint16_t nsatellite;
float hdop;
float alti;
char m1;
float wgs84;
char m2;
int Receive_state = 0; //0: header, 1: Data, 2: checksum
String str = "GPGGA";
const char* ssid = "iptime2";
const char* password = "control505!";
char gps_raw[] = "$GPGGA,064422.00,3732.52240,N,12704.79333,E,1,05,2.70,42.6,M,18.7,M,,*6D";
char gps_raw2[] = "GPGGA,064422.00,3732.52240,N,12704.79333,E,1,05,2.70,42.6,M,18.7,M,,*6D";
int num = 0;
char message[100];
WiFiClient client;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  
  int n = WiFi.scanNetworks();
  Serial.println("Scan Done");
  if(n==0){
    Serial.println("no networks found");
  }
  else{
    Serial.print(n);
    Serial.println("  networks found");
  }

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conncected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("connection Start");

  if(!client.connect("192.168.0.2", 3000)){
    Serial.println("connection failed");
    delay(500);
  }  
}

void loop() {
  
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) { // test코드

    if (gpsSerial.available() > 0) {
      //char a = gpsSerial.read();
      char a = gps_raw[0]; // test 코드
      if(a=='$'){
        //String gngga = gpsSerial.readStringUntil('\n');
        String gngga = gps_raw2; // test 코드
        
        if(gngga.substring(0,5).equals(str)){
          //Serial.println(gngga);
          //gngga.toCharArray(gps_string, gngga.length());
          //Serial.println(gps_string);

          gngga.replace(" ", ""); // 빈칸 제거
          //Serial.println(gngga);
  
          gngga.toCharArray(gps_string, gngga.length());
          sscanf(gps_string, "%5c,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c,%f,%c",
            &header, &gtime, &lat, &north_south, &lon, &east_west, &fix, &nsatellite,
            &hdop, &alti, &m1, &wgs84, &m2);
          
          if(fix==1){
            memset(message, 0, sizeof(message));
            sprintf(message, "%f, %c, %f, %c, %d", lat, north_south, lon, east_west, fix);
            client.write(message);
            client.write('\n');
          }
          else if(fix==0){
            Serial.println(0);
          }
          
        }
      }
    } // test 코드

    previousMillis = currentMillis; // test 코드
  }

}
