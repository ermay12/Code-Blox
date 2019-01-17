import serial
import time

ser = 0

CLOCK_PERIOD = 0.08

def init():
	global ser
	ser = serial.Serial(port = "COM5", baudrate = 9600)
	time.sleep(3)


def sendNBits(N, data):
	if N <= 0:
		return

	for i in range(N):
		ser.write(bytes('7 0\n', "utf-8"))
		ser.write(bytes('6 ' + (str((data>>i) & 1) + "\n"), "utf-8"))
		time.sleep(CLOCK_PERIOD/2)
		ser.write(bytes('7 1\n', "utf-8"))
		time.sleep(CLOCK_PERIOD/2)


	
def readN(N):
	ser.write(bytes(b'6 r\n'))
	if N <= 0:
		return

	for i in range(N):
		ser.write(bytes('7 0\n', "utf-8"))
		time.sleep(CLOCK_PERIOD/2)
		ser.write(bytes('7 1\n', "utf-8"))
		ser.flush()
		time.sleep(CLOCK_PERIOD/2)
		ser.reset_input_buffer()
		ser.write(bytes(b'6 r\n'))
		print(ser.readline().decode("utf-8")[0:-2])

def cleanup():
  	ser.close()



init()

ser.write(bytes('7 1\n', "utf-8"))
ser.write(bytes('5 1\n', "utf-8"))
time.sleep(CLOCK_PERIOD/2)
sendNBits(5, 27) #11010
sendNBits(5, 12) #01100
print("chirp")
readN(10)

sendNBits(5, 27)
sendNBits(5, 9)
print("left")
readN(10)

sendNBits(5, 27)
sendNBits(5, 10)
print("right")
readN(10)

sendNBits(5, 27)
sendNBits(5, 4)
print("enb_left")
readN(10)

sendNBits(5, 27)
sendNBits(5, 3)
print("enb_bottom")
readN(10)

sendNBits(5, 26)
sendNBits(5, 12)
print("false id")
readN(10)

ser.write(bytes('5 0\n', "utf-8"))
sendNBits(5, 27)
sendNBits(5, 11)
print("reset")
readN(10)

ser.write(bytes('5 1\n', "utf-8"))
time.sleep(CLOCK_PERIOD/2)
sendNBits(5, 26)
sendNBits(5, 12)
print("chirp 2")
readN(10)

sendNBits(5, 26)
sendNBits(5, 9)
print("left 2")
readN(10)

sendNBits(5, 26)
sendNBits(5, 10)
print("right 2")
readN(10)

sendNBits(5, 27)
sendNBits(5, 12)
print("false id 2")
readN(10)

cleanup()