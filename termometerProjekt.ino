#include <DHT.h>
#include <DHT_U.h>

#include <Adafruit_Sensor.h>


#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>


#include <ESPAsyncWebServer.h>

#define DHTPIN 2      // Define the pin to which the DHT11 data pin is connected
#define DHTTYPE DHT11 // Define the type of DHT sensor (DHT11 or DHT22)

float temperature = 0.0;
float humidity = 0.0;

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);  // Create a web server on port 80

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    .header2 { font-size: 3.0rem; 
      color: #059e8a;
    }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    #Tempdht{
      color: #059e8a;
    }
    #Humdht {
      color: #00add6;
    }
    #temperature{
      color: #059e8a;
    }
    #humidity{
      color: #00add6;
    }
    #humidityUnit{
      color: #00add6;
    }
    #tempUnit{
      color: #059e8a;
    }
  </style>
</head>
<body>
  <h2 class="header2">Los Aceitunas Hermanos Humidity and Temperature</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels"id="Tempdht">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units" id="tempUnit">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels"id="Humdht">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units" id="humidityUnit">%</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 2000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 2000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(temperature);
  }
  else if(var == "HUMIDITY"){
    return String(humidity);
  }
  return String();
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin("ABB_Gym_IOT", "Welcome2abb");  // Replace with your WiFi credentials

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(temperature).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(humidity).c_str());
  });

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  // Print the IP address to the Serial Monitor
  server.begin();  // Start the web server


}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(2000); // Delay for 2 seconds before taking another reading
}
