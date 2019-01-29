#ifndef SF_OneWireCOM_h
  #include "Arduino.h"
  
  #define SF_OneWireCOM_h
  
  #define LONG_BIT_PART 75        // 75uS
  #define SHORT_BIT_PART 25       // 25uS
  #define LONG_BIT_TOLERANCE 20   // 15uS
  #define SHORT_BIT_TOLERANCE 20  // 15uS
  #define BYTE_BREAK 100          // Give slave time (100uS) to prepare for new byte...
  #define READ_BYTE_TIMEOUT 1000  // 1000uS

  #define ATTENTION_SIGNAL_HALFDURATION 100 // 100uS
  #define ATTENTION_SIGNAL_NO_OF_PULSES 50  // 50 pulses of 1 and 0
  #define ATTENTION_SIGNAL_BLANK 500        // 500uS
  #define ATTENTION_SIGNAL_TIMEOUT 1000     // 1000uS

  #define TXT_MESSAGE 0
  #define BYTE_MESSAGE 1
  
  #define SF_OWCOM_MASTER 0  // Class object type MASTER
  #define SF_OWCOM_SLAVE 1   // Class object type SLAVE
  
  #define SESSION_SendTXTDataToSlave 0
  #define SESSION_SendBYTEDataToSlave 1
  #define SESSION_SendTXTDataToMaster 2
  #define SESSION_SendBYTEDataToMaster 3
  #define SESSION_ERR_NO_SESSION 4
  #define SESSION_ERR_NO_MESSAGE 5
  #define SESSION_FAILED 6
  #define SESSION_SUCCESS 7
  #define SESSION_DEBUG 252
  #define SESSION_ERR_UNKNOWN_SESSION 8
  #define SESSION_ERR_NO_MASTER 9
  #define SESSION_ERR_MESSAGE_CHECKSUM 10
  #define SESSION_ERR_MESSAGE_TOO_BIG 11

  #define ERR_OK_NOERROR 0
  #define ERR_READBYTE_TIMEOUT 1
  #define ERR_DETECT_ATTENTIONSIGNAL_TIMEOUT 2
  #define ERR_SLAVE_TIMEOUT 3
  
  #define MESSAGE_BUFFERS_SIZE 256
  class SF_OneWireCOM
  {
    public:
      // Input/Output Message buffers
      char SFOneWireCOM_InputBuffer_TXT[MESSAGE_BUFFERS_SIZE];
      byte SFOneWireCOM_InputBuffer_BYTE[MESSAGE_BUFFERS_SIZE];
      char SFOneWireCOM_OutputBuffer_TXT[MESSAGE_BUFFERS_SIZE];
      byte SFOneWireCOM_OutputBuffer_BYTE[MESSAGE_BUFFERS_SIZE];

      // Constructors
      SF_OneWireCOM(); // SlaveID set to '0', 
                       // Communication PIN set to '0',
                       // SlaveMaster set to 'SFOW_MASTER'
      SF_OneWireCOM(byte aCommunicationPIN, byte aMasterSlave);
      
      // Private variables functions
      void SetSlaveID(byte aSlaveID);
      byte GetSlaveID();
      void SetCommunicationPIN(byte aCommunicationPIN);
      byte GetCommunicationPIN();
      void SetMasterSlave(byte aMasterSlave);
      byte GetMasterSlave();
	  String GetSoftwareSerial();
	  String GetLibraryName();
	  String GetCopyright();
	  
      // Protocol support functions
      void CREATE_AttentionSignal();
      byte DETECT_AttentionSignal();
      void CallSlave();
      byte WaitForSlaveToReply();
      byte ReadErrorCode();
      byte GetLastRecievedByte();
	  int CalculateTXTMessageChecksum(char * aMessage, int aMessageSize);
	  int CalculateBYTEMessageChecksum(byte * aMessage, int aMessageSize);
	  int RecieveMessageChecksum();
	  void SendMessageChecksum(int aChecksum);
	  // Protocol implementation functions
	  byte SESSION_Launch(byte aSlaveID, byte aSessionType);
	  void SESSION_Debug(byte aSlaveID);
	  byte SESSION();
	  byte SESSION_SendTXTData_To_Slave(byte aSlaveID, char * aMessage, int aMessageLength);
	  byte SESSION_SendBYTEData_To_Slave(byte aSlaveID, byte * aMessage, int aMessageLength);
      byte SESSION_RecieveTXTData_From_Slave(byte aSlaveID);
      byte SESSION_RecieveBYTEData_From_Slave(byte aSlaveID);

	  // Buffers functions
	  void ClearAllBuffers();
	  void ClearInputTXTBuffer();
	  void ClearInputBYTEBuffer();
	  void ClearOutputTXTBuffer();
	  void ClearOutputBYTEBuffer();
	  void CopyTXTMessageToOutputBuffer(char * aMessage, int aMessageLength);
	  void CopyBYTEMessageToOutputBuffer(byte * aMessage, int aMessageLength);
	  int GET_InputTXTMessageSize();
	  int GET_InputBYTEMessageSize();
	  int GET_OutputTXTMessageSize();
	  int GET_OutputBYTEMessageSize();
	  
      // Physical layer functions
	  void PrepareTXTMessageForMaster(char * aMessage, int aMessageSize);
	  void PrepareBYTEMessageForMaster(byte * aMessage, int aMessageSize);
      void SendTxtMessage(char * aMessage, int aMessageLength);
      void SendByteMessage(byte * aMessage, int aMessageLength);
      void RecieveTxtMessage(int aMessageLength);
      void RecieveByteMessage(int aMessageLength);
      void WriteByte(byte aByteToSend);
      byte ReadByte();

    private:
      // Variables
      byte _CommunicationPIN;
      byte _SlaveID;
      byte _MasterSlave;
      String _SOFTWARE_SERIAL = "V7527.0408";
	  String _LIBRARY_NAME = "SF-Logic OneWireCOM";
	  String _SF_COPYRIGHT = "Copyright SF-Logic";
      byte _ErrorCode;
      byte _LastRecievedByte;
	  
	  int _InputTXTMessageSize;
	  int _InputBYTEMessageSize;
	  int _OutputTXTMessageSize;
	  int _OutputBYTEMessageSize;
	  
	  // Debug SESSION variables
      byte DebugNoOfReadByteErrors = 0;
      byte DebugNoOfSessionErrors = 0;
	  byte DebugByte = 0;
  };

#endif


