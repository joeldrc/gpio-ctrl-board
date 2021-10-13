import time
import requests


def queryValues(url):
    response = requests.get(url)
    val = response.content
    val = val.decode("utf-8")
    print("\r" + val, end = '')
    return val


def port(val):
    for i in range(256):        
        queryValues('http://btms-mcu-0.cern.ch/portVal='+ val + str(i) + '&')
        time.sleep(0.1)
 

print("Ethernet remote control v1 \nTerminal reading \n")
while (True):
    port('a')
    port('b')
    port('c')
    port('d')

    
