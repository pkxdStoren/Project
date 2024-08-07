Hi! This is **Aryan Singh**, the owner of this repository. This is a project made by me for the Inspire awards. The project is of a quadruped robot which is specialized for locating victims of a fire. 

- The file named *ardu.ino* is the code of the Arduino Nano used for data collection and servo control.
- The file named *esp.ino* is the code of the esp32 for communication between controller and the robot.
- The file named *control.ino* is the code of the remote control which uses a joystick for manouvering && an oled screen for showing the data collected by numerous sensors of the robot.

**The code is under development so there is still much to be done like improving functions, improving compatibility, etc. The microphone is removed.**

# THESE ARE THE CONNECTIONS OF THE MACHINE (WHICH MAY CHANGE)

**Arduino Nano Connections**
|COMPONENT	          |  PIN ON ARDUINO NANO	|PIN ON COMPONENT|
|SERVO 1, JOINT 1	    |  D11	                |Signal          |
|SERVO 1, JOINT 2	    |  D12	                |Signal          |
|SERVO 1, JOINT 3	    |  D13	                |Signal          |
|SERVO 2, JOINT 1	    |  D2	                  |Signal          |
|SERVO 2, JOINT 2	    |  D4	                  |Signal          |
|SERVO 2, JOINT 3	    |  D7	                  |Signal          |
|SERVO 3, JOINT 1	    |  D14	                |Signal          |
|SERVO 3, JOINT 2	    |  D15	                |Signal          |
|SERVO 3, JOINT 3	    |  D16	                |Signal          |
|SERVO 4, JOINT 1	    |  D8	                  |Signal          |
|SERVO 4, JOINT 2	    |  D9	                  |Signal          |
|SERVO 4, JOINT 3	    |  D10	                |Signal          |
|FLAME SENSOR	        |  A0	                  |Signal          |
|ULTRASONIC TRIG	    |  D5	                  |Trig            |
|ULTRASONIC ECHO	    |  D6	                  |Echo            |
|SMOKE SENSOR	        |  A1	                  |Signal          |
|TEMPERATURE SENSOR   |	 A2	                  |Signal          |
|BME280 SDA	          |  A4	                  |SDA             |
|BME280 SCL	          |  A5	                  |SCL             |
|ESP32 RX	            |  TX (GPIO1)	          |Arduino Nano TX |
|ESP32 TX	            |  RX (GPIO3)	          |Arduino Nano RX |

ESP32 Connections (on Robot)
COMPONENT	PIN ON ESP32	PIN ON COMPONENT
ARDUINO NANO TX	RX2 (GPIO16)	TX
ARDUINO NANO RX	TX2 (GPIO17)	RX
WIFI	-	-

Remote Control (ESP32)
COMPONENT	PIN ON ESP32	PIN ON COMPONENT
JOYSTICK X-AXIS	A0	Signal
JOYSTICK Y-AXIS	A1	Signal
JOYSTICK BUTTON	GPIO2	Signal
OLED DISPLAY SDA	GPIO21	SDA
OLED DISPLAY SCL	GPIO22	SCL


