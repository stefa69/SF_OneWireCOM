// =============================================================================
// =============================================================================
// =============================================================================
//
//  SF-Logic OneWireCOM Arduino library, V7527.0406
// ------------------------------------------------
//
// Author: Stefanovic Srdjan
// Pocetak radova: Mart 2018., 7526. дерикожа
//
// Programming language: C++
//
// Compatible devices
// - WEMOS LOLIN32 (ESP32)
// - NODEMCU 0.9 sa ESP-12 plocicom (ESP8266)
//


// ****************************************************************
// ****************************************************************
// INCLUDEs...
// ****************************************************************
#include "SF_OneWireCOM.h"
// ****************************************************************


// ****************************************************************
// ***  Constructor 1 *********************************************
// ****************************************************************
SF_OneWireCOM::SF_OneWireCOM()
{
  _SlaveID = 0;
  _CommunicationPIN = 0;
  _MasterSlave = SF_OWCOM_MASTER;
}
// ****************************************************************



// ****************************************************************
// ***  Constructor 2 *********************************************
// ****************************************************************
SF_OneWireCOM::SF_OneWireCOM(byte aCommunicationPIN, byte aMasterSlave)
{
  _CommunicationPIN = aCommunicationPIN;
  _MasterSlave = aMasterSlave;
}
// ****************************************************************



// ****************************************************************
// *** Private variables functions ********************************
// ****************************************************************
void SF_OneWireCOM::SetSlaveID(byte aSlaveID)
{
  _SlaveID = aSlaveID;
}
byte SF_OneWireCOM::GetSlaveID()
{
  return _SlaveID;
}
void SF_OneWireCOM::SetCommunicationPIN(byte aCommunicationPIN)
{
  _CommunicationPIN = aCommunicationPIN;
}
byte SF_OneWireCOM::GetCommunicationPIN()
{
  return _CommunicationPIN;
}
void SF_OneWireCOM::SetMasterSlave(byte aMasterSlave)
{
  _MasterSlave = aMasterSlave;
}
byte SF_OneWireCOM::GetMasterSlave()
{
  return _MasterSlave;
}
String SF_OneWireCOM::GetSoftwareSerial()
{
  return _SOFTWARE_SERIAL;
}
String SF_OneWireCOM::GetLibraryName()
{
  return _LIBRARY_NAME;
}
String SF_OneWireCOM::GetCopyright()
{
  return _SF_COPYRIGHT;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::CREATE_AttentionSignal()
{
  pinMode(_CommunicationPIN, OUTPUT);  // mode: INPUT, OUTPUT, or INPUT_PULLUP

  // CREATE Attention signal...
  for (int i=0; i<ATTENTION_SIGNAL_NO_OF_PULSES; i++)
  {
    digitalWrite(_CommunicationPIN, HIGH);
    delayMicroseconds(ATTENTION_SIGNAL_HALFDURATION);
    digitalWrite(_CommunicationPIN, LOW);
    delayMicroseconds(ATTENTION_SIGNAL_HALFDURATION);
  }
  delayMicroseconds(ATTENTION_SIGNAL_BLANK);
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
byte SF_OneWireCOM::DETECT_AttentionSignal()
{
  // Return value
  // ---------------
  // 0 - No Attention signal...
  // 1 - Attention signal DETECTED!
  // 2 - DETECT_AttentionSignal timeout
  int HighCounter = 0;
  int LowCounter = 0;
  int X;
  int pocetak;
  int trajanje;
  byte temp_PIN_value;

  pinMode(_CommunicationPIN, INPUT);  // mode: INPUT, OUTPUT, or INPUT_PULLUP

  // Skip garbage at the begining and also detect timeout...
  pocetak = micros();
  temp_PIN_value = digitalRead(_CommunicationPIN);
  blank:
  if (micros() - pocetak > ATTENTION_SIGNAL_TIMEOUT)  
  {
    _ErrorCode = 2; // DETECT_AttentionSignal timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == temp_PIN_value) goto blank;
 
  X = digitalRead(_CommunicationPIN);
  do
  {
    pocetak = micros();
    if (X == 1)
    {
      do
      {
      } while (digitalRead(_CommunicationPIN) == 1 && (micros() - pocetak) < 200);
      trajanje = micros() - pocetak;
      if (trajanje > ATTENTION_SIGNAL_HALFDURATION - 10 && trajanje < ATTENTION_SIGNAL_HALFDURATION + 10)
      {
		X = 0;
        HighCounter += 1;
      }
      else
      {
		break;
      }
    }
    else  //if (X == 0)
    {
      do
      {
      } while (digitalRead(_CommunicationPIN) == 0 && (micros() - pocetak) < 200);
      trajanje = micros() - pocetak;
      if (trajanje > ATTENTION_SIGNAL_HALFDURATION - 10 && trajanje < ATTENTION_SIGNAL_HALFDURATION + 10)
      {
		X = 1;
        LowCounter += 1;	  
      }
      else
      {
		break;
      }
    }
  } while (trajanje < ATTENTION_SIGNAL_HALFDURATION + 10 || HighCounter == 0);
  
  if (HighCounter > 3 && LowCounter > 3)
  {
    return 1; // Attention signal DETECTED!
  }
  else
  {
    return 0; // No Attention signal...
  }
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::CallSlave()
{
  WriteByte(_SlaveID);
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
byte SF_OneWireCOM::WaitForSlaveToReply()
{
  ReadByte();
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
byte SF_OneWireCOM::ReadErrorCode()
{
  return _ErrorCode;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
byte SF_OneWireCOM::GetLastRecievedByte()
{
  return _LastRecievedByte;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
int SF_OneWireCOM::CalculateTXTMessageChecksum(char * aMessage, int aMessageSize)
{
	int returnValue = 0;
	
	for (int i = 0; i < aMessageSize; i++)
	{
		returnValue += aMessage[i];
	}
	return returnValue;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
int SF_OneWireCOM::CalculateBYTEMessageChecksum(byte * aMessage, int aMessageSize)
{
	int returnValue = 0;
	
	for (int i = 0; i < aMessageSize; i++)
	{
		returnValue += aMessage[i];
	}
	return returnValue;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
int SF_OneWireCOM::RecieveMessageChecksum()
{
	byte tempByte;
	int tempChecksum;
	
	tempByte = ReadByte();
	if (ReadErrorCode() == ERR_OK_NOERROR)
	{
	  tempChecksum += tempByte;
	  tempChecksum <<= 8;
	}
	else
	{
		return 0;  // Greska pri citanju bajta...
	}
	
	tempByte = ReadByte();
	if (ReadErrorCode() == ERR_OK_NOERROR)
	{
	  tempChecksum += tempByte;
	  tempChecksum <<= 8;
	}
	else
	{
		return 0;  // Greska pri citanju bajta...
	}
	
	tempByte = ReadByte();
	if (ReadErrorCode() == ERR_OK_NOERROR)
	{
	  tempChecksum += tempByte;
	  tempChecksum <<= 8;
	}
	else
	{
		return 0;  // Greska pri citanju bajta...
	}

	tempByte = ReadByte();
	if (ReadErrorCode() == ERR_OK_NOERROR)
	{
	  tempChecksum += tempByte;
	}
	else
	{
		return 0;  // Greska pri citanju bajta...
	}
	return tempChecksum;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::SendMessageChecksum(int aChecksum)
{
	byte tempByte;

	tempByte = (aChecksum & 0b11111111000000000000000000000000) >> 24;
	WriteByte(tempByte);	
	tempByte = (aChecksum & 0b00000000111111110000000000000000) >> 16;
	WriteByte(tempByte);
	tempByte = (aChecksum & 0b00000000000000001111111100000000) >> 8;
	WriteByte(tempByte);
	tempByte = aChecksum & 0b00000000000000000000000011111111;
	WriteByte(tempByte);
}
// ****************************************************************



// ****************************************************************
// *** SESSION Launcher *******************************************
// ****************************************************************
byte SF_OneWireCOM::SESSION_Launch(byte aSlaveID, byte aSessionType)
{
  // Ova f-ja 'uspostavlja' sesiju i sluzi kao 'zaglavlje' za
  // ostale SESSION f-je Master-a

  // Set SlaveID
  SetSlaveID(aSlaveID);
  // Make slaves listen...
  CREATE_AttentionSignal();
  // Call slave... (send ID of Slave)
  CallSlave();
  // Wait for slave reply with ID... (Slave replies with its ID)
  WaitForSlaveToReply();
  // Slave replied
  if (ReadErrorCode() == ERR_OK_NOERROR)
  {
    if (GetLastRecievedByte() == GetSlaveID()) // Slave ID...
    {
      // Send SESSION type to 'Slave'
	  WriteByte(aSessionType);
	  // Wait for slave confirm SESSION type
      WaitForSlaveToReply();
      if (ReadErrorCode() == ERR_OK_NOERROR)
      {
        if (GetLastRecievedByte() == aSessionType)
        {
          return ERR_OK_NOERROR;
		}
	  }
	}
  }
  return SESSION_FAILED;
}
// ****************************************************************



// ****************************************************************
// *** Master SESSION function - SESSION Debug ********************
// ****************************************************************
void SF_OneWireCOM::SESSION_Debug(byte aSlaveID)
{
  // Provera tacnosti prenosa velike kolicine podataka
  
  byte tempByte;
  
  if (SESSION_Launch(aSlaveID, SESSION_DEBUG) == ERR_OK_NOERROR)
  {
    WriteByte(DebugByte);
    tempByte = ReadByte();
    if(tempByte == DebugByte)
    {
      // No error
      DebugByte += 1;
    }
    else
    {
      DebugNoOfReadByteErrors += 1;
      // Display error data
//      Serial.print("Sent: ");
//      Serial.println(DebugByte);
//      Serial.print("Recieved: ");
//      Serial.println(tempByte);
//      Serial.println("---------------");
      DebugByte += 1;
    }
  }
  else
  {
	DebugNoOfSessionErrors += 1;
  }
}
// ****************************************************************



// ****************************************************************
// *** Slave SESSION function *************************************
// ****************************************************************
byte SF_OneWireCOM::SESSION()
{
  // Return value...
  // ------------------------------------
  // SESSION_ERR_NO_SESSION
  // SESSION_ERR_NO_MESSAGE
  // SESSION_ERR_UNKNOWN_SESSION
  // SESSION_ERR_NO_MASTER
  //
  // Session types
  // ------------------------------------
  // SESSION_DEBUG
  // SESSION_SendTXTDataToSlave
  // SESSION_SendBYTEDataToSlave
  // SESSION_SendTXTDataToMaster
  // SESSION_SendBYTEDataToMaster
  //
  
  byte messageLength;
  byte messageType; // 0 - TXT, 1 - BYTE
  byte returnValue;

  byte tempByte;
  int tempInt;

  returnValue = SESSION_ERR_NO_SESSION;
  if (DETECT_AttentionSignal())
  {
	// Mater will send Slave ID that will be active in this session
	tempByte = ReadByte();
	if (ReadErrorCode() != ERR_OK_NOERROR)
	{
      return SESSION_ERR_NO_SESSION;
	}
    if (tempByte == GetSlaveID())
    {
      // Reply to Master with SlaveID - It's me! :) 
      WriteByte(GetSlaveID());
      // Wait for Master - master will send SESSION type
      tempByte = ReadByte();
	  if (ReadErrorCode() != ERR_OK_NOERROR)
	  {
        return SESSION_ERR_NO_SESSION;
	  }
      switch (tempByte)
	  {
		  case SESSION_DEBUG:
		      // Confirm SESSION type
              WriteByte(SESSION_DEBUG);
			  // --------------------------
			  
			  // Ovde treba posebno obratiti paznju na kontrolu komunikacije
			  // Ako je pogresna sesija onda Master 'prekida' sesiju tj. "cuti"
			  // treba proveravati prvi sledeci bajt koji se ocekuje
			  // ako vrati ERR_READBYTE_TIMEOUT onda nista od sesije
			  
              // Wait for Master - master will send byte...
              tempByte = ReadByte();
			  if (ReadErrorCode() == ERR_OK_NOERROR)
			  {
			    // Return same byte to master
                WriteByte(tempByte);
			    returnValue = SESSION_DEBUG;
			  }
			  else
			  {
				returnValue = SESSION_ERR_NO_SESSION;
			  }
			  break;
		  case SESSION_SendTXTDataToSlave:
		      // Confirm SESSION type
              WriteByte(SESSION_SendTXTDataToSlave);
			  // --------------------------
			  
			  // Ovde treba posebno obratiti paznju na kontrolu komunikacije
			  // Ako je pogresna sesija onda Master 'prekida' sesiju tj. "cuti"
			  // nakon svakog ReadByte salje se isti primljeni bajt Master-u
			  
			  // TXT poruka se salje bez ponavljanja bajtova ali se na kraju salje CRC
			  // ako je CRC los, sesija se prekida
			  // Nakon prekida sesije Master moze da ponovi istu sesiju			  
			  
              // Wait for Master - master will send number of bytes that will be sent...
              messageLength = ReadByte();
  			  if (ReadErrorCode() == ERR_OK_NOERROR)
			  {
                // Wait for Master - master will send message type
                messageType = ReadByte();
                if (ReadErrorCode() == ERR_OK_NOERROR)
			    {
                  // Recieve TXT message
                  if (messageType == TXT_MESSAGE) 
                  {
                    RecieveTxtMessage(messageLength);
					tempInt = CalculateTXTMessageChecksum(SFOneWireCOM_InputBuffer_TXT, messageLength);
					SendMessageChecksum(tempInt);
					tempByte = ReadByte();  // Proveri da li je doslo do greske
					if (ReadErrorCode() == ERR_READBYTE_TIMEOUT)
					{
						// Master se ne 'odaziva' - sve je OK!!!
                        returnValue = SESSION_SendTXTDataToSlave; 
					}
					else
					{
                        returnValue = SESSION_ERR_NO_SESSION;
					}
                  }
				}
				else
				{
                  returnValue = SESSION_ERR_NO_SESSION;
				}
			  }
			  else
			  {
				returnValue = SESSION_ERR_NO_SESSION;
			  }
			  break;
			  
		  case SESSION_SendBYTEDataToSlave:
		      // Confirm SESSION type
              WriteByte(SESSION_SendBYTEDataToSlave);
			  // --------------------------
			  
			  
			  // Ovde treba posebno obratiti paznju na kontrolu komunikacije
			  // Ako je pogresna sesija onda Master 'prekida' sesiju tj. "cuti"
			  // nakon svakog ReadByte salje se isti primljeni bajt Master-u
			  
			  // TXT poruka se salje bez ponavljanja bajtova ali se na kraju salje CRC
			  // ako je CRC los, sesija se prekida
			  // Nakon prekida sesije Master moze da ponovi istu sesiju			  
			  
              // Wait for Master - master will send number of bytes that will be sent...
              messageLength = ReadByte();
  			  if (ReadErrorCode() == ERR_OK_NOERROR)
			  {
                // Wait for Master - master will send message type
                messageType = ReadByte();
                if (ReadErrorCode() == ERR_OK_NOERROR)
			    {
                  // Recieve BYTE message
                  if (messageType == BYTE_MESSAGE) 
                  {
                    RecieveByteMessage(messageLength);
					tempInt = CalculateBYTEMessageChecksum(SFOneWireCOM_InputBuffer_BYTE, messageLength);
					SendMessageChecksum(tempInt);
					tempByte = ReadByte();  // Proveri da li je doslo do greske
					if (ReadErrorCode() == ERR_READBYTE_TIMEOUT)
					{
						// Master se ne 'odaziva' - sve je OK!!!
                        returnValue = SESSION_SendBYTEDataToSlave; 
					}
					else
					{
                        returnValue = SESSION_ERR_NO_SESSION;
					}
                  }
				}
				else
				{
                  returnValue = SESSION_ERR_NO_SESSION;
				}
			  }
			  else
			  {
				returnValue = SESSION_ERR_NO_SESSION;
			  }
			  break;
			  
		  case SESSION_SendTXTDataToMaster:
		      // Confirm SESSION type
              WriteByte(SESSION_SendTXTDataToMaster);
			  // --------------------------
		      if (GET_OutputTXTMessageSize() > 0)
			  {
                WriteByte(GET_OutputTXTMessageSize());
                SendTxtMessage(SFOneWireCOM_OutputBuffer_TXT, GET_OutputTXTMessageSize());
				tempInt = CalculateTXTMessageChecksum(SFOneWireCOM_OutputBuffer_TXT, GET_OutputTXTMessageSize());
				SendMessageChecksum(tempInt);
				tempByte = ReadByte();  // Proveri da li je doslo do greske
				if (ReadErrorCode() == ERR_READBYTE_TIMEOUT)
				{
					// Master se ne 'odaziva' - sve je OK!!!
					ClearOutputTXTBuffer(); // Resetuj poruku - Salje se samo jednom
					returnValue = SESSION_SendTXTDataToMaster;
				}
				else
				{
                    returnValue = SESSION_ERR_NO_SESSION;
				}
			  }
			  else
			  {
                WriteByte(0);
                returnValue = SESSION_ERR_NO_MESSAGE;
			  }
              break;
			  
		  case SESSION_SendBYTEDataToMaster:
		      // Confirm SESSION type
              WriteByte(SESSION_SendBYTEDataToMaster);
			  // --------------------------
		      if (GET_OutputBYTEMessageSize() > 0)
			  {
                WriteByte(GET_OutputBYTEMessageSize());
                SendByteMessage(SFOneWireCOM_OutputBuffer_BYTE, GET_OutputBYTEMessageSize());
				tempInt = CalculateBYTEMessageChecksum(SFOneWireCOM_OutputBuffer_BYTE, GET_OutputBYTEMessageSize());
				SendMessageChecksum(tempInt);
				tempByte = ReadByte();  // Proveri da li je doslo do greske
				if (ReadErrorCode() == ERR_READBYTE_TIMEOUT)
				{
					// Master se ne 'odaziva' - sve je OK!!!
					ClearOutputBYTEBuffer(); // Resetuj poruku - Salje se samo jednom
					returnValue = SESSION_SendBYTEDataToMaster;
				}
				else
				{
                    returnValue = SESSION_ERR_NO_SESSION;
				}
			  }
			  else
			  {
                WriteByte(0);
                returnValue = SESSION_ERR_NO_MESSAGE;
			  }
              break;
			  
          default:
		      returnValue = SESSION_ERR_UNKNOWN_SESSION;
              break;		  
	  }
    }
  }	
  else
  {
	returnValue = SESSION_ERR_NO_MASTER;
  }
  return returnValue; 
}
// ****************************************************************



// ****************************************************************
// *** Master SESSION function - Send TXT to slave ****************
// ****************************************************************
byte SF_OneWireCOM::SESSION_SendTXTData_To_Slave(byte aSlaveID, char * aMessage, int aMessageLength)
{
  // Return value...
  // ------------------------------------
  // SESSION_ERR_MESSAGE_TOO_BIG
  // SESSION_ERR_MESSAGE_CHECKSUM
  // SESSION_ERR_NO_SESSION
  // SESSION_SUCCESS
  // ------------------------------------


  // Note...
  // ------------------------------------
  // Max 255 bytes sent per session...
  // 
  // ------------------------------------

  int tempInt;
  int tempRecievedChecksum;
  
  if (aMessageLength > 255)
  {
	  return SESSION_ERR_MESSAGE_TOO_BIG;
  }
  else
  {
	  // Save TEXT message in class variables
	  ClearOutputTXTBuffer();
	  CopyTXTMessageToOutputBuffer(aMessage, aMessageLength);
  }
  
  if (SESSION_Launch(aSlaveID, SESSION_SendTXTDataToSlave) == ERR_OK_NOERROR)
  {
    // Send to slave number of bytes that Master will send...
    WriteByte(aMessageLength);
    // Send to slave type of message that Master will send...
    WriteByte(TXT_MESSAGE);  
    SendTxtMessage(aMessage, aMessageLength);
	tempRecievedChecksum = RecieveMessageChecksum();
	tempInt = CalculateTXTMessageChecksum(aMessage, aMessageLength);
	if (tempRecievedChecksum != tempInt)
	{
        WriteByte(255); // Obavesti Slave-a da je doslo do greske
		return SESSION_ERR_MESSAGE_CHECKSUM;
	}
	if (ReadErrorCode() != ERR_OK_NOERROR)
	{
        WriteByte(255); // Obavesti Slave-a da je doslo do greske
		return SESSION_ERR_MESSAGE_CHECKSUM;
	}
  }
  else
  {
	  return SESSION_ERR_NO_SESSION;
  }
  delayMicroseconds(1000);  // MUST delay!
  return SESSION_SUCCESS;
}
// ****************************************************************



// ****************************************************************
// *** Master SESSION function - Send BYTE to slave ***************
// ****************************************************************
byte SF_OneWireCOM::SESSION_SendBYTEData_To_Slave(byte aSlaveID, byte * aMessage, int aMessageLength)
{
  // Return value...
  // ------------------------------------
  // SESSION_ERR_MESSAGE_TOO_BIG
  // SESSION_ERR_MESSAGE_CHECKSUM
  // SESSION_ERR_NO_SESSION
  // SESSION_SUCCESS
  // ------------------------------------


  // Note...
  // ------------------------------------
  // Max 255 bytes sent per session...
  // 
  // ------------------------------------

  int tempInt;
  int tempRecievedChecksum;
  
  if (aMessageLength > 255)
  {
	  return SESSION_ERR_MESSAGE_TOO_BIG;
  }
  else
  {
	  // Save BYTE message in class variables
	  ClearOutputBYTEBuffer();
	  CopyBYTEMessageToOutputBuffer(aMessage, aMessageLength);
  }
  
  if (SESSION_Launch(aSlaveID, SESSION_SendBYTEDataToSlave) == ERR_OK_NOERROR)
  {
    // Send to slave number of bytes that Master will send...
    WriteByte(aMessageLength);
    // Send to slave type of message that Master will send...
    WriteByte(BYTE_MESSAGE);  
    SendByteMessage(aMessage, aMessageLength);
	tempRecievedChecksum = RecieveMessageChecksum();
	tempInt = CalculateBYTEMessageChecksum(aMessage, aMessageLength);
	if (tempRecievedChecksum != tempInt)
	{
        WriteByte(255); // Obavesti Slave-a da je doslo do greske
		return SESSION_ERR_MESSAGE_CHECKSUM;
	}
//	if (ReadErrorCode() != ERR_OK_NOERROR)
//	{
//        WriteByte(255); // Obavesti Slave-a da je doslo do greske
//		return SESSION_ERR_MESSAGE_CHECKSUM;
//	}
  }
  else
  {
	  return SESSION_ERR_NO_SESSION;
  }
  delayMicroseconds(1000);  // MUST delay!
  return SESSION_SUCCESS;
}
// ****************************************************************



// ****************************************************************
// *** Master SESSION function - Recieve TXT from slave ***********
// ****************************************************************
byte SF_OneWireCOM::SESSION_RecieveTXTData_From_Slave(byte aSlaveID)
{
  // Max 255 bytes recieved per session...

  int tempMessageLength;
  int tempInt;
  int tempRecievedChecksum;

  ClearInputTXTBuffer();

  if (SESSION_Launch(aSlaveID, SESSION_SendTXTDataToMaster) == ERR_OK_NOERROR)
  {
    // Read from slave number of bytes that Master will recieve in message...
    tempMessageLength = ReadByte();
    if (tempMessageLength == 0)
	{
      return SESSION_ERR_NO_MESSAGE;
	}
	else
	{
      RecieveTxtMessage(tempMessageLength);
	  tempRecievedChecksum = RecieveMessageChecksum();
	  tempInt = CalculateTXTMessageChecksum(SFOneWireCOM_InputBuffer_TXT, tempMessageLength);
      if (tempRecievedChecksum != tempInt)
      {
        WriteByte(255); // Obavesti Slave-a da je doslo do greske
	    return SESSION_ERR_MESSAGE_CHECKSUM;
      }
	}
  }
  // No reply from slave
  if (ReadErrorCode() == ERR_READBYTE_TIMEOUT)
  {
    return SESSION_FAILED;
  }
  delayMicroseconds(1000);  // MUST delay!
  return SESSION_SUCCESS;
}
// ****************************************************************



// ****************************************************************
// *** Master SESSION function - Recieve BYTE from slave **********
// ****************************************************************
byte SF_OneWireCOM::SESSION_RecieveBYTEData_From_Slave(byte aSlaveID)
{
  // Max 255 bytes recieved per session...

  int tempMessageLength;
  int tempInt;
  int tempRecievedChecksum;

  ClearInputBYTEBuffer();

  if (SESSION_Launch(aSlaveID, SESSION_SendBYTEDataToMaster) == ERR_OK_NOERROR)
  {
    // Read from slave number of bytes that Master will recieve in message...
    tempMessageLength = ReadByte();
    if (tempMessageLength == 0)
	{
      return SESSION_ERR_NO_MESSAGE;
	}
	else
	{
      RecieveByteMessage(tempMessageLength);
	  tempRecievedChecksum = RecieveMessageChecksum();
	  tempInt = CalculateBYTEMessageChecksum(SFOneWireCOM_InputBuffer_BYTE, tempMessageLength);
      if (tempRecievedChecksum != tempInt)
      {
        WriteByte(255); // Obavesti Slave-a da je doslo do greske
	    return SESSION_ERR_MESSAGE_CHECKSUM;
      }
	}
  }
  // No reply from slave
  if (ReadErrorCode() == ERR_READBYTE_TIMEOUT)
  {
    return SESSION_FAILED;
  }
  delayMicroseconds(1000);  // MUST delay!
  return SESSION_SUCCESS;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::ClearAllBuffers()
{
	ClearInputTXTBuffer();
	ClearInputBYTEBuffer();
	ClearOutputTXTBuffer();
	ClearOutputBYTEBuffer();
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::ClearInputTXTBuffer()
{
  for (int i = 0; i < MESSAGE_BUFFERS_SIZE; i++)
  {
    SFOneWireCOM_InputBuffer_TXT[i] = 0;
  }
  _InputTXTMessageSize = 0;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::ClearInputBYTEBuffer()
{
  for (int i = 0; i < MESSAGE_BUFFERS_SIZE; i++)
  {
    SFOneWireCOM_InputBuffer_BYTE[i] = 0;
  }
  _InputBYTEMessageSize = 0;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::ClearOutputTXTBuffer()
{
  for (int i = 0; i < MESSAGE_BUFFERS_SIZE; i++)
  {
    SFOneWireCOM_OutputBuffer_TXT[i] = 0;
  }
  _OutputTXTMessageSize = 0;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::ClearOutputBYTEBuffer()
{
  for (int i = 0; i < MESSAGE_BUFFERS_SIZE; i++)
  {
    SFOneWireCOM_OutputBuffer_BYTE[i] = 0;
  }
  _OutputBYTEMessageSize = 0;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::CopyTXTMessageToOutputBuffer(char * aMessage, int aMessageLength)
{
  for (int i = 0; i < aMessageLength; i++)
  {
    SFOneWireCOM_OutputBuffer_TXT[i] = aMessage[i];
  }
  _OutputTXTMessageSize = aMessageLength;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::CopyBYTEMessageToOutputBuffer(byte * aMessage, int aMessageLength)
{
  for (int i = 0; i < aMessageLength; i++)
  {
    SFOneWireCOM_OutputBuffer_BYTE[i] = aMessage[i];
  }
  _OutputBYTEMessageSize = aMessageLength;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
int SF_OneWireCOM::GET_InputTXTMessageSize()
{
	return _InputTXTMessageSize;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
int SF_OneWireCOM::GET_InputBYTEMessageSize()
{
	return _InputBYTEMessageSize;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
int SF_OneWireCOM::GET_OutputTXTMessageSize()
{
	return _OutputTXTMessageSize;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
int SF_OneWireCOM::GET_OutputBYTEMessageSize()
{
	return _OutputBYTEMessageSize;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::PrepareTXTMessageForMaster(char * aMessage, int aMessageSize)
{
  for (int i=0; i<aMessageSize; i++)
  {
    SFOneWireCOM_OutputBuffer_TXT[i] = aMessage[i];
  }
  _OutputTXTMessageSize = aMessageSize;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::PrepareBYTEMessageForMaster(byte * aMessage, int aMessageSize)
{
  for (int i=0; i<aMessageSize; i++)
  {
    SFOneWireCOM_OutputBuffer_BYTE[i] = aMessage[i];
  }
  _OutputBYTEMessageSize = aMessageSize;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::SendTxtMessage(char * aMessage, int aMessageLength)
{
  for (int i = 0; i < aMessageLength; i++)
  {
    WriteByte(aMessage[i]);
  }
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::SendByteMessage(byte * aMessage, int aMessageLength)
{
  for (int i = 0; i < aMessageLength; i++)
  {
    WriteByte(aMessage[i]);
  }
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::RecieveTxtMessage(int aMessageLength)
{
  for (int i = 0; i < aMessageLength; i++)
  {
    SFOneWireCOM_InputBuffer_TXT[i] = ReadByte();
  }
  _InputTXTMessageSize = aMessageLength;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::RecieveByteMessage(int aMessageLength)
{
  for (int i = 0; i < aMessageLength; i++)
  {
    SFOneWireCOM_InputBuffer_BYTE[i] = ReadByte();
  }
  _InputBYTEMessageSize = aMessageLength;
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void SF_OneWireCOM::WriteByte(byte aByteToSend)
{
  // SEND Byte...
  // ------------

  byte Bit_0_HIGH, Bit_0_LOW;
  byte Bit_1_HIGH, Bit_1_LOW;
  byte Bit_2_HIGH, Bit_2_LOW;
  byte Bit_3_HIGH, Bit_3_LOW;
  byte Bit_4_HIGH, Bit_4_LOW;
  byte Bit_5_HIGH, Bit_5_LOW;
  byte Bit_6_HIGH, Bit_6_LOW;
  byte Bit_7_HIGH, Bit_7_LOW;

  pinMode(_CommunicationPIN, OUTPUT);  // mode: INPUT, OUTPUT, or INPUT_PULLUP

  // Prepare bits...
  //----------------
  // BIT 0 sequence...
  if ((aByteToSend & 0b00000001) == 0)
  {
    Bit_0_HIGH = SHORT_BIT_PART;
    Bit_0_LOW = LONG_BIT_PART;
  }
  else
  {
    Bit_0_HIGH = LONG_BIT_PART;
    Bit_0_LOW = SHORT_BIT_PART;
  }

  // BIT 1 sequence...
  if ((aByteToSend & 0b00000010) == 0)
  {
    Bit_1_HIGH = SHORT_BIT_PART;
    Bit_1_LOW = LONG_BIT_PART;
  }
  else
  {
    Bit_1_HIGH = LONG_BIT_PART;
    Bit_1_LOW = SHORT_BIT_PART;
  }

  // BIT 2 sequence...
  if ((aByteToSend & 0b00000100) == 0)
  {
    Bit_2_HIGH = SHORT_BIT_PART;
    Bit_2_LOW = LONG_BIT_PART;
  }
  else
  {
    Bit_2_HIGH = LONG_BIT_PART;
    Bit_2_LOW = SHORT_BIT_PART;
  }

  // BIT 3 sequence...
  if ((aByteToSend & 0b00001000) == 0)
  {
    Bit_3_HIGH = SHORT_BIT_PART;
    Bit_3_LOW = LONG_BIT_PART;
  }
  else
  {
    Bit_3_HIGH = LONG_BIT_PART;
    Bit_3_LOW = SHORT_BIT_PART;
  }

  // BIT 4 sequence...
  if ((aByteToSend & 0b00010000) == 0)
  {
    Bit_4_HIGH = SHORT_BIT_PART;
    Bit_4_LOW = LONG_BIT_PART;
  }
  else
  {
    Bit_4_HIGH = LONG_BIT_PART;
    Bit_4_LOW = SHORT_BIT_PART;
  }

  // BIT 5 sequence...
  if ((aByteToSend & 0b00100000) == 0)
  {
    Bit_5_HIGH = SHORT_BIT_PART;
    Bit_5_LOW = LONG_BIT_PART;
  }
  else
  {
    Bit_5_HIGH = LONG_BIT_PART;
    Bit_5_LOW = SHORT_BIT_PART;
  }

  // BIT 6 sequence...
  if ((aByteToSend & 0b01000000) == 0)
  {
    Bit_6_HIGH = SHORT_BIT_PART;
    Bit_6_LOW = LONG_BIT_PART;
  }
  else
  {
    Bit_6_HIGH = LONG_BIT_PART;
    Bit_6_LOW = SHORT_BIT_PART;
  }

  // BIT 7 sequence...
  if ((aByteToSend & 0b10000000) == 0)
  {
    Bit_7_HIGH = SHORT_BIT_PART;
    Bit_7_LOW = LONG_BIT_PART;
  }
  else
  {
    Bit_7_HIGH = LONG_BIT_PART;
    Bit_7_LOW = SHORT_BIT_PART;
  }

  // Send bits...
  delayMicroseconds(BYTE_BREAK);  // Give slave time to prepare for new byte...
  // bit 7
  digitalWrite(_CommunicationPIN, HIGH);
  delayMicroseconds(Bit_7_HIGH);
  digitalWrite(_CommunicationPIN, LOW);
  delayMicroseconds(Bit_7_LOW);

  // bit 6
  digitalWrite(_CommunicationPIN, HIGH);
  delayMicroseconds(Bit_6_HIGH);
  digitalWrite(_CommunicationPIN, LOW);
  delayMicroseconds(Bit_6_LOW);

  // bit 5
  digitalWrite(_CommunicationPIN, HIGH);
  delayMicroseconds(Bit_5_HIGH);
  digitalWrite(_CommunicationPIN, LOW);
  delayMicroseconds(Bit_5_LOW);

  // bit 4
  digitalWrite(_CommunicationPIN, HIGH);
  delayMicroseconds(Bit_4_HIGH);
  digitalWrite(_CommunicationPIN, LOW);
  delayMicroseconds(Bit_4_LOW);

  // bit 3
  digitalWrite(_CommunicationPIN, HIGH);
  delayMicroseconds(Bit_3_HIGH);
  digitalWrite(_CommunicationPIN, LOW);
  delayMicroseconds(Bit_3_LOW);

  // bit 2
  digitalWrite(_CommunicationPIN, HIGH);
  delayMicroseconds(Bit_2_HIGH);
  digitalWrite(_CommunicationPIN, LOW);
  delayMicroseconds(Bit_2_LOW);

  // bit 1
  digitalWrite(_CommunicationPIN, HIGH);
  delayMicroseconds(Bit_1_HIGH);
  digitalWrite(_CommunicationPIN, LOW);
  delayMicroseconds(Bit_1_LOW);

  // bit 0
  digitalWrite(_CommunicationPIN, HIGH);
  delayMicroseconds(Bit_0_HIGH);
  digitalWrite(_CommunicationPIN, LOW);
  delayMicroseconds(Bit_0_LOW);

}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
byte SF_OneWireCOM::ReadByte()
{
  int lPocetak;
  // ---------------------------------
  int trajanje_BLANK;
  int trajanje_HIGH_7, trajanje_LOW_7;
  int trajanje_HIGH_6, trajanje_LOW_6;
  int trajanje_HIGH_5, trajanje_LOW_5;
  int trajanje_HIGH_4, trajanje_LOW_4;
  int trajanje_HIGH_3, trajanje_LOW_3;
  int trajanje_HIGH_2, trajanje_LOW_2;
  int trajanje_HIGH_1, trajanje_LOW_1;
  int trajanje_HIGH_0, trajanje_LOW_0;
  int temp_PIN_value;
  byte RecievedByte;

  _ErrorCode = ERR_OK_NOERROR; // No error
  _LastRecievedByte = 0;
  pinMode(_CommunicationPIN, INPUT);  // mode: INPUT, OUTPUT, or INPUT_PULLUP

  // Wait for BLANK to pass (NO CHANGE on communication pin for MAX 1000uS)
  lPocetak = micros();
  temp_PIN_value = digitalRead(_CommunicationPIN);
  blank:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == temp_PIN_value) goto blank;
  trajanje_BLANK = micros() - lPocetak;

  // Read BIT definition signals...
  // ------------------------------
  // BIT 7 - HIGH part
  lPocetak = micros();
  bh7:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 1) goto bh7;
  trajanje_HIGH_7 = micros() - lPocetak;
  
  // BIT 7 - LOW part
  lPocetak = micros();
  bl7:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 0) goto bl7;
  trajanje_LOW_7 = micros() - lPocetak;
  // BIT Detection

  // BIT 6 - HIGH part
  lPocetak = micros();
  bh6:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 1) goto bh6;
  trajanje_HIGH_6 = micros() - lPocetak;
  // BIT 6 - LOW part
  lPocetak = micros();
  bl6:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 0) goto bl6;
  trajanje_LOW_6 = micros() - lPocetak;

  // BIT 5 - HIGH part
  lPocetak = micros();
  bh5:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 1) goto bh5;
  trajanje_HIGH_5 = micros() - lPocetak;
  // BIT 5 - LOW part
  lPocetak = micros();
  bl5:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 0) goto bl5;
  trajanje_LOW_5 = micros() - lPocetak;

  // BIT 4 - HIGH part
  lPocetak = micros();
  bh4:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 1) goto bh4;
  trajanje_HIGH_4 = micros() - lPocetak;
  // BIT 4 - LOW part
  lPocetak = micros();
  bl4:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 0) goto bl4;
  trajanje_LOW_4 = micros() - lPocetak;

  // BIT 3 - HIGH part
  lPocetak = micros();
  bh3:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 1) goto bh3;
  trajanje_HIGH_3 = micros() - lPocetak;
  // BIT 3 - LOW part
  lPocetak = micros();
  bl3:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 0) goto bl3;
  trajanje_LOW_3 = micros() - lPocetak;

  // BIT 2 - HIGH part
  lPocetak = micros();
  bh2:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 1) goto bh2;
  trajanje_HIGH_2 = micros() - lPocetak;
  // BIT 2 - LOW part
  lPocetak = micros();
  bl2:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 0) goto bl2;
  trajanje_LOW_2 = micros() - lPocetak;

  // BIT 1 - HIGH part
  lPocetak = micros();
  bh1:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 1) goto bh1;
  trajanje_HIGH_1 = micros() - lPocetak;
  // BIT 1 - LOW part
  lPocetak = micros();
  bl1:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 0) goto bl1;
  trajanje_LOW_1 = micros() - lPocetak;

  // BIT 0 - HIGH part
  lPocetak = micros();
  bh0:
  if (micros() - lPocetak > READ_BYTE_TIMEOUT)  // 1000uS
  {
    _ErrorCode = ERR_READBYTE_TIMEOUT; // More than 1000uS -> ReadByte timeout
    return 0;
  }
  if (digitalRead(_CommunicationPIN) == 1) goto bh0;
  trajanje_HIGH_0 = micros() - lPocetak;

  // Calculate recieved byte...
  // --------------------------
  RecievedByte = 0;
  // Calculate BIT 7
  if ((trajanje_HIGH_7 > (LONG_BIT_PART - LONG_BIT_TOLERANCE) && trajanje_HIGH_7 < (LONG_BIT_PART + LONG_BIT_TOLERANCE)) && (trajanje_LOW_7 > (SHORT_BIT_PART - SHORT_BIT_TOLERANCE) && trajanje_LOW_7 < (SHORT_BIT_PART + SHORT_BIT_TOLERANCE)))
  {
    RecievedByte = RecievedByte + 128;
  }

  // Calculate BIT 6
  if ((trajanje_HIGH_6 > (LONG_BIT_PART - LONG_BIT_TOLERANCE) && trajanje_HIGH_6 < (LONG_BIT_PART + LONG_BIT_TOLERANCE)) && (trajanje_LOW_6 > (SHORT_BIT_PART - SHORT_BIT_TOLERANCE) && trajanje_LOW_6 < (SHORT_BIT_PART + SHORT_BIT_TOLERANCE)))
  {
    RecievedByte = RecievedByte + 64;
  }

  // Calculate BIT 5
  if ((trajanje_HIGH_5 > (LONG_BIT_PART - LONG_BIT_TOLERANCE) && trajanje_HIGH_5 < (LONG_BIT_PART + LONG_BIT_TOLERANCE)) && (trajanje_LOW_5 > (SHORT_BIT_PART - SHORT_BIT_TOLERANCE) && trajanje_LOW_5 < (SHORT_BIT_PART + SHORT_BIT_TOLERANCE)))
  {
    RecievedByte = RecievedByte + 32;
  }

  // Calculate BIT 4
  if ((trajanje_HIGH_4 > (LONG_BIT_PART - LONG_BIT_TOLERANCE) && trajanje_HIGH_4 < (LONG_BIT_PART + LONG_BIT_TOLERANCE)) && (trajanje_LOW_4 > (SHORT_BIT_PART - SHORT_BIT_TOLERANCE) && trajanje_LOW_4 < (SHORT_BIT_PART + SHORT_BIT_TOLERANCE)))
  {
    RecievedByte = RecievedByte + 16;
  }

  // Calculate BIT 3
  if ((trajanje_HIGH_3 > (LONG_BIT_PART - LONG_BIT_TOLERANCE) && trajanje_HIGH_3 < (LONG_BIT_PART + LONG_BIT_TOLERANCE)) && (trajanje_LOW_3 > (SHORT_BIT_PART - SHORT_BIT_TOLERANCE) && trajanje_LOW_3 < (SHORT_BIT_PART + SHORT_BIT_TOLERANCE)))
  {
    RecievedByte = RecievedByte + 8;
  }

  // Calculate BIT 2
  if ((trajanje_HIGH_2 > (LONG_BIT_PART - LONG_BIT_TOLERANCE) && trajanje_HIGH_2 < (LONG_BIT_PART + LONG_BIT_TOLERANCE)) && (trajanje_LOW_2 > (SHORT_BIT_PART - SHORT_BIT_TOLERANCE) && trajanje_LOW_2 < (SHORT_BIT_PART + SHORT_BIT_TOLERANCE)))
  {
    RecievedByte = RecievedByte + 4;
  }

  // Calculate BIT 1
  if ((trajanje_HIGH_1 > (LONG_BIT_PART - LONG_BIT_TOLERANCE) && trajanje_HIGH_1 < (LONG_BIT_PART + LONG_BIT_TOLERANCE)) && (trajanje_LOW_1 > (SHORT_BIT_PART - SHORT_BIT_TOLERANCE) && trajanje_LOW_1 < (SHORT_BIT_PART + SHORT_BIT_TOLERANCE)))
  {
    RecievedByte = RecievedByte + 2;
  }

  // Calculate BIT 0
  if (trajanje_HIGH_0 > (LONG_BIT_PART - LONG_BIT_TOLERANCE) && trajanje_HIGH_0 < (LONG_BIT_PART + LONG_BIT_TOLERANCE))
  {
    RecievedByte = RecievedByte + 1;
  }
  
  _LastRecievedByte = RecievedByte;
  return RecievedByte;
}
// ****************************************************************



