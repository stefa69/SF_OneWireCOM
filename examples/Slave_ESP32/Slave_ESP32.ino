
// ================================================================
// ================================================================
// ================================================================
//
//  SF-Logic OneWireCOM library (SLAVE) Example
// --------------------------------------------
//
// Author: Stefanovic Srdjan
// Work start: Mart 2018., 7526. дерикожа
//
// Compatible devices
// -----------------------------------------------------------------------------
// - NODEMCU 0.9 sa ESP-12 plocicom (ESP8266)
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
#define DEBUG_BUILD         // Serial debug
// ****************************************************************


// ****************************************************************
// ****************************************************************
// Globalne promenljive...
// ****************************************************************

// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void setup() 
{
  // Initialize SERIAL communication...
  // ----------------------------------
  Serial.begin(115200); 
  while(!Serial) { }

  // Create SLAVE object...
  // ESP8266
//  SF_OneWireCOM SLA(D0, SF_OWCOM_SLAVE);
  // ESP32
  SF_OneWireCOM SLA(17, SF_OWCOM_SLAVE);
  // Display DEVICE "Hello Screen"...
  Serial.println("");
  Serial.println("********************************");
  Serial.println(" SF-Logic OneWire communication ");
  Serial.println(" Role: Slave Device             ");
  Serial.print(" Software SN: ");
  Serial.println(SLA.GetSoftwareSerial());
  Serial.print(" Library: ");
  Serial.println(SLA.GetLibraryName());
  Serial.print(" Copyright: ");
  Serial.println(SLA.GetCopyright());
  Serial.println("********************************");
  // Set SLAVE ID...
  SLA.SetSlaveID(48);
  
  char tempMessage[] = "Probna poruka (SLAVE)...";
  byte tempByteMessage[] = {1,2,3,4,5,6,7,8,9,10};

  int i = 0;
  int j = 0;
  int temp_SessionReply;
  int CASE0, CASE1, CASE2, CASE3, CASE4, CASE5, CASE252, CASE8, CASE9;
  CASE0 = 0;
  CASE1 = 0;
  CASE2 = 0;
  CASE3 = 0;
  CASE4 = 0;
  CASE5 = 0;
  CASE252 = 0;
  CASE8 = 0;
  CASE9 = 0;

  byte startStatisticsCount = 0;
  do
  {
//    SLA.PrepareBYTEMessageForMaster(tempByteMessage, sizeof(tempByteMessage));
//    SLA.PrepareTXTMessageForMaster(tempMessage, sizeof(tempMessage));
    temp_SessionReply = SLA.SESSION();
    switch (temp_SessionReply)
    {
      case 0:   // SESSION_SendTXTDataToSlave
          startStatisticsCount = 1;
          CASE0 += 1;
          Serial.print(CASE0);
          Serial.print(": ");
          Serial.println(SLA.SFOneWireCOM_InputBuffer_TXT);
          break;
      case 1:   // SESSION_SendBYTEDataToSlave
          startStatisticsCount = 1;
          CASE1 += 1;
          break;
      case 2:   // SESSION_SendTXTDataToMaster
          startStatisticsCount = 1;
          CASE2 += 1;
          Serial.println("2");
          break;
      case 3:   // SESSION_SendBYTEDataToMaster
          startStatisticsCount = 1;
          CASE3 += 1;
          break;
      case 4:   // SESSION_ERR_NO_SESSION
          startStatisticsCount = 1;
          CASE4 += 1;
          break;
      case 5:   // SESSION_ERR_NO_MESSAGE
          startStatisticsCount = 1;
          CASE5 += 1;
          break;
      case 252: // SESSION_DEBUG
          startStatisticsCount = 1;
          CASE252 += 1;
          break;
      case 8:   // SESSION_ERR_UNKNOWN_SESSION
          startStatisticsCount = 1;
          CASE8 += 1;
          break;
      case 9:   // SESSION_ERR_NO_MASTER
          if (startStatisticsCount == 1) // Wait for Master to be alive...
          {
            CASE9 += 1;
          }
          break;
    }

    // Display summarized SESSION statistics - Example
    // -----------------------------------------------
    if (startStatisticsCount == 1)
    {
      i += 1;
      if (i == 10000)
      {
        j += 1;
        i = 0;
        Serial.println("-------------------------------------------");
        Serial.print(j*10000);
        Serial.println(" SLAVE cycles");
        Serial.println("-------------------------------------------");
        Serial.print("SESSION_SendTXTDataToSlave: ");
        Serial.println(CASE0);
        Serial.print("SESSION_SendBYTEDataToSlave: ");
        Serial.println(CASE1);
        Serial.print("SESSION_SendTXTDataToMaster: ");
        Serial.println(CASE2);
        Serial.print("SESSION_SendBYTEDataToMaster: ");
        Serial.println(CASE3);
        Serial.print("SESSION_ERR_NO_SESSION: ");
        Serial.println(CASE4);
        Serial.print("SESSION_ERR_NO_MESSAGE: ");
        Serial.println(CASE5);
        Serial.print("SESSION_DEBUG: ");
        Serial.println(CASE252);
        Serial.print("SESSION_ERR_UNKNOWN_SESSION: ");
        Serial.println(CASE8);
        Serial.print("SESSION_ERR_NO_MASTER: ");
        Serial.println(CASE9);
        Serial.println("-------------------------------------------");
        Serial.println("");
      }
    }
    yield();
  } while(1);

  
}
// ****************************************************************



// ****************************************************************
// ****************************************************************
// ****************************************************************
void loop()
{

}
// ****************************************************************
