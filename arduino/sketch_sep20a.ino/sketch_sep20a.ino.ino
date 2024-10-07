#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// Definir los pines analógicos donde están conectados los sensores
const int sensorPinPH = A1;   // Pin A1 para el sensor de pH
const int sensorPinTurb = A2; // Pin A2 para el sensor de turbidez
const int sensorPinOxygen = A3; // Pin A3 para el sensor de oxígeno disuelto
const int sensorPinConductividad = A4; // Pin A4 para el sensor de conductividad

// Definir los pines para la comunicación serial con el módulo SIM7600G-H
const int sim7600Tx = 7;  // Arduino TX -> SIM7600G-H RX
const int sim7600Rx = 6;  // Arduino RX -> SIM7600G-H TX

// Crear una instancia de SoftwareSerial para comunicar con el módulo SIM7600G-H
SoftwareSerial sim7600(sim7600Rx, sim7600Tx);

// Capacidad del JsonDocument ajustada para manejar los sensores
const size_t capacity = JSON_OBJECT_SIZE(8) + 150;

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

  Serial.begin(115200);             // Inicia la comunicación serial
  sim7600.begin(115200);          // Inicia la comunicación serial con el módulo SIM7600G-H a 115200 baudios

  // Configurar el SIM7600
  Serial.println("Inicializando SIM7600...");
  delay(1000);

  // Limpia el buffer antes de enviar comandos AT
  while(sim7600.available()) sim7600.read(); 

  // Cambiar la tasa de baudios a 9600
  sim7600.println("AT+IPR=9600");
  delay(1000);

  // Reinicia la comunicación a 9600 baudios
  sim7600.begin(9600);
  Serial.println("Cambio de baudios a 9600 realizado.");

  // Configurar el módulo SIM7600 con comandos AT
  sendCommand("AT");                 // Verificar si el SIM7600 está listo
  sendCommand("AT+CPIN?");           // Verificar si la SIM está insertada y desbloqueada
  sendCommand("AT+CREG?");           // Verificar si está registrado en la red móvil
  sendCommand("AT+CGATT?");          // Verificar si GPRS está adjunto
  sendCommand("AT+CSQ");             // Verificar la intensidad de la señal
  sendCommand("AT+CGDCONT=1,\"IP\",\"internet.movistar.com.co\"");  // Configurar el APN
  sendCommand("AT+CNMP=38");         // Configurar el modo de red (LTE)
  sendCommand("AT+NETOPEN");         // Activar la red de datos
  delay(2000);
  sendCommand("AT+IPADDR");          // Verificar la IP asignada
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
  doc["sensorOxygenSaturacion"] = globalOxygenSaturation;
  doc["sensorConductividad"] = globalConductivity;

  // Serializar el documento JSON a un string
  String jsonString;
  serializeJson(doc, jsonString);

  // Enviar los datos al servidor mediante SIM7600 usando comandos AT
  sendCommand("AT+HTTPINIT");
  delay(5000);
  
//sendCommand("AT+HTTPPARA=\"URL\",\"https://d3nm9bfn-3300.use2.devtunnels.ms/api/1.0/sensores_vista\"");
  sendCommand("AT+HTTPPARA=\"URL\",\"https://d3nm9bfn-3300.use2.devtunnels.ms/api/1.0/sensores_vista\"");

  delay(2000);
  sendCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(1000);
  sendCommand("AT+HTTPDATA=" + String(jsonString.length()) + ",10000");
  delay(2000);  // Espera para la preparación de la solicitud
  
  sim7600.println(jsonString);  // Enviar los datos
  Serial.println("Enviando datos: " + jsonString);
  delay(1000);
  
  sendCommand("AT+HTTPACTION=1");  // Realizar solicitud POST
  delay(2000);  // Espera respuesta del servidor

  sendCommand("AT+HTTPREAD=0,100");  // Leer respuesta
  
  sendCommand("AT+HTTPTERM");  // Terminar sesión HTTP
  delay(2000);
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

// Función para enviar comandos AT al módulo SIM7600
void sendCommand(String command) {
  sim7600.println(command);
  delay(1000);
  while (sim7600.available()) {
    Serial.write(sim7600.read());  // Muestra la respuesta en el monitor serial
  }
}

