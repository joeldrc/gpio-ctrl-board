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
      


# reset the board
updatePort(' ')

print("---\nEthernet remote control v1\n---\nJD 10/2021\n---\n\n>>> CTRL attenuator <<<\n")
print("CMD list: \n- <cal-amp> -<on-off> \n- <cal-amp> -<a/b><0-6> [dB steps] \n- read [read settings]\n")


"""
while (True):
    cmd = input('> ')
    #cmd = cmd.split(' ')
    print(cmd)

"""


def setDevice(cmd):
    global portA
    global portB
    global portC
    global portD
    
    if(cmd.find("amp") >= 0):
                  
        if(cmd.find("a0") >= 0):
            portC = 0
            updatePort(cmd)
            
        elif(cmd.find("a1") >= 0):
            portC ^= 0b00000001
            updatePort(cmd)
            
        elif(cmd.find("a2") >= 0):
            portC ^= 0b00000010
            updatePort(cmd)

        elif(cmd.find("a3") >= 0):
            portC ^= 0b00000100
            updatePort(cmd)
            
        elif(cmd.find("a4") >= 0):
            portC ^= 0b00001000
            updatePort(cmd)
            
        elif(cmd.find("a5") >= 0):
            portC ^= 0b00010000
            updatePort(cmd)

        elif(cmd.find("a6") >= 0):
            portC ^= 0b00100000
            updatePort(cmd)

        elif(cmd.find("b0") >= 0):
            portD = 0
            updatePort(cmd)
            
        elif(cmd.find("b1") >= 0):
            portD ^= 0b00000001
            updatePort(cmd)
            
        elif(cmd.find("b2") >= 0):
            portD ^= 0b00000010
            updatePort(cmd)

        elif(cmd.find("b3") >= 0):
            portD ^= 0b00000100
            updatePort(cmd)
            
        elif(cmd.find("b4") >= 0):
            portD ^= 0b00001000
            updatePort(cmd)
            
        elif(cmd.find("b5") >= 0):
            portD ^= 0b00010000
            updatePort(cmd)

        elif(cmd.find("b6") >= 0):
            portD ^= 0b00100000
            updatePort(cmd)

        return cmd

        
    elif(cmd.find("cal") >= 0):
        
        if(cmd.find("on") >= 0):       
            portA ^= 0b00000010
            updatePort(cmd)
            
        elif(cmd.find("off") >= 0):
            portA &= 0b11111101
            updatePort(cmd)
            
        elif(cmd.find("a0") >= 0):
            portA &= 0b10101010
            portB &= 0b01011111
            updatePort(cmd)
            
        elif(cmd.find("a1") >= 0):
            portA ^= 0b00000001
            updatePort(cmd)
            
        elif(cmd.find("a2") >= 0):
            portA ^= 0b00000100
            updatePort(cmd)

        elif(cmd.find("a3") >= 0):
            portA ^= 0b00010000
            updatePort(cmd)
            
        elif(cmd.find("a4") >= 0):
            portA ^= 0b01000000
            updatePort(cmd)
            
        elif(cmd.find("a5") >= 0):
            portB ^= 0b10000000
            updatePort(cmd)

        elif(cmd.find("a6") >= 0):
            portB ^= 0b00100000
            updatePort(cmd)

        elif(cmd.find("b0") >= 0):
            portB ^= 0b10100000
            updatePort(cmd)
            
        elif(cmd.find("b1") >= 0):
            portB ^= 0b00000001
            updatePort(cmd)
            
        elif(cmd.find("b2") >= 0):
            portB ^= 0b00001000
            updatePort(cmd)

        elif(cmd.find("b3") >= 0):
            portB ^= 0b00000100
            updatePort(cmd)
            
        elif(cmd.find("b4") >= 0):
            portB ^= 0b00000010
            updatePort(cmd)
            
        elif(cmd.find("b5") >= 0):
            portB ^= 0b00010000
            updatePort(cmd)

        elif(cmd.find("b6") >= 0):
            portB ^= 0b01000000
            updatePort(cmd)

        return cmd

    elif(cmd.find("read") >= 0):      
        return queryValues(boardUrl + '/getValues')


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
        self.ctrlBtn = [0] * 27
        self.btnColor = [0] * 27
        
        self.ctrlBtn[0] = Button(self.leftFrame, text="CAL ON", command=lambda:self.setCmd(0,'cal on'))
        self.ctrlBtn[1] = Button(self.leftFrame, text="CAL OFF", command=lambda:self.setCmd(1,'cal off'), state="disabled")
        self.ctrlBtn[2] = Button(self.leftFrame, text="CAL A1", command=lambda:self.setCmd(2,'cal a1'))
        self.ctrlBtn[3] = Button(self.leftFrame, text="CAL A2", command=lambda:self.setCmd(3,'cal a2'))
        self.ctrlBtn[4] = Button(self.leftFrame, text="CAL A3", command=lambda:self.setCmd(4,'cal a3'))
        self.ctrlBtn[5] = Button(self.leftFrame, text="CAL A4", command=lambda:self.setCmd(5,'cal a4'))
        self.ctrlBtn[6] = Button(self.leftFrame, text="CAL A5", command=lambda:self.setCmd(6,'cal a5'))
        self.ctrlBtn[7] = Button(self.leftFrame, text="CAL A6", command=lambda:self.setCmd(7,'cal a6'))
        self.ctrlBtn[8] = Button(self.leftFrame, text="CAL B1", command=lambda:self.setCmd(8,'cal b1'))
        self.ctrlBtn[9] = Button(self.leftFrame, text="CAL B2", command=lambda:self.setCmd(9,'cal b2'))
        self.ctrlBtn[10] = Button(self.leftFrame, text="CAL B3", command=lambda:self.setCmd(10,'cal b3'))
        self.ctrlBtn[11] = Button(self.leftFrame, text="CAL B4", command=lambda:self.setCmd(11,'cal b4'))
        self.ctrlBtn[12] = Button(self.leftFrame, text="CAL B5", command=lambda:self.setCmd(12,'cal b5'))
        self.ctrlBtn[13] = Button(self.leftFrame, text="CAL B6", command=lambda:self.setCmd(13,'cal b6'))

        self.ctrlBtn[14] = Button(self.rightFrame, text="AMP A1", command=lambda:self.setCmd(14,'amp a1'))
        self.ctrlBtn[15] = Button(self.rightFrame, text="AMP A2", command=lambda:self.setCmd(15,'amp a2'))
        self.ctrlBtn[16] = Button(self.rightFrame, text="AMP A3", command=lambda:self.setCmd(16,'amp a3'))
        self.ctrlBtn[17] = Button(self.rightFrame, text="AMP A4", command=lambda:self.setCmd(17,'amp a4'))
        self.ctrlBtn[18] = Button(self.rightFrame, text="AMP A5", command=lambda:self.setCmd(18,'amp a5'))
        self.ctrlBtn[19] = Button(self.rightFrame, text="AMP A6", command=lambda:self.setCmd(19,'amp a6'))
        self.ctrlBtn[20] = Button(self.rightFrame, text="AMP B1", command=lambda:self.setCmd(20,'amp b1'))
        self.ctrlBtn[21] = Button(self.rightFrame, text="AMP B2", command=lambda:self.setCmd(21,'amp b2'))
        self.ctrlBtn[22] = Button(self.rightFrame, text="AMP B3", command=lambda:self.setCmd(22,'amp b3'))
        self.ctrlBtn[23] = Button(self.rightFrame, text="AMP B4", command=lambda:self.setCmd(23,'amp b4'))
        self.ctrlBtn[24] = Button(self.rightFrame, text="AMP B5", command=lambda:self.setCmd(24,'amp b5'))
        self.ctrlBtn[25] = Button(self.rightFrame, text="AMP B6", command=lambda:self.setCmd(25,'amp b6'))
     
        self.ctrlBtn[26] = Button(self, text="Read", command=lambda:self.setCmd(26,'read'))
        self.output = Text(self, height = 5, width = 25, bg = "white")
        self.sendBtn = Button(self, text="Send", command=lambda:setDevice(self.output.get("1.0", "end-1c")))
        exitButton = Button(self, text="Exit", command=self.clickExitButton)

        # place buttons
        for i in range(len(self.ctrlBtn)):
            self.ctrlBtn[i].pack(side = TOP)

        self.output.pack(side = TOP)
        self.sendBtn.pack(side = TOP)
        exitButton.pack(side = BOTTOM)
        

    def clickExitButton(self):
        print('exit')
        exit()
        

    def setCmd(self, btn, cmd):
        if (self.btnColor[btn] == 0):
            self.ctrlBtn[btn].configure(bg = "lime")
        else:
            self.ctrlBtn[btn].configure(bg='SystemButtonFace')

        self.btnColor[btn] = not self.btnColor[btn]
            
        txt = setDevice(cmd)     
        print(txt)
        self.output.delete('1.0', END)
        self.output.insert(END, txt)
        
        
root = Tk()
app = Window(root)
root.wm_title("CTRL attenuator")
#root.geometry("320x200")
root.mainloop()   

