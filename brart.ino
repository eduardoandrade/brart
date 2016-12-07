#include <EmonLib.h>		//é preciso baixar esta biblioteca
#include <SPI.h>		
#include <WiFi.h>		

EnergyMonitor emon1;
const int CT_PIN = 0;

char ssid[] = "myNetwork"; 
char pass[] = "myPassword";
byte mac[]  = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6}; //pode ser qualquer endereço MAC válido, desde que seja diferente do restante dos dispositivos na sua LAN
WiFi client;

int status = WL_IDLE_STATUS;
IPAddress server(74,125,115,105);

void setup()
{ 
  Serial.begin(9600);

  emon1.current(CT_PIN, 64);

  Serial.println("Attempting to get a WiFi connection…”);
  Serial.print("SSID: ");
  Serial.println(ssid);

  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a WiFi connection");
    // don't do anything else:
    while(true);
  } 
  else {
    Serial.println("Connected to WiFi");
  }

	delay(1000);
	Serial.println(“Starting measurement: \n");
}

void sendData(double Irms, double Power) {
  
  client.connect(server,80);

  delay(500);
  if (client.connected()) {
    client.print("GET /measure?”);
    client.print("irms=");
    client.print(Irms);
    client.print("&power=");
    client.print(Power);
    client.println(" HTTP/1.1");
    client.println("Host: 74,125,115,105");
    client.println();
    delay(500);


    while (client.available()) {
      char c = client.read();
    }

    delay(100);
    client.stop();
    Serial.println("Data sent”);
  } else {
    Serial.println(“Couldn't connect”);
  }
}


void loop() {

  double Irms = emon1.calcIrms(1480);
  double Power = Irms * 127.0;
  Serial.print("Irms: ");
  Serial.print(Irms);
  Serial.print(" A \t");
  Serial.print("Power: ");
  Serial.print(Power);
  Serial.print(" W");
  Serial.println();

  sendData(Irms, Power);

  delay(1000);

}
