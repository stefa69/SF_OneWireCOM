# SF_OneWireCOM
Arduino library for one wire (one pin) communication for ESP8266 and ESP32 microcontrollers

In case of other possibilities, this library can be used to exchange text or byte data between one Master and one or more Slaves. 
If you managed to use all other ports in your project, you need just one pin free and you still can exchange data. That is what
happened to me! This is my solution for such situation. 

SESSIONS
1. Master starts session
2. Slaves must listen if there is rquest for session from Master
3. Only Slave with adequate ID answers
4. Data transfer starts...
5. There are four types of sessions
   - Master sends text message to Slave
   - Master sends byte message to Slave
   - Slave sends txt message to Master
   - Slave sends byte message to Master


Byte messages can be used to exchange data in structures

Example:

  memcpy(&stored_sensorDS1820, MASTER.SFOneWireCOM_InputBuffer_BYTE, sizeof(stored_sensorDS1820))

where "stored_sensorDS1820" is some user structure with length <= 256

REMARKS
1. Data transfer speed is about 1KB per second
2. There is a simple checksum to ensure exact data transfer
3. Max message size (text or byte) is 256byte long
4. Every slave has its own address
5. Master can address any slave and communicate only with chosen one
6. Communication is done in sessions

FURTHER DEVELOPMENT
- Test with ARDUINO UNO
