

// command to send: portVal=a255&portVal=b255&portVal=c255&portVal=d255&

const char softwareVersion[] = "1.00";
const char softwareUpdate[] = "07.2021";

const char asciiFilledSquare[] = "&#9608;"; //'█';
const char asciiSpace[] = "_";              //'_';


// size of buffer to store HTTP requests
const uint8_t REQUEST_BUFFER = 100;
String httpRequest = ""; // HTTP request string


EthernetServer server(80);


void ethernetConfig_thread() {
  if (Ethernet.begin(mac, 60000, 4000) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }

  // start the server
  server.begin();

  /*
    Serial.print("MAC: ");
    for (byte octet = 0; octet < 6; octet++) {
    Serial.print(mac[octet], HEX);
    if (octet < 5) {
      Serial.print('-');
    }
    }
    Serial.println();
    Serial.print("IP: ");
    Serial.println(Ethernet.localIP());
  */
}


int ctrlConnection() {
  auto link = Ethernet.linkStatus();

  switch (link) {
    case LinkON:
      //Serial.println("Link status: connected.");
      return 1;
      break;
    case Unknown:
      Serial.println("Link status: unknown.");
      return -1;
      break;
    case LinkOFF:
      Serial.println("Link status: not connected.");
      return 0;
      break;
    default: {
        return -2;
      }
  }
}


const char html_1[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>BTMS MCU</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<style>
* {
  box-sizing: border-box;
}
th, td {
  padding: 3px;
}
.resizable {
  resize: both;
  overflow: scroll;
}
footer {
  text-align: center;
  padding: 3px;
  background-color: LightBlue;
}
</style>
<script>
function addOptionsFunction() {
  var x = document.getElementById("timingPanel");
  if (x.style.display === "none") {
    x.style.display = "block";
  } else {
    x.style.display = "none";
  }
}
</script>
</head>
<body style="font-family:Verdana;color: DarkSlateGray;">
)rawliteral";


String httpFilterString(String httpRqst, int index){
  int posVal = httpRqst.indexOf('=', index);
  int endNumber = httpRqst.indexOf('&', posVal + 1);
  //Serial.println(httpRqst);
  String subString = httpRqst.substring(posVal + 1, endNumber);
  
  //Serial.println(subString.toInt());
  //return subString.toInt();  
  return subString;
}


String findInHttpRequest(String httpRqst, String request){
  
}


void htmlPage(auto client) {

  // Start html
  String htmlPage = "";
  htmlPage += "HTTP/1.1 200 OK";
  htmlPage += "Content-Type: text/html";
  //htmlPage += "Refresh: 1";
  htmlPage += "Connection: close";
  htmlPage += "\n";

  htmlPage += html_1;

  // check http requests
//  if (httpRequest.indexOf("portVal=")  > 0) { 
//    String val = decodeData(httpFilterString(httpRequest, "portVal="));
//    Serial.println(val);
//  }

  bool checkRequest = true;
  uint8_t indexVal = 0;  
  while(checkRequest){
    int indexFound = httpRequest.indexOf("portVal=", indexVal);
    if (indexFound > 0) {
      String val = decodeData(httpFilterString(httpRequest, indexFound));
      Serial.println(val);
      
      indexVal = indexFound + 1; 
    }   
    else{
      checkRequest = false;
    }
  }

  htmlPage += decodeData("a");
  htmlPage += "<br>";
  htmlPage += decodeData("b");
  htmlPage += "<br>";
  htmlPage += decodeData("c");
  htmlPage += "<br>";
  htmlPage += decodeData("d");
  htmlPage += "<br>";

  // Footer
  htmlPage += "<p><input type=\"button\" value=\"Refresh\" onclick = \"location.href='/?refresh'\"></p>";
  htmlPage += "<br><footer><p><br>Version: ";
  htmlPage += softwareVersion;
  htmlPage += "<br><br>JD ";
  htmlPage += softwareUpdate;
  htmlPage += "<br><br></p></footer></body></html>";

  // send html page
  client.println(htmlPage);
  
  // close client connection
  client.close();
}


void handleWebServer() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    //Serial.println("New client.");

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        if (httpRequest.length() < REQUEST_BUFFER) {
          httpRequest += c;
        }

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          htmlPage(client);

          // reset buffer
          httpRequest = "";
          break;
        }
        if (c == '\n') {
          // start new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // character received on the current line
          currentLineIsBlank = false;
        }
      }
    }

    // give the web browser time to receive the data
    //delay(1);

    // close the connection:
    client.stop();
    //Serial.println("Client disconnected.");
  }
}
