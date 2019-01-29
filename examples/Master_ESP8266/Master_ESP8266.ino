
// ================================================================
// ================================================================
// ================================================================
//
//  SF-Logic OneWireCOM library (MASTER) Example
// ---------------------------------------------
//
// Author: Stefanovic Srdjan
// Development from: Mart 2018., 7526. дерикожа
//
// Compatible devices...
// -----------------------------------------------------------------------------
// - ESP8266
// - WEMOS LOLIN32 (ESP32)
//
// ================================================================
// ================================================================
// ================================================================


// ****************************************************************
// ****************************************************************
// INCLUDEs...
// ****************************************************************
#include <SF_OneWireCOM.h>

// ****************************************************************


// ****************************************************************
// ****************************************************************
// #DEFINE section...
// ****************************************************************
#define DEBUG_BUILD // Serial debug
// ****************************************************************


// ****************************************************************
// ****************************************************************
// Global vars...
// ****************************************************************
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void setup() 
{
  // Initialize SERIAL communication...
  // ----------------------------------
//  Serial.begin(9600); 
  Serial.begin(115200); 
  while(!Serial) { }

  // Create MASTER object...
  // ESP8266
  SF_OneWireCOM MASTER(D0, SF_OWCOM_MASTER);
  // Display DEVICE "Hello Screen"...
  Serial.println("");
  Serial.println("********************************");
  Serial.println(" SF-Logic OneWire communication ");
  Serial.println(" Role: Master Device            ");
  Serial.print(" Software SN: ");
  Serial.println(MASTER.GetSoftwareSerial());
  Serial.print(" Library: ");
  Serial.println(MASTER.GetLibraryName());
  Serial.print(" Copyright: ");
  Serial.println(MASTER.GetCopyright());
  Serial.println("********************************");

  char tempMessage[] = "Test message...Test message...Test message...Test message...Test message...Test message...Test message...Test message...";
  byte tempByteMessage[] = {11,12,13,14,15,16,17,18,19,20};
  
  int lChecksum = 0;
  int lNoSession = 0;
  int lSuccess = 0;



//  // Send TXT/BYTE data to Slave - Session example
//  // ---------------------------------------------
//  do
//  {
//    switch (MASTER.SESSION_SendTXTData_To_Slave(48, tempMessage, sizeof(tempMessage)))
////    switch (MASTER.SESSION_SendBYTEData_To_Slave(48, tempByteMessage, sizeof(tempByteMessage)))
//    {
//      case 11:  // SESSION_ERR_MESSAGE_TOO_BIG
//        Serial.print("SESSION_ERR_MESSAGE_TOO_BIG: ");
//        Serial.print(sizeof(tempByteMessage));
//        Serial.println("bytes");
//        break;
//      case 10:  // SESSION_ERR_MESSAGE_CHECKSUM
//        lChecksum += 1;
//        Serial.print("SESSION_ERR_MESSAGE_CHECKSUM: ");
//        Serial.println(lChecksum);
//        break;
//      case 4:   // SESSION_ERR_NO_SESSION
//        lNoSession += 1;
//        Serial.print("SESSION_ERR_NO_SESSION: ");
//        Serial.println(lNoSession);
//        break;
//      case 7:   // SESSION_SUCCESS
//        lSuccess += 1;
//        Serial.print("SESSION_SUCCESS: ");
//        Serial.println(lSuccess);
//        break;
//      default:
//        break;
//    }
//    yield();
//  } while (lSuccess < 10);




  
//  // Recieve TXT data from Slave - Session example
//  // ---------------------------------------------
//  do
//  {
//    switch (MASTER.SESSION_RecieveTXTData_From_Slave(48))
//    {
//      case 5: // SESSION_ERR_NO_MESSAGE
////          Serial.println("NO MESSAGE");
//          break;
//      case 6: // SESSION_FAILED
////          Serial.println("FAILED");
//          break;
//      case 7: // SESSION_SUCCESS
//          lSuccess += 1;
//          Serial.println(MASTER.SFOneWireCOM_InputBuffer_TXT);
//          break;
//    }
//    yield();
//  } while(lSuccess < 10);



  // Recieve BYTE data from Slave - Session example
  // ----------------------------------------------
  do
  {
    switch (MASTER.SESSION_RecieveBYTEData_From_Slave(48))
    {
      case 5: // SESSION_ERR_NO_MESSAGE
//          Serial.println("NO MESSAGE");
          break;
      case 6: // SESSION_FAILED
//          Serial.println("FAILED");
          break;
      case 7: // SESSION_SUCCESS
          lSuccess += 1;
          //Serial.println("Recieved BYTEs from Slave");
          Serial.print(MASTER.SFOneWireCOM_InputBuffer_BYTE[0], HEX);
          Serial.print(", ");
          Serial.print(MASTER.SFOneWireCOM_InputBuffer_BYTE[1]);
          Serial.print(", ");
          Serial.print(MASTER.SFOneWireCOM_InputBuffer_BYTE[2]);
          Serial.print(", ");
          Serial.print(MASTER.SFOneWireCOM_InputBuffer_BYTE[3]);
          Serial.print(", ");
          Serial.print(MASTER.SFOneWireCOM_InputBuffer_BYTE[4]);
          Serial.print(", ");
          Serial.print(MASTER.SFOneWireCOM_InputBuffer_BYTE[5]);
          Serial.print(", ");
          Serial.print(MASTER.SFOneWireCOM_InputBuffer_BYTE[6]);
          Serial.print(", ");
          Serial.print(MASTER.SFOneWireCOM_InputBuffer_BYTE[7]);
          Serial.print(", ");
          Serial.print(MASTER.SFOneWireCOM_InputBuffer_BYTE[8]);
          Serial.print(", ");
          Serial.println(MASTER.SFOneWireCOM_InputBuffer_BYTE[9]);
          break;
    }
    yield();
  } while(lSuccess < 10);



//  // DEBUG - Session example
//  // -----------------------
//  int i = 0;
//  int j = 0;
//  do
//  {
//    delay(1);
//    MASTER.SESSION_Debug(48);
//    i += 1;
//    if (i == 1000)
//    {
//      j += 1;
//      i = 0;
//      Serial.print(j*1000);
//      Serial.print(", ReadByte Errors: ");
//      Serial.print(MASTER.DebugNoOfReadByteErrors);
//      Serial.print(", Session Errors: ");
//      Serial.println(MASTER.DebugNoOfSessionErrors);
//    }
//    yield();
//  } while(1);

}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void loop()
{
  
}
// ****************************************************************
