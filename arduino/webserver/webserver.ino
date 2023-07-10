#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x88, 0x47};
//IPAddress ip(192,168,178,35);
IPAddress ip(192, 168, 1, 12);

EthernetServer server(80);  // port 80 is default for HTTP

int sensorpin = A0;
int sensorpower = 8;
int LED1 = 2;
int LED2 = 3;
int LED3 = 4;
int pumppin = 11;

// variable for sensor reading
int sensor;

// delay time between sensor readings (milliseconds)
const int delayTime = 1; 

// "wet" and "dry" thresholds - these require calibration
int wetness = 0;
int wet = 800;
int dry = 500;

void setup() 
{

  Serial.begin(9600);

  // Start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(sensorpower,OUTPUT);
  pinMode(pumppin,OUTPUT);

}

void loop() 
{
    digitalWrite(sensorpower,HIGH);
  delay(10);
  // take reading from sensor
  sensor = analogRead(sensorpin);
    wetness = sensor / 8;
  if(wetness>100)
  {
    wetness = 100;
  }

  // turn sensor off to help prevent corrosion
  digitalWrite(sensorpower,LOW);
  
  // print sensor reading
  Serial.println(sensor);
  //Serial.print("Server is at ");
  //Serial.println(Ethernet.localIP());


  
  // If sensor reading is greater than "wet" threshold,
  // turn on the blue LED. If it is less than the "dry"
  // threshold, turn on the red LED and the pump. 
  // If it is in between the two values, turn on 
  // the yellow LED.
  if(sensor>wet){
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,HIGH);
    digitalWrite(pumppin,LOW);
  }
  else if(sensor<dry){
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(pumppin,HIGH);
  }
  else{
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,LOW);
    digitalWrite(pumppin,LOW);
  }
  
  // wait before taking next reading
  delay(delayTime);

  // Listen for incoming clients
  EthernetClient client = server.available();
  if (client) 
  {
    Serial.println("new client");
    // An http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        Serial.write(c);
        // If you've gotten to the end of the  line (received a newline character) and the 
        // line is blank, the http request has ended, so you can send a reply:
        if (c == '\n' && currentLineIsBlank) 
        {
          // Send a standard http response header:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          client.println("<body>");
          client.println("<h1>MEETING NATHEID</h1>");
          client.print(wetness);
          client.println("%");

          client.println("</body>");

          // Add a meta refresh tag, so the browser pulls again every 5 seconds:
//          client.println("<meta http-equiv=\"refresh\" content=\"5\">");

          // Output the value of each analog input pin:
//          for (int analogChannel = 0; analogChannel < 6; analogChannel++) 
//          {
//            int sensorReading = analogRead(analogChannel);
//            client.print("analog input ");
//            client.print(analogChannel);
//            client.print(" is ");
//            client.print(sensorReading);
//            client.println("<br />");       
//          }

          client.println("</html>");
          break;
        }
        
        if (c == '\n') 
        {
          currentLineIsBlank = true;  // you're starting a new line
        } 
        else if (c != '\r') 
        {
          currentLineIsBlank = false; // you've gotten a character on the current line
        }
      }
      else
      {
//        Serial.println("Client unavailable");
          delay(1);
      }
    }
    
    delay(1); // Give the web browser time to receive the data
    
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


//// Add a meta refresh tag, so the browser pulls again every 5 seconds:
//client.println("<meta http-equiv=\"refresh\" content=\"5\">");
//
//// Output the value of each analog input pin:
//for (int analogChannel = 0; analogChannel < 6; analogChannel++) 
//{
//  int sensorReading = analogRead(analogChannel);
//  client.print("analog input ");
//  client.print(analogChannel);
//  client.print(" is ");
//  client.print(sensorReading);
//  client.println("<br />");       
//}
