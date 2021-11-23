import time
import requests


boardUrl = "http://btms-mcu-0.cern.ch"

# 'abc', portA, portB, portC, portD
tableVal = [('cal on', 0b00000010, 0, 0, 0),
            #('cal off', 0, 0, 0, 0),
            #('cal a0', 0, 0, 0, 0),
            ('cal a1', 0b00000001, 0, 0, 0),
            ('cal a2', 0b00000100, 0, 0, 0),
            ('cal a3', 0b00010000, 0, 0, 0),
            ('cal a4', 0b01000000, 0, 0, 0),
            ('cal a5', 0, 0b10000000, 0, 0),
            ('cal a6', 0, 0b00100000, 0, 0),
            #('cal b0', 0, 0, 0, 0),
            ('cal b1', 0, 0b00000001, 0, 0),
            ('cal b2', 0, 0b00001000, 0, 0),
            ('cal b3', 0, 0b00000100, 0, 0),
            ('cal b4', 0, 0b00000010, 0, 0),
            ('cal b5', 0, 0b00010000, 0, 0),
            ('cal b6', 0, 0b01000000, 0, 0),  
            ('amp a0', 0, 0, 0, 0),
            ('amp a1', 0, 0, 0b00000001, 0),
            ('amp a2', 0, 0, 0b00000010, 0),
            ('amp a3', 0, 0, 0b00000100, 0),
            ('amp a4', 0, 0, 0b00001000, 0),
            ('amp a5', 0, 0, 0b00010000, 0),
            ('amp a6', 0, 0, 0b00100000, 0),
            #('amp b0', 0, 0, 0, 0),
            ('amp b1', 0, 0, 0, 0b00000001),
            ('amp b2', 0, 0, 0, 0b00000010),
            ('amp b3', 0, 0, 0, 0b00000100),
            ('amp b4', 0, 0, 0, 0b00001000),
            ('amp b5', 0, 0, 0, 0b00010000),
            ('amp b6', 0, 0, 0, 0b00100000),
            #('cal b0', 0, 0, 0, 0),
            ('read', 0, 0, 0, 0),
            ]

portA = 0
portB = 0
portC = 0
portD = 0


def queryValues(url):
    response = requests.get(url)
    val = response.content
    val = val.decode("utf-8")
    val = val.split(' ')    
    time.sleep(0.1)  
    return val


def portWrite(port, val):
    queryValues(boardUrl + '/portVal='+ port + str(val) + '&')
      
      
def updatePort(index, txt):
    global portA
    global portB
    global portC
    global portD
    
    portA ^= tableVal[index][1]
    portB ^= tableVal[index][2]
    portC ^= tableVal[index][3]
    portD ^= tableVal[index][4]
    
    portWrite('a', portA)
    portWrite('b', portB)
    portWrite('c', portC)
    portWrite('d', portD)
    print(txt, '-> done')
      

def setDevice(cmd):
    global tableVal

    print(cmd)
    for i in range(len(tableVal)):
        if(cmd == "read"):
            return queryValues(boardUrl + '/getValues')
        
        elif(cmd == tableVal[i][0]):
            updatePort(i, cmd)
            return cmd

    return 'No cmd'
            
    
from tkinter import *

class Window(Frame):

    def __init__(self, master=None):
        Frame.__init__(self, master)        
        self.master = master

        # widget can take all window
        self.pack(fill=BOTH, expand=1)

        self.leftFrame = Frame(self)
        self.leftFrame.pack(side = LEFT)
        self.rightFrame = Frame(self)
        self.rightFrame.pack(side = LEFT)
       
        # create button, link it to clickExitButton()
        self.ctrlBtn = [0] * len(tableVal)
        self.btnColor = [0] * len(tableVal)

        for i in range(len(tableVal)):
            if (i < 13):
                self.ctrlBtn[i] = Button(self.leftFrame, text=tableVal[i][0], command=lambda c=i:self.setCmd(c))
            elif (i < 26):
                self.ctrlBtn[i] = Button(self.rightFrame, text=tableVal[i][0], command=lambda c=i:self.setCmd(c))
            else:
                self.ctrlBtn[i] = Button(self, text=tableVal[i][0], command=lambda c=i:self.setCmd(c))
    
        #self.read = Button(self, text=tableVal[26][0], command=lambda:self.setCmd(26))
        self.output = Text(self, height = 5, width = 25, bg = "white")
        self.sendBtn = Button(self, text="Send", command=lambda:setDevice(self.output.get("1.0", "end-1c")))
        exitButton = Button(self, text="Exit", command=self.clickExitButton)

        # place buttons
        for i in range(len(tableVal)):
            self.ctrlBtn[i].pack(side = TOP)

        #self.read.pack(side = TOP)
        self.output.pack(side = TOP)
        self.sendBtn.pack(side = TOP)
        exitButton.pack(side = BOTTOM)
        

    def clickExitButton(self):
        print('exit')
        exit()
        

    def setCmd(self, btn):
        try:
            if (self.btnColor[btn] == 0):
                self.ctrlBtn[btn].configure(bg = "lime")
            else:
                self.ctrlBtn[btn].configure(bg='SystemButtonFace')
            self.btnColor[btn] = not self.btnColor[btn]
        except:
          print("An exception occurred")

        txt = setDevice(tableVal[btn][0])    
        print(txt)
        self.output.delete('1.0', END)
        self.output.insert(END, txt)


# reset the board
portWrite('a', 0)
portWrite('b', 0)
portWrite('c', 0)
portWrite('d', 0)

print("---\nEthernet remote control v1\n---\nJD 10/2021\n---\n\n>>> CTRL attenuator <<<\n")
print("CMD list: \n- <cal-amp> -<on-off> \n- <cal-amp> -<a/b><0-6> [dB steps] \n- read [read settings]\n")
         
root = Tk()
app = Window(root)
root.wm_title("CTRL attenuator")
#root.geometry("320x200")
root.mainloop()   

