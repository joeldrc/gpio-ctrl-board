

// command to send: portVal=a255&portVal=b255&portVal=c255&portVal=d255&


String request = "";
unsigned int count = 0;
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";


const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
 <head>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
  <meta charset='utf-8'>
  <style>
    body {font-size:100%;} 
    #main {display: table; margin: auto;  padding: 0 10px 0 10px; } 
    h2 {text-align:center; } 
    p { text-align:center; }
  </style>

  <script> 
    function updateCount() 
    {  
       ajaxLoad('getValues'); 
    }


    var ajaxRequest = null;
    if (window.XMLHttpRequest)  { ajaxRequest =new XMLHttpRequest(); }
    else                        { ajaxRequest =new ActiveXObject("Microsoft.XMLHTTP"); }

    function ajaxLoad(ajaxURL)
    {
      if(!ajaxRequest){ alert('AJAX is not supported.'); return; }

      ajaxRequest.open('GET',ajaxURL,true);
      ajaxRequest.onreadystatechange = function()
      {
        if(ajaxRequest.readyState == 4 && ajaxRequest.status==200)
        {
          var ajaxResult = ajaxRequest.responseText;
          document.getElementById('count_P').innerHTML = ajaxResult;
        }
      }
      ajaxRequest.send();
    }

    setInterval(updateCount, 500);

  </script>
  <title>Auto Update Example Using Javascript 2</title>
 </head>

 <body>
   <div id='main'>
     <h2>Auto Update Example Using Javascript 2</h2>
     <div id='count_DIV'> 
       <p id='count_P'>Count = 0</p>
     </div>
   </div> 
 </body>
</html>
)rawliteral"; 


int ctrlConnection() {
  auto link = Ethernet.linkStatus();
  switch (link) {
    case LinkON: {
        return 1;
      }
      break;
    case Unknown: {
        return -1;
      }
      break;
    case LinkOFF: {
        return 0;
      }
      break;
    default: {
        return -2;
      }
  }
}


String httpFilterString(String httpRqst, int index) {
  int posVal = httpRqst.indexOf('=', index);
  int endNumber = httpRqst.indexOf('&', posVal + 1);
  //Serial.println(httpRqst);
  String subString = httpRqst.substring(posVal + 1, endNumber);

  //Serial.println(subString.toInt());
  //return subString.toInt();
  return subString;
}

void clientResponse(auto client){
  client.print(header);
  client.print("Count="); 
  client.print(count); 

  client.print(" portA="); 
  client.print(P1A_val); 
  client.print(" portB="); 
  client.print(P1B_val); 
  client.print(" portC="); 
  client.print(P2A_val); 
  client.print(" portD="); 
  client.print(P2B_val); 

  Serial.print("Count=");
  Serial.println(count);
}


void handleWebServer() {
  // listen for incoming clients
  EthernetClient client = server.available();

  if (client.available()) {  
    //Serial.println("client available");
     
    // Read the first line of the request
    request = client.readStringUntil('\r');

    int indexFound = request.indexOf("portVal=");
    if (indexFound > 0) {
      String val = decodeData(httpFilterString(request, indexFound));
      //Serial.println(val);
      clientResponse(client);
    }   
    else if (request.indexOf("getValues") > 0 ){ 
      count ++;
      clientResponse(client);
    } 
    else{
      client.flush();
      client.print(header);
      client.print(html); 
      count=0;
    }
  
    // close client connection
    client.close();
  
    // give the web browser time to receive the data
    //delay(1);    
  }

  if (!client.connected()) {
    // close the connection:
    client.stop();
    //Serial.println("Client disconnected.");
  }
}
