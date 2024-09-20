#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <HTTPClient.h>

// Definir los pines analógicos donde están conectados los sensores
const int sensorPinPH = A1;   // Pin A1 para el sensor de pH
const int sensorPinTurb = A2; // Pin A2 para el sensor de turbidez
const int sensorPinOxygen = A3; // Pin A3 para el sensor de oxígeno disuelto
const int sensorPinConductividad = A4; // Pin A4 para el sensor de conductividad
// Definir los pines para la comunicación serial con el módulo SIM7600G-H
//enviar y recibir datos
const int sim7600Tx = 7;  // Arduino TX -> SIM7600G-H RX
const int sim7600Rx = 8;  // Arduino RX -> SIM7600G-H TX
// Crear una instancia de SoftwareSerial para comunicar con el módulo SIM7600G-H
SoftwareSerial sim7600(sim7600Rx, sim7600Tx);
// Capacidad del JsonDocument ajustada para manejar los sensores
const size_t capacity = JSON_OBJECT_SIZE(8) + 150;

// Credenciales del broker MQTT de Fogwing
String MQTT_BROKER = "iothub.fogwing.net";
String CLIENT_ID = "1151-2749-2393-3534";
String USER_NAME = "5357f0b2ef6344f4";
String PASSWORD = "Mqobk7";
String PUB_EDGE = "fwent/edge/5357f0b2ef6344f4/inbound";

// Variables globales para almacenar los valores de los sensores
int globalPHValue = 0;
int globalTurbValue = 0;
float globalOxygenConcentration = 0.0;
float globalOxygenSaturation = 0.0;
float globalConductivity = 0.0;

void setup() {
  // Configuración inicial de los pines de los sensores
  pinMode(sensorPinPH, INPUT);    
  pinMode(sensorPinTurb, INPUT);  
  pinMode(sensorPinOxygen, INPUT);  
  pinMode(sensorPinConductividad, INPUT);  
  Serial.begin(9600);             // Inicia la comunicación serial
  sim7600.begin(9600);            // Inicia la comunicación serial con el módulo SIM7600G-H
  sim7600.println("AT");                      // Verificar que el módulo responde
  delay(1000);
  sim7600.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  // Seleccionar GPRS
  delay(1000);
  sim7600.println("AT+SAPBR=3,1,\"APN\",\"movistar.co\""); // APN de Movistar Colombia
  delay(1000);
  sim7600.println("AT+SAPBR=3,1,\"USER\",\"MOVISTAR\"");  // Usuario para el APN
  delay(1000);
  sim7600.println("AT+SAPBR=3,1,\"PWD\",\"MOVISTAR\"");   // Contraseña para el APN
  delay(1000);
  sim7600.println("AT+SAPBR=1,1");  // Activar el perfil de datos
  delay(3000);
  sim7600.println("AT+SAPBR=2,1");            // Verifica la conexión
  delay(1000);

}

void loop() {
  // Leer valores de los sensores
  int localPHValue = analogRead(sensorPinPH);
  int localTurbValue = analogRead(sensorPinTurb);
  float oxygenVoltage = analogRead(sensorPinOxygen) * (5.0 / 1023.0);
  int localConductivityValue = analogRead(sensorPinConductividad);
  // Asignar los valores leídos a las variables globales
  globalPHValue = localPHValue;
  globalTurbValue = localTurbValue;
  globalOxygenConcentration = convertirAConcentracion(oxygenVoltage);
  globalOxygenSaturation = convertirASaturacion(oxygenVoltage);
  globalConductivity = calcularConductividad(localConductivityValue);
  // Crear un documento JSON
  StaticJsonDocument<capacity> doc;
  // Añadir valores de los sensores al documento JSON
  doc["sensorPH"] = globalPHValue;
  doc["sensorTurbidez"] = globalTurbValue;
  doc["sensorOxygenConcentration"] = globalOxygenConcentration;
  doc["sensorOxygenSaturation"] = globalOxygenSaturation;
  doc["sensorConductividad"] = globalConductivity;
  // Serializar el documento JSON a un string
  String jsonString;
  serializeJson(doc, jsonString);
  // Enviar el JSON al backend a través de HTTP POST
  enviarDatos(jsonString);
  // Esperar un breve periodo antes de la próxima lectura
  delay(1000);
}

// Función para convertir voltaje a concentración de oxígeno disuelto en mg/L
float convertirAConcentracion(float voltage) {
  return voltage * 5.0;
}

// Función para convertir voltaje a saturación de oxígeno en %
float convertirASaturacion(float voltage) {
  return voltage * 100.0;
}

// Función para calcular la conductividad en µS/cm
float calcularConductividad(int sensorValue) {
  float voltage = sensorValue * (5.0 / 1023.0);
  if (voltage < 2.5) return mapFloat(voltage, 0.0, 2.5, 0.0, 200.0);
  else if (voltage < 3.5) return mapFloat(voltage, 2.5, 3.5, 200.0, 2000.0);
  return mapFloat(voltage, 3.5, 5.0, 2000.0, 20000.0);

}

// Función para mapear valores de un rango a otro
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//EnviarDatosAlServidor
void enviarDatosAlServidor(String jsonString) {
  // Iniciar la sesión HTTP
  sim7600.println("AT+HTTPINIT");
  delay(1000);
  // Configurar el contexto GPRS para HTTP
  sim7600.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
  // Configurar la URL del servidor (aquí tu API)
  sim7600.println("AT+HTTPPARA=\"URL\",\"https://8ac9-2800-484-aa80-5f00-3d10-f07a-9bf2-68a4.ngrok-free.app/api/sensores""); // Ajusta la IP o dominio
  delay(1000);
  // Indicar que la petición será POST
  sim7600.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(1000);
  // Enviar los datos
  sim7600.print("AT+HTTPDATA=");
  // sim7600.print(jsonData.length()); // Longitud del JSON
  sim7600.print(jsonString.length());
  sim7600.println(",10000");  // Especificar el tamaño de los datos
  delay(1000);
  sim7600.println(jsonString); // Enviar el JSON
  delay(1000);
  // Iniciar el POST
  sim7600.println("AT+HTTPACTION=1");
  delay(5000);  // Espera a que termine la petición
// Leer la respuesta del servidor (opcional)
  sim7600.println("AT+HTTPREAD");
  delay(1000);
  // Cerrar la sesión HTTP
  sim7600.println("AT+HTTPTERM");
  delay(1000);

}





