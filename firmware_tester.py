import serial
import time

ser = 0

CLOCK_PERIOD = 0.1

def init():
	global ser
	ser = serial.Serial(port = "COM5", baudrate = 9600)
	time.sleep(3)


def sendNBits(N, data):
	if N <= 0:
		return

	for i in range(N):
		ser.write(bytes('11 0\n', "utf-8"))
		ser.write(bytes('10 ' + (str((data>>i) & 1) + "\n"), "utf-8"))
		time.sleep(CLOCK_PERIOD/2)
		ser.write(bytes('11 1\n', "utf-8"))
		time.sleep(CLOCK_PERIOD/2)


	
def readN(N):
	if N <= 0:
		return

	for i in range(N):
		ser.write(bytes(b'10 r\n'))
		ser.readline()
		time.sleep(CLOCK_PERIOD/2)
		ser.reset_input_buffer()
		ser.write(bytes(b'10 r\n'))
		print(ser.readline().decode("utf-8")[0:-2])
		time.sleep(CLOCK_PERIOD/2)
		ser.write(bytes('11 0\n', "utf-8"))
		time.sleep(CLOCK_PERIOD/2)
		ser.write(bytes('11 1\n', "utf-8"))

def fullDataCycle(id, cmd, enb):
	if(enb):
		ser.write(bytes('8 0\n', "utf-8"))

	for i in range(5):
		ser.write(bytes('11 0\n', "utf-8"))
		ser.write(bytes('10 ' + (str((id>>i) & 1) + "\n"), "utf-8"))
		time.sleep(CLOCK_PERIOD/2)
		ser.write(bytes('11 1\n', "utf-8"))
		time.sleep(CLOCK_PERIOD/2)
	if(enb):
		ser.write(bytes('8 1\n', "utf-8"))

	for i in range(5):
		ser.write(bytes('11 0\n', "utf-8"))
		ser.write(bytes('10 ' + (str((cmd>>i) & 1) + "\n"), "utf-8"))
		time.sleep(CLOCK_PERIOD/2)
		ser.write(bytes('11 1\n', "utf-8"))
		time.sleep(CLOCK_PERIOD/2)


	ser.write(bytes(b'10 r\n'))
	for i in range(10):
		ser.write(bytes(b'11 0\n'))
		time.sleep(CLOCK_PERIOD/2)
		ser.write(bytes(b'11 1\n'))
		time.sleep(0.1)
		ser.reset_input_buffer()
		ser.write(bytes(b'10 r\n'))
		print(ser.readline().decode("utf-8")[0:-2])
		time.sleep(CLOCK_PERIOD/2)


def cleanup():
  	ser.close()



init()

ser.write(bytes('11 1\n', "utf-8"))
time.sleep(CLOCK_PERIOD/2)
print("chirp")
fullDataCycle(13, 14, True)
#10110 01110

CLOCK_PERIOD = 0.2


print("green flash")
fullDataCycle(13, 9, False)
print("red on")
fullDataCycle(13, 8, False)
print("red off")
fullDataCycle(13, 6, False)
print("green off")
fullDataCycle(13, 5, False)
print("green on")
fullDataCycle(13, 7, False)


print("enb right")
fullDataCycle(13, 2, False)

print("false id green blink")
fullDataCycle(12, 9, False)

print("green blink")
fullDataCycle(13, 9, False)

print("red blink")
fullDataCycle(13, 10, False)
#01101 01010

print("right")
fullDataCycle(13, 12, False)

print("left")
fullDataCycle(13, 11, False)


print("reset")
fullDataCycle(13, 13, False)

print("chirp")
fullDataCycle(14, 14, True)

print("red blink")
fullDataCycle(14, 10, False)

print("false id")
fullDataCycle(13, 9, False)


cleanup()