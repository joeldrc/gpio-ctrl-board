import time
import requests


boardUrl = "http://btms-mcu-0.cern.ch"
portA = 0b00000000
portB = 0b00000000
portC = 0b00000000
portD = 0b00000000


def queryValues(url):
    response = requests.get(url)
    val = response.content
    val = val.decode("utf-8")
    val = val.split(' ')
    
    time.sleep(0.1)
    
    return val


def portWrite(port, val):
    queryValues(boardUrl + '/portVal='+ port + str(val) + '&')
      
      
def updatePort(txt):
    portWrite('a', portA)
    portWrite('b', portB)
    portWrite('c', portC)
    portWrite('d', portD)
    print(txt, '-> done')
      

print("---\nEthernet remote control v1\n---\nJD 10/2021\n---\n\n>>> CTRL attenuator <<<\n")
print("CMD list: \n- <cal-amp> -<on-off> \n- <cal-amp> -<a/b><0-6> [dB steps] \n- read [read settings]\n")

while (True):
    cmd = input('> ')
    #cmd = cmd.split(' ") >= 0)

    if(cmd.find("amp") >= 0):
                  
        if(cmd.find("a0") >= 0):
            portC &= 0b10000000
            updatePort(cmd)
            
        elif(cmd.find("a1") >= 0):
            portC |= 0b00000001
            updatePort(cmd)
            
        elif(cmd.find("a2") >= 0):
            portC |= 0b00000010
            updatePort(cmd)

        elif(cmd.find("a3") >= 0):
            portC |= 0b00000100
            updatePort(cmd)
            
        elif(cmd.find("a4") >= 0):
            portC |= 0b00001000
            updatePort(cmd)
            
        elif(cmd.find("a5") >= 0):
            portC |= 0b00010000
            updatePort(cmd)

        elif(cmd.find("a6") >= 0):
            portC |= 0b01000000
            updatePort(cmd)

        elif(cmd.find("b0") >= 0):
            portD &= 0b10000000
            updatePort(cmd)
            
        elif(cmd.find("a1") >= 0):
            portD |= 0b00000001
            updatePort(cmd)
            
        elif(cmd.find("a2") >= 0):
            portD |= 0b00000010
            updatePort(cmd)

        elif(cmd.find("a3") >= 0):
            portD |= 0b00000100
            updatePort(cmd)
            
        elif(cmd.find("a4") >= 0):
            portD |= 0b00001000
            updatePort(cmd)
            
        elif(cmd.find("a5") >= 0):
            portD |= 0b00010000
            updatePort(cmd)

        elif(cmd.find("a6") >= 0):
            portD |= 0b01000000
            updatePort(cmd)

        
    elif(cmd.find("cal") >= 0):
        
        if(cmd.find("on") >= 0):       
            portA |= 0b00000010
            updatePort(cmd)
            
        elif(cmd.find("off") >= 0):
            portA &= 0b11111101
            updatePort(cmd)
            
        elif(cmd.find("a0") >= 0):
            portA &= 0b10101010
            portB &= 0b01011111
            updatePort(cmd)
            
        elif(cmd.find("a1") >= 0):
            portA |= 0b00000001
            updatePort(cmd)
            
        elif(cmd.find("a2") >= 0):
            portA |= 0b00000100
            updatePort(cmd)

        elif(cmd.find("a3") >= 0):
            portA |= 0b00010000
            updatePort(cmd)
            
        elif(cmd.find("a4") >= 0):
            portA |= 0b01000000
            updatePort(cmd)
            
        elif(cmd.find("a5") >= 0):
            portB |= 0b10000000
            updatePort(cmd)

        elif(cmd.find("a6") >= 0):
            portB |= 0b00100000
            updatePort(cmd)

        elif(cmd.find("b0") >= 0):
            portB &= 0b10100000
            updatePort(cmd)
            
        elif(cmd.find("b1") >= 0):
            portB |= 0b00000001
            updatePort(cmd)
            
        elif(cmd.find("b2") >= 0):
            portB |= 0b00001000
            updatePort(cmd)

        elif(cmd.find("b3") >= 0):
            portB |= 0b00000100
            updatePort(cmd)
            
        elif(cmd.find("b4") >= 0):
            portB |= 0b00000010
            updatePort(cmd)
            
        elif(cmd.find("b5") >= 0):
            portB |= 0b00010000
            updatePort(cmd)

        elif(cmd.find("b6") >= 0):
            portB |= 0b01000000
            updatePort(cmd)


    elif(cmd.find("read") >= 0):      
        print(*queryValues(boardUrl + '/getValues'), sep = '\n')

     

