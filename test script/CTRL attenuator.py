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
    print(txt + ' -> setted')
      

print("Ethernet remote control v1 \n--- JD 10/2021 ---\n>>> CTRL attenuator <<<\n")
print("CMD list: \n- <on-off> \n- <a/b><0-6> [dB steps] \n- read [read settings]\n")

while (True):
    cmd = input('>>> ')
    cmd = cmd.split(' ')
    
    for i in range(len(cmd)):
       
        if(cmd[i] == 'on'):       
            portA |= 0b00000010
            updatePort(cmd[i])
            
        elif(cmd[i] == 'off'):
            portA &= 0b11111101
            updatePort(cmd[i])
            
        elif(cmd[i] == 'a0'):
            portA &= 0b10101010
            portB &= 0b01011111
            updatePort(cmd[i])
            
        elif(cmd[i] == 'a1'):
            portA |= 0b00000001
            updatePort(cmd[i])
            
        elif(cmd[i] == 'a2'):
            portA |= 0b00000100
            updatePort(cmd[i])

        elif(cmd[i] == 'a3'):
            portA |= 0b00010000
            updatePort(cmd[i])
            
        elif(cmd[i] == 'a4'):
            portA |= 0b01000000
            updatePort(cmd[i])
            
        elif(cmd[i] == 'a5'):
            portB |= 0b10000000
            updatePort(cmd[i])

        elif(cmd[i] == 'a6'):
            portB |= 0b00100000
            updatePort(cmd[i])

        elif(cmd[i] == 'b0'):
            portB &= 0b10100000
            updatePort(cmd[i])
            
        elif(cmd[i] == 'b1'):
            portB |= 0b00000001
            updatePort(cmd[i])
            
        elif(cmd[i] == 'b2'):
            portB |= 0b00001000
            updatePort(cmd[i])

        elif(cmd[i] == 'b3'):
            portB |= 0b00000100
            updatePort(cmd[i])
            
        elif(cmd[i] == 'b4'):
            portB |= 0b00000010
            updatePort(cmd[i])
            
        elif(cmd[i] == 'b5'):
            portB |= 0b00010000
            updatePort(cmd[i])

        elif(cmd[i] == 'b6'):
            portB |= 0b01000000
            updatePort(cmd[i])

        elif(cmd[i] == 'read'):      
            print(*queryValues(boardUrl + '/getValues'), sep = '\n')
    
     

