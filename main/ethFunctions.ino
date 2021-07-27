

const char softwareVersion[] = "2.00";
const char softwareUpdate[] = "04.2021";

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
}


int ctrlConnection() {
  auto link = Ethernet.linkStatus();

  Serial.print("Link status: ");
  switch (link) {
    case LinkON:
      Serial.println("connected.");
      return 1;
      break;
    case Unknown:
      Serial.println("unknown.");
      return -1;
      break;
    case LinkOFF:
      Serial.println("not connected.");
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


String setupTiming(){
  String htm = "<br><input type=\"checkbox\" onclick=\"addOptionsFunction()\" name=\"advancedSettings\">Advanced settings</p>";
  htm += "<div style=\"text-align:left;display: none;\" id=\"timingPanel\"><h3>SETUP SIMULATED TIMING</h3>";
  htm += "<form action=\"/\"><table>";

  htm += "<tr><td>calstartTime (Value in &#181;s):</td><td><input type=\"number\" id=\"quantity\" name=\"val1\" min=\"1\" max=\"1200000\" value=\"";
  htm += calstartTime;
  htm += "\"></td></tr>";
  htm += "<tr><td>calstopTime (Value in &#181;s):</td><td><input type=\"number\" id=\"quantity\" name=\"val2\" min=\"1\" max=\"1200000\" value=\"";
  htm += calstopTime;
  htm += "\"></td></tr>";
  htm += "<tr><td>injTime (Value in &#181;s):</td><td><input type=\"number\" id=\"quantity\" name=\"val3\" min=\"1\" max=\"1200000\" value=\"";
  htm += injTime;
  htm += "\"></td></tr>";
  htm += "<tr><td>hchTime (Value in &#181;s):</td><td><input type=\"number\" id=\"quantity\" name=\"val4\" min=\"1\" max=\"1200000\" value=\"";
  htm += hchTime;
  htm += "\"></td></tr>";
  htm += "<tr><td>ecyTime (Value in &#181;s):</td><td><input type=\"number\" id=\"quantity\" name=\"val5\" min=\"1\" max=\"1200000\" value=\"";
  htm += ecyTime;
  htm += "\"></td></tr>";
  
  htm += "</table><br><input type=\"submit\" value=\"Save\"></form>";
  htm += "<input type=\"submit\" value=\"Reset\" onclick=\"location.href='/?reset='\"><br><br></div>";
  return htm;
}


String manualTiming(){
  String htm = "<div style=\"text-align:left;\" id=\"manualPanel\"><h3>MANUAL TIMING</h3>";
  htm += "<form action=\"/\"><table>";
  htm += "<tr><td><input type=\"checkbox\" onclick=\"return false;\" checked></td><td>SCY</td>";
  if (!continuousRunning) htm += "<td><input type=\"submit\" value=\"Simulate once\" name=\"manualSim0'\"></td></tr>";
  
  htm += "<tr><td><input type=\"checkbox\" name=\"CalStrt\"";
  if (calStartSimulation) htm += "checked";
  htm += "></td><td>CalStrt</td>";
  if (!continuousRunning) htm += "<td><input type=\"submit\" value=\"Simulate once\" name=\"manualSim1'\"></td></tr>";
  
  htm += "<tr><td><input type=\"checkbox\" name=\"CalStp\"";
  if (calStopSimulation) htm += "checked";
  htm += "></td><td>CalStp</td>";
  if (!continuousRunning) htm += "<td><input type=\"submit\" value=\"Simulate once\" name=\"manualSim2'\"></td></tr>";
  
  htm += "<tr><td><input type=\"checkbox\" name=\"INJ\"";
  if (injSimulation) htm += "checked";
  htm += "></td><td>INJ</td>";
  if (!continuousRunning) htm += "<td><input type=\"submit\" value=\"Simulate once\" name=\"manualSim3'\"></td></tr>";
  
  htm += "<tr><td><input type=\"checkbox\" name=\"HCH\"";
  if (hchSimulation) htm += "checked";
  htm += "></td><td>HCH</td>";
  if (!continuousRunning) htm += "<td><input type=\"submit\" value=\"Simulate once\" name=\"manualSim4'\"></td></tr>";
  
  htm += "<tr><td><input type=\"checkbox\" onclick=\"return false;\" checked></td><td>ECY</td>"; 
  if (!continuousRunning) htm += "<td><input type=\"submit\" value=\"Simulate once\" name=\"manualSim5'\"></td></tr>";
  
  htm += "</table><br>Continuous running: <input type=\"submit\" name=\"continuousRunning\" value=\"";
  htm += continuousRunning;
  htm += "\"></form></div>";
  return htm;
}


String opModeOption(int mode){
  String htm = "<form action=/><label for=\"opMode\"><b>Operation mode: ";
  htm += mode;
  htm += "</b><br><br></label>";
  htm += "<select name=\"opMode\" id=\"opMode\"><optgroup label=\"Read only\">";
  
  if (mode==0) htm += "<option value=\"0\" selected>0. Read timing</option>";
  else htm += "<option value=\"0\">0. Read timing</option>";
  
  htm += "</optgroup><optgroup label=\"Simulated\">";

  if (mode==1) htm += "<option value=\"1\" selected>1. Simulate SCY and ECY</option>";
  else htm += "<option value=\"1\">1. Simulate SCY and ECY</option>";

  if (mode==2) htm += "<option value=\"2\" selected>2. Simulate SCY, INJ and ECY</option>";
  else htm += "<option value=\"2\">2. Simulate SCY, INJ and ECY</option>";

  if (mode==3) htm += "<option value=\"3\" selected>3. Simulate SCY, CALSTART, CALSTOP, INJ and ECY</option>";
  else htm += "<option value=\"3\">3. Simulate SCY, CALSTART, CALSTOP, INJ and ECY</option>";

  htm += "</optgroup><optgroup label=\"Manual\">";

  if (mode==4) htm += "<option value=\"4\" selected>4. Manual mode</option>";
  else htm += "<option value=\"4\">4. Manual mode</option>";

  htm += "</optgroup></select><button type=”submit”>Change</button></form>"; 
  return htm;
}


String h1_title(int val){
  String htm = "<div style=\"background-color:LightBlue;padding:15px;text-align:center;\"><h1>BTMS MCU #";
  htm += val;
  htm += "</h1></div>";
  return htm;
}


String showInfo(String color, String title){
  String htm = "<tr><td><svg width=\"14\" height=\"14\"><circle cx=\"7\" cy=\"7\" r=\"8\" fill=\"";
  htm += color;
  htm += "\"/> Your browser does not support inline SVG. </svg><b> ";
  htm += title;
  htm += "</b></td>";
  return htm;
}


String h2_title(String title){
  String htm = "<br><div style=\"text-align:left;\"><h2>";
  htm += title;
  htm += "</h2></div><hr>";
  return htm;
}


void buildPlot(){
  for(uint8_t i=0; i < numTraces; i++){
    uint32_t val = 0;      
    if (traceTime[i] < psTimeCycle){
      val = ceil(traceTime[i] / 4999.0);  // 4999 so 5000 will be rounded to 2 with the function ceil
    }
    else {
      val = 0;
    }

    for(uint32_t cnt = 0; cnt < samplesNumber; cnt++){  
      plot[i][cnt] = 0;
    }
    
    if (val > 0) {
      val--;
      for(uint32_t cnt = 0; cnt < samplesNumber; cnt++){  
        if (val == cnt){
          plot[i][cnt] = 1;
        }
      }
    }    
  }
}


uint32_t httpFilterString(String httpRqst, String request){
  int posVal = httpRqst.indexOf('=', httpRqst.indexOf(request));
  int endNumber = httpRqst.indexOf(' ', posVal + 1);
  //Serial.println(httpRqst);
  String subString = httpRequest.substring(posVal + 1, endNumber);
  Serial.println(subString.toInt());
  return subString.toInt();  
}


void htmlPage(auto client) {
  uint32_t tempVal = 0;

  // Read incoming timing on the interrupts
  noInterrupts();
  traceTime[0] = startOfcycle;
  traceTime[1] = calStart;
  traceTime[2] = calStop;
  traceTime[3] = 0xFFFFFFFF;      // no INJ signal
  traceTime[4] = harmonicChange;
  traceTime[5] = endOfCycle;

  // Reset values
  startOfcycle = 0;
  calStart = 0;
  calStop = 0;
  harmonicChange = 0;
  endOfCycle = 0;
  interrupts();

  // check http requests
  if (httpRequest.indexOf("opMode=")  > 0) {
    operationMode = httpFilterString(httpRequest, "opMode=");
    selectOperationMode();
  }

  if (httpRequest.indexOf("val1=")  > 0) {
    tempVal = httpFilterString(httpRequest, "val1=");
    noInterrupts();
    calstartTime = tempVal;
    interrupts();
  }
  if (httpRequest.indexOf("val2=")  > 0) {    
    tempVal = httpFilterString(httpRequest, "val2=");
    noInterrupts();
    calstopTime =  tempVal;
    interrupts();
  }
  if (httpRequest.indexOf("val3=")  > 0) {
    tempVal = httpFilterString(httpRequest, "val3=");
    noInterrupts();
    injTime = tempVal;
    interrupts();
  }
  if (httpRequest.indexOf("val4=")  > 0) {
    tempVal = httpFilterString(httpRequest, "val4=");
    noInterrupts();
    hchTime = tempVal;
    interrupts();
  }
  if (httpRequest.indexOf("val5=")  > 0) {
    tempVal = httpFilterString(httpRequest, "val5=");
    noInterrupts();
    ecyTime = tempVal;
    interrupts();
  }

  if (httpRequest.indexOf("continuousRunning")  > 0) {
    continuousRunning = !continuousRunning;
    if (!continuousRunning){
      simulatedTiming.end();

      noInterrupts();
      calStartSimulation = false;
      calStopSimulation = false;
      injSimulation = false;
      hchSimulation = false;
      interrupts();
    }
    else{
      noInterrupts();
      cnt_cycle = 0;
      interrupts();
      simulatedCycle();
    }       
  }

  if (!continuousRunning) {
    if (httpRequest.indexOf("manualSim0")  > 0) {
      triggerSCY(); 
    }
    else if (httpRequest.indexOf("manualSim1")  > 0) {
      triggerCalStrt();
    }
    else if (httpRequest.indexOf("manualSim2")  > 0) {
      triggerCalStp();
    }
    else if (httpRequest.indexOf("manualSim3")  > 0) {
      triggerINJ();
    }
    else if (httpRequest.indexOf("manualSim4")  > 0) {
      triggerHCH();
    }
    else if (httpRequest.indexOf("manualSim5")  > 0) {
      triggerECY();
    }
    digitalWriteFast(StsLed1, LOW);
    digitalWriteFast(StsLed2, LOW);
    digitalWriteFast(StsLed3, LOW);
    digitalWriteFast(StsLed4, LOW);
  }
  else {
    if (httpRequest.indexOf("CalStrt=on")  > 0) {
      noInterrupts();
      calStartSimulation = true;
      interrupts();
    }
    if (httpRequest.indexOf("CalStp=on")  > 0) {    
      noInterrupts();
      calStopSimulation = true;
      interrupts();
    }
    if (httpRequest.indexOf("INJ=on")  > 0) {
      noInterrupts();
      injSimulation = true;
      interrupts();
    }
    if (httpRequest.indexOf("HCH=on")  > 0) {
      noInterrupts();
      hchSimulation = true;
      interrupts();
    }
  }

  if (httpRequest.indexOf("reset=")  > 0) {
    noInterrupts();
    scyTime = SCY_T;            // time in µs
    calstartTime = CALSTART_T;  // time in µs
    calstopTime = CALSTOP_T;    // time in µs
    injTime = INJ_T;            // time in µs
    hchTime = HCH_T;            // time in µs
    ecyTime = ECY_T;            // time in µs
    psTimeCycle = PSCYCLE_T;    // time in µs
    interrupts();
  }
  
   
  // Start html
  String htmlPage = "";
  htmlPage += "HTTP/1.1 200 OK";
  htmlPage += "Content-Type: text/html";
  htmlPage += "Connection: close";
  //htmlPage += "Refresh: 1";
  htmlPage += "Connection: close";
  htmlPage += "\n";

  htmlPage += html_1;

  htmlPage += h1_title(boardSN);

  htmlPage += h2_title("CONTROL PANEL");

  htmlPage += "<span style=\"font-weight:bold\"><table>";
  if (true) htmlPage += showInfo("LawnGreen", "<td>BOARD STATUS</td></tr>");
  else htmlPage += showInfo("Red", "<td>BOARD STATUS</td></tr>");
  
  if (det10Mhz == 1) htmlPage += showInfo("LawnGreen", "<td>DET10MHz</td></tr>");
  else htmlPage += showInfo("LightGray", "<td>DET10MHz</td></tr>");
  
  if (lock == 0) htmlPage += showInfo("LawnGreen", "<td>LOCK</td></tr>");
  else htmlPage += showInfo("LightGray", "<td>LOCK</td></tr>");

  htmlPage += showInfo("LightGray", "<td>MCU temp:</td><td>" + String(cpuTemp) + " &#176;C</td></tr>");
  htmlPage += showInfo("LightGray", "<td>BOX temp:</td><td>" + String(v1) + " &#176;C</td></tr>");
  htmlPage += showInfo("LightGray", "<td>V3 (-12V):</td><td>" + String(v2) + " V</td></tr>");
  htmlPage += showInfo("LightGray", "<td>V2 (+12V):</td><td>" + String(v3) + " V</td></tr>");
  htmlPage += showInfo("LightGray", "<td>V1 (+5V):</td><td>" + String(v4) + " V</td></tr>");
  htmlPage += "</table></span>";

  
  htmlPage += h2_title("SETTINGS");
  htmlPage += opModeOption(operationMode);

  htmlPage += setupTiming();
  
  if (operationMode == 4){
    htmlPage += manualTiming();
  }

  htmlPage += h2_title("PLOTS");
  buildPlot();
  
  String html_2 = "<div class=\"resizable\"><table>";
  html_2 += "<tr><th> </th><th>Time in &#181;s</th><th>Time scale in ms</th></tr>"; 
  for (uint8_t cnt = 0; cnt < numTraces; cnt++) {
    if (traceTime[cnt] < 0xFFFFFFFF){
      html_2 += "<tr><th>";
      html_2 += traceName[cnt];
      html_2 += "</th><td>";
      html_2 += traceTime[cnt];
      html_2 += "</td><td><pre>";  
      for (uint32_t i = 0; i < samplesNumber; i++) {
        if (plot[cnt][i]) {
          html_2 += asciiFilledSquare;
        }
        else {
          html_2 += asciiSpace;
        }
      }
      html_2 += "\n";  
      /*
        for (uint32_t i = 0; i < samplesNumber; i++) {
        htmlPage2 += plot[cnt][i];
        }
        htmlPage2 += "\n";
      */
      html_2 += "</pre></td></tr>";
    }
  }
  html_2 += "</table><br></div>";
  htmlPage += html_2; 
  

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
    Serial.println("New client.");

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
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("Client disconnected.");
  }
}
