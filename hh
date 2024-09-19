Code
#include <painlessMesh.h>
#include <Arduino_JSON.h>
//#define BME_280
#define DHT22
//#define DS18B20
//#define ENABLE_LOG
String nodeName = "NODE_4"; // Name needs to be unique
float temp(NAN), hum(NAN), pres(NAN);
//########################## Init_BME280 ##########################
#ifdef BME_280
#include <BME280I2C.h>
#include <Wire.h>
BME280I2C bme;
BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
BME280::PresUnit presUnit(BME280::PresUnit_Pa);
#endif
//__________________________ End of _BME280 __________________________
//########################## Init_DHT22 ##########################
#ifdef DHT22
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#endif
//__________________________ End of DHT22 __________________________
//########################## Init_Ds18B20 ##########################
#ifdef DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature ds18b20(&oneWire);
#endif
//__________________________ End of DHT22 __________________________
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
JSONVar myVar;
void sendMessage() {
  String msg = construnct_json();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}
void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}
void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
void setup()
{
  Serial.begin(115200);
  Serial.println(nodeName);
//  mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
#ifdef BME_280
  Wire.begin();
  while (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
  // bme.chipID(); // Deprecated. See chipModel().
  switch (bme.chipModel())
  {
    case BME280::ChipModel_BME280:
      Serial.println("Found BME280 sensor! Success.");
      break;
    case BME280::ChipModel_BMP280:
      Serial.println("Found BMP280 sensor! No Humidity available.");
      break;
    default:
      Serial.println("Found UNKNOWN sensor! Error!");
  }
#endif
#ifdef DHT22
  Serial.println(F("DHTxx test!"));
  dht.begin();
#endif
#ifdef DS18B20
  ds18b20.begin();
#endif
}
void loop()
{
  mesh.update();
  // construnct_json();
}
String construnct_json()
{
#ifdef BME_280
  bme.read(pres, temp, hum, tempUnit, presUnit); // update with new values
  pres = pres / 100;
  myVar["Sensor Type"] = "BME280";
  myVar["Node Name"] = nodeName;
  myVar["Temperature"] = serialized(String(temp, 2)); // serialized need to conver flot values
  myVar["pres"] = serialized(String(pres, 2));// serialized need to conver flot values
#ifdef ENABLE_LOG
  Serial.println(JSON.stringify(myVar)); //stringify converts the arry to a string
#endif
  return JSON.stringify(myVar);
#endif
#ifdef DHT22
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  myVar["Sensor Type"] = "DHT22";
  myVar["Node Name"] = nodeName;
  myVar["Temperature"] = serialized(String(temp));
  myVar["Humidity"] = serialized(String(hum));
#ifdef ENABLE_LOG
  Serial.println(JSON.stringify(myVar));
#endif
  return JSON.stringify(myVar);
#endif
#ifdef DS18B20
  ds18b20.requestTemperatures();
  temp = ds18b20.getTempCByIndex(0);
  myVar["Sensor Type"] = "DS18B20";
  myVar["Node Name"] = nodeName;
  myVar["Temperature"] = serialized(String(temp));
#ifdef ENABLE_LOG
  Serial.println(JSON.stringify(myVar));
#endif
  return JSON.stringify(myVar);
#endif
}