#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x88, 0x47};
//IPAddress ip(192,168,178,35);
IPAddress ip(192, 168, 1, 12);

EthernetServer server(80);  // port 80 is default for HTTP

void setup() 
{
  Serial.begin(9600);

  // Start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() 
{
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
          client.println("<h1>Welkom op mijn mooie Arduino website</h1>");
          client.println("<p>HTML Hello World.</p>");
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
