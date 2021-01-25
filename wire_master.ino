//#include <Streaming.h>         
//#include <Ethernet.h>          
//#include <SPI.h>
//#include <MemoryFree.h>
//#include <Agentuino.h> 
//#include <Flash.h>
//#include <SoftwareSerial.h>
//#include <AD9959.h>
#include <Wire.h>

/*static byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static byte ip[] = { 192, 168, 2, 155 };
static byte RemoteIP[4] = {192, 168, 88, 184};*/ //The IP address of the host that will receive the trap
/*byte message_ADD[] = {0x7E, 0xFF, 0x02, 0x03, 0x04, 0x05, 0x7F}; //MODBUS RTU QUERY COMMAND FOR DEVICE ADDRESS
byte message_TEMP[] = {0x7E, 0x1F, 0x02, 0x06, 0x06, 0xE2, 0x7F}; //MODBUS RTU QUERY COMMAND FOR DEVICE TEMPERATURE
byte message_PSV[] = {0x7E, 0x1F, 0x02, 0x06, 0x10, 0xF4, 0x7F};  //MODBUS RTU QUERY COMMAND FOR DEVICE POWER SUPPLY VOLTAGE
byte message_ADD1[] = {0x7E, 0xFF, 0x02, 0x03, 0x04, 0x72, 0x7F}; // MODBUS RTU QUERY COMMAND FOR DEVICE ADDRESS CRC CALCULAT*/
//static byte gateway[] = { 192, 168, 20, 1 };
//static byte subnet[] = { 255, 255, 255, 0 };

//
// tkmib - linux mib browser
//
// RFC1213-MIB OIDs
// .iso (.1)
// .iso.org (.1.3)
// .iso.org.dod (.1.3.6)
// .iso.org.dod.internet (.1.3.6.1)
// .iso.org.dod.internet.mgmt (.1.3.6.1.2)
// .iso.org.dod.internet.mgmt.mib-2 (.1.3.6.1.2.1)
// .iso.org.dod.internet.mgmt.mib-2.system (.1.3.6.1.2.1.1)
// .iso.org.dod.internet.mgmt.mib-2.system.sysDescr (.1.3.6.1.2.1.1.1)
static const char sysDescr1[] PROGMEM      = "1.3.6.1.2.1.1.1.0";  // read-only  (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysObjectID (.1.3.6.1.2.1.1.2)
//static char sysObjectID[] PROGMEM   = "1.3.6.1.2.1.1.2.0";  // read-only  (ObjectIdentifier)
// .iso.org.dod.internet.mgmt.mib-2.system.sysUpTime (.1.3.6.1.2.1.1.3)
static const char sysUpTime1[] PROGMEM     = "1.3.6.1.2.1.1.3.0";  // read-only  (TimeTicks)
// .iso.org.dod.internet.mgmt.mib-2.system.sysContact (.1.3.6.1.2.1.1.4)
static const char sysContact1[] PROGMEM    = "1.3.6.1.2.1.1.4.0";  // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysName (.1.3.6.1.2.1.1.5)
static const char sysName1[] PROGMEM       = "1.3.6.1.2.1.1.5.0";  // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysLocation (.1.3.6.1.2.1.1.6)
static const char sysLocation1[] PROGMEM   = "1.3.6.1.2.1.1.6.0";  // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysServices (.1.3.6.1.2.1.1.7)
//static const char sysTemp[] PROGMEM       = "1.3.6.1.2.1.1.8.0";
// .iso.org.dod.internet.mgmt.mib-2.system.sysTemperature (.1.3.6.1.2.1.1.8)
static const char sysServices1[] PROGMEM   = "1.3.6.1.2.1.1.7.0";  // read-only  (Integer)
static const char sysGain[] PROGMEM        = "1.3.6.1.2.1.1.10.2";
static const char sysGainP[] PROGMEM       = "1.3.6.1.2.1.1.10.3";
static const char sysErrAmpAz[] PROGMEM    = "1.3.6.1.2.1.1.10.4";
static const char sysErrAmpPAz[] PROGMEM   = "1.3.6.1.2.1.1.10.5";
static const char sysErrAmpEl[] PROGMEM    = "1.3.6.1.2.1.1.10.6";
static const char sysErrAmpPEl[] PROGMEM   = "1.3.6.1.2.1.1.10.7";
//
// Arduino defined OIDs
// .iso.org.dod.internet.private (.1.3.6.1.4)
// .iso.org.dod.internet.private.enterprises (.1.3.6.1.4.1)
// .iso.org.dod.internet.private.enterprises.arduino (.1.3.6.1.4.1.36582)
//
//
// RFC1213 local values
static char locDescr1[]              = "Monitor and control";  // read-only (static)
//static char locObjectID[]          = "1.3.6.1.3.2009.0";                       // read-only (static)
static uint32_t locUpTime1           = 0;                                        // read-only (static)
static char locContact1[20]          = "";                         // should be stored/read from EEPROM - read/write (not done for simplicity)
static char locName1[20]             = "Agentuino";                              // should be stored/read from EEPROM - read/write (not done for simplicity)
static char locLocation1[20]         = "Italy";                         // should be stored/read from EEPROM - read/write (not done for simplicity)
//static int32_t locTemp             = 34;
//int32_t locTemp;
static char setName[20]             = "";
static int32_t locServices1         = 6;                                        // read-only (static)
char *locGain  = (char *)malloc(15+1 * sizeof(char));
char *locGainP = (char *)malloc(15+1 * sizeof(char));
char *locErrAmpAz  = (char *)malloc(15+1 * sizeof(char));
char *locErrAmpPAz = (char *)malloc(15+1 * sizeof(char));
char *locErrAmpEl  = (char *)malloc(15+1 * sizeof(char));
char *locErrAmpPEl = (char *)malloc(15+1 * sizeof(char));
char *header_start = (char *)malloc(2+1 * sizeof(char));
char *header_end = (char *)malloc(2+1 * sizeof(char));
char *header_1 = (char *)malloc(2+1 * sizeof(char));
char *header_2 = (char *)malloc(2+1 * sizeof(char));
char *header_3 = (char *)malloc(2+1 * sizeof(char));
char *header_4 = (char *)malloc(2+1 * sizeof(char));
char *header_5 = (char *)malloc(2+1 * sizeof(char));
char *header_6 = (char *)malloc(2+1 * sizeof(char));


uint32_t prevMillis = millis();
char oid[SNMP_MAX_OID_LEN];
SNMP_API_STAT_CODES api_status;
SNMP_ERR_CODES status;*/
const int csbPin = 7;
const int IO_UpdatePin = 2;
const int resetPin = 10;
const int SDIO3Pin = 3;
AD9959 myDDS = AD9959(resetPin, IO_UpdatePin, csbPin, SDIO3Pin);

long testFreq = 50; //MHz
long fs = 500; // ref clk in MHz
long testFrequencyTuningWord = (((pow(2,32))*testFreq)/fs);*/




void setup() {

  
  Serial.begin(9600);
  //Ethernet.begin(mac, ip);
 // myDDS.initialize();
  Wire.begin();

  
     cpy();
   
  

  
   /*api_status = Agentuino.begin();
  //
  if ( api_status == SNMP_API_STAT_SUCCESS ) {
    //
    Agentuino.onPduReceive(pduReceived);
    //
    delay(10);
    //
    return;
  }
  delay(10);

*/

  
}

void cpy(){
  
 strcpy(locGain, "1");
  strcpy(locGainP, "1");
  strcpy(locErrAmpAz, "180.0");
  strcpy(locErrAmpPAz, "180.0");
  strcpy(locErrAmpEl, "180.0");
  strcpy(locErrAmpPEl, "180.0");
  strcpy(header_start,"~S");
  strcpy(header_1,"A1");
  strcpy(header_2,"A2");
  strcpy(header_3,"A3");
  strcpy(header_4,"A4");
  strcpy(header_5,"A5");
  strcpy(header_6,"A6");
  strcpy(header_end,"~E");
  
}

void pduReceived()
{
  SNMP_PDU pdu;
  //
  #ifdef DEBUG
    Serial << F("UDP Packet Received Start..") << F(" RAM:") << freeMemory() << endl;
  #endif
  //
  api_status = Agentuino.requestPdu(&pdu);
  //
  if ( pdu.type == SNMP_PDU_GET || pdu.type == SNMP_PDU_GET_NEXT || pdu.type == SNMP_PDU_SET
    && pdu.error == SNMP_ERR_NO_ERROR && api_status == SNMP_API_STAT_SUCCESS ) {
    //
    pdu.OID.toString(oid);
    //
    Serial << "OID: " << oid << endl;
    //
    if ( strcmp_P(oid, sysDescr1 ) == 0 ) {
  // handle sysDescr (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read-only
    pdu.VALUE.encode(SNMP_SYNTAX_NULL);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = SNMP_ERR_READ_ONLY;
  } else {
    // response packet from get-request - locDescr
    status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locDescr1);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysDescr...") << locDescr1 << F(" ") << pdu.VALUE.size << endl;
  #endif
    } else if ( strcmp_P(oid, sysUpTime1 ) == 0 ) {
  // handle sysName (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read-only
    pdu.VALUE.encode(SNMP_SYNTAX_NULL);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = SNMP_ERR_READ_ONLY;
  } else {
    // response packet from get-request - locUpTime
    status = pdu.VALUE.encode(SNMP_SYNTAX_TIME_TICKS, locUpTime1);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysUpTime...") << locUpTime1 << F(" ") << pdu.VALUE.size << endl;
  #endif
    } else if ( strcmp_P(oid, sysName1 ) == 0 ) {
  // handle sysName (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read/write
    status = pdu.VALUE.decode(locName, strlen(locName1));
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
    int a;
    for(a=0;a<19;a++){
      Serial.print(locName1[a]);
    }
  } else {
    // response packet from get-request - locName
    status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locName);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysName...") << locName1 << F(" ") << pdu.VALUE.size << endl;
  #endif
    } else if ( strcmp_P(oid, sysContact1 ) == 0 ) {
  // handle sysContact (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read/write
    status = pdu.VALUE.decode(locContact, strlen(locContact1));
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  } else {
    // response packet from get-request - locContact
    status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locContact1);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysContact...") << locContact1 << F(" ") << pdu.VALUE.size << endl;
  #endif
    } else if ( strcmp_P(oid, sysLocation1 ) == 0 ) {
  // handle sysLocation (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read/write
    status = pdu.VALUE.decode(locLocation1, strlen(locLocation1));
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  } else {
    // response packet from get-request - locLocation
    status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locLocation1);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysLocation...") << locLocation1 << F(" ") << pdu.VALUE.size << endl;
  #endif
    }else if ( strcmp_P(oid, sysGain ) == 0 ) {
  // handle sysName (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read/write
    status = pdu.VALUE.decode(locGain, strlen(locGain));
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  } else {
    // response packet from get-request - locName
    status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locGain);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysGain...") << locGain << F(" ") << pdu.VALUE.size << endl;
  #endif
    }else if ( strcmp_P(oid, sysGainP) == 0 ) {
  // handle sysName (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read/write
    status = pdu.VALUE.decode(locGainP, strlen(locGainP));
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  } else {
    // response packet from get-request - locName
    status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locGainP);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysGainP...") << locGainP << F(" ") << pdu.VALUE.size << endl;
  #endif
    }else if ( strcmp_P(oid, sysErrAmpAz) == 0 ) {
  // handle sysName (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read/write
    status = pdu.VALUE.decode(locErrAmpAz, strlen(locErrAmpAz));
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  } else {
    // response packet from get-request - locName
    status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locErrAmpAz);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysErrAmpAz...") << locErrAmpAz << F(" ") << pdu.VALUE.size << endl;
  #endif
    }else if ( strcmp_P(oid, sysErrAmpPAz) == 0 ) {
  // handle sysName (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read/write
    status = pdu.VALUE.decode(locErrAmpPAz, strlen(locErrAmpPAz));
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  } else {
    // response packet from get-request - locName
    status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locErrAmpPAz);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysErrAmpPAz...") << locErrAmpPAz << F(" ") << pdu.VALUE.size << endl;
  #endif
    }else if ( strcmp_P(oid, sysErrAmpEl) == 0 ) {
  // handle sysName (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read/write
    status = pdu.VALUE.decode(locErrAmpEl, strlen(locErrAmpEl));
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  } else {
    // response packet from get-request - locName
    status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locErrAmpEl);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysErrAmpEl...") << locErrAmpEl << F(" ") << pdu.VALUE.size << endl;
  #endif
    }else if ( strcmp_P(oid, sysErrAmpPEl) == 0 ) {
  // handle sysName (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read/write
    status = pdu.VALUE.decode(locErrAmpPEl, strlen(locErrAmpPEl));
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  } else {
    // response packet from get-request - locName
    status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locErrAmpPEl);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("sysErrAmpPEl...") << locErrAmpPEl << F(" ") << pdu.VALUE.size << endl;
  #endif
    }else if ( strcmp_P(oid, sysServices1) == 0 ) {
  // handle sysServices (set/get) requests
  if ( pdu.type == SNMP_PDU_SET ) {
    // response packet from set-request - object is read-only
    pdu.VALUE.encode(SNMP_SYNTAX_NULL);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = SNMP_ERR_READ_ONLY;
  } else {
    // response packet from get-request - locServices
    status = pdu.VALUE.encode(SNMP_SYNTAX_INT, locServices1);
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
  }
  //
  #ifdef DEBUG
    Serial << F("locServices...") << locServices1 << F(" ") << pdu.VALUE.size << endl;
  #endif
    } else {
  // oid does not exist
  //
  // response packet - object not found
  pdu.VALUE.encode(SNMP_SYNTAX_NULL);
  pdu.type = SNMP_PDU_RESPONSE;
  pdu.error = SNMP_ERR_NO_SUCH_NAME;
    }
    //
    Agentuino.responsePdu(&pdu);
  }
  //
  Agentuino.freePdu(&pdu);
  //
  Serial << "UDP Packet Received End.." << " RAM:" << freeMemory() << endl;
} 



void loop() {

   
  Agentuino.listen();


 
  

  Wire.beginTransmission(9);
  Wire.write(header_start);
  Wire.endTransmission();
   
  Wire.beginTransmission(9);
  Wire.write(header_1);
  Wire.write(locGain);
  Wire.endTransmission();
  Serial.println(locGain);

  Wire.beginTransmission(9);
  Wire.write(header_2);
  Wire.write(locGainP);
  Wire.endTransmission();
   Serial.println(locGainP);

  Wire.beginTransmission(9);
  Wire.write(header_3);
  Wire.write(locErrAmpAz);
  Wire.endTransmission();
    Serial.println(locErrAmpAz);

  Wire.beginTransmission(9);
  Wire.write(header_4);
  Wire.write(locErrAmpPAz);
  Wire.endTransmission();
    Serial.println(locErrAmpPAz);

  Wire.beginTransmission(9);
  Wire.write(header_5);
  Wire.write(locErrAmpEl);
  Wire.endTransmission();
    Serial.println(locErrAmpEl);

   Wire.beginTransmission(9);
  Wire.write(header_6);
  Wire.write(locErrAmpPEl);
  Wire.endTransmission();
    Serial.println(locErrAmpEl);
  
  Wire.beginTransmission(9);
  Wire.write(header_end);
  Wire.endTransmission();
 
    
  

   

}
