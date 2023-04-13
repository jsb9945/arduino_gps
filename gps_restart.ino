#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <stdio.h>

SoftwareSerial gpsSerial(D5, D6);
char gps_string[200];
String str = "GNGGA";
const char* ssid = "iptime2";
const char* password = "control505!";
int num = 0;
char gps_raw[] = "$GNGGA,064422.00,3732.52240,N,12704.79333,E,1,05,2.70,42.6,M,18.7,M,,*6D";
char gps_raw2[] = "GNGGA,064422.00,3732.52240,N,12704.79333,E,1,05,2.70,42.6,M,18.7,M,,*6D";

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

  //if (currentMillis - previousMillis >= 1000) {

    if (gpsSerial.available() > 0) {
      char a = gpsSerial.read();
      //char a = gps_raw[0]; // test 코드
      if(a=='$'){
        String gngga = gpsSerial.readStringUntil('\n');
        //String gngga = gps_raw2;
        
        if(gngga.substring(0,5).equals(str)){
          //Serial.println(gngga);
          //gngga.toCharArray(gps_string, gngga.length());
          //Serial.println(gps_string);

          gngga.replace(" ", ""); // 빈칸 제거
          Serial.println(gngga);
  
          char* token;  // 분리된 문자열을 저장할 포인터 변수
          int index = 0;  // 토큰 인덱스
          
          while ((token = strtok(index == 0 ? gngga.begin() : NULL, ",")) != NULL) {
            //Serial.print(token);  // 토큰 출력
            //Serial.print('\t');

              switch(index){
                case 0: case 1: case 2: case 3: case 4: case 5: case 6:
                  client.write(token);
                  client.write("\t");
                  index++;
                  break;  
                default:
                  index++; 
                  break;       
              }              
           
  
          }
          client.write("\n");
        }
      }
    //}

    //previousMillis = currentMillis;
  }
}
