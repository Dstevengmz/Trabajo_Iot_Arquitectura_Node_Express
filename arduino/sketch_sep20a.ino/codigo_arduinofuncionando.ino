#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// Pines de los sensores
const int sensorPinPH = A1;
const int sensorPinTurb = A2;
const int sensorPinOxygen = A3;
const int sensorPinConductividad = A4;

// Pines para la comunicación con el módulo SIM7600
const int sim7600Tx = 7;
const int sim7600Rx = 6;
SoftwareSerial sim7600(sim7600Rx, sim7600Tx);  // Instancia para comunicar con SIM7600

// Capacidad del JsonDocument para manejar los sensores
const size_t capacity = JSON_OBJECT_SIZE(8) + 150;

// Variables globales de los sensores
int globalPHValue = 0;
int globalTurbValue = 0;
float globalOxygenConcentration = 0.0;
float globalOxygenSaturation = 0.0;
float globalConductivity = 0.0;

// Setup
void setup() {
  // Configuración de los pines de los sensores
  pinMode(sensorPinPH, INPUT);
  pinMode(sensorPinTurb, INPUT);
  pinMode(sensorPinOxygen, INPUT);
  pinMode(sensorPinConductividad, INPUT);

  // Inicialización de la comunicación serial
  Serial.begin(115200);
  sim7600.begin(115200);
  
  // Configuración del SIM7600 para GPRS
  sim7600.println("AT");
  delay(1000);
  sim7600.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(1000);
  sim7600.println("AT+SAPBR=3,1,\"APN\",\"internet.movistar.com.co\"");
  delay(1000);
  sim7600.println("AT+SAPBR=3,1,\"USER\",\"MOVISTAR\"");
  delay(1000);
  sim7600.println("AT+SAPBR=3,1,\"PWD\",\"MOVISTAR\"");
  delay(1000);
  sim7600.println("AT+SAPBR=1,1");
  delay(3000);
  sim7600.println("AT+SAPBR=2,1");
  delay(1000);
}

   // Loop principal

   void loop() {

  // Leer los valores de los sensores

  int localPHValue = analogRead(sensorPinPH);
  int localTurbValue = analogRead(sensorPinTurb);
  float oxygenVoltage = analogRead(sensorPinOxygen) * (5.0 / 1023.0);
  int localConductivityValue = analogRead(sensorPinConductividad);

  // Asignar los valores a las variables globales

  globalPHValue = localPHValue;
  globalTurbValue = localTurbValue;
  globalOxygenConcentration = convertirAConcentracion(oxygenVoltage);
  globalOxygenSaturation = convertirASaturacion(oxygenVoltage);
  globalConductivity = calcularConductividad(localConductivityValue);

  // Crear un documento JSON con los valores de los sensores

  StaticJsonDocument<capacity> doc;
  doc["sensorPH"] = globalPHValue;
  doc["sensorTurbidez"] = globalTurbValue;
  doc["sensorOxygenConcentration"] = globalOxygenConcentration;
  doc["sensorOxygenSaturation"] = globalOxygenSaturation;
  doc["sensorConductividad"] = globalConductivity;

  // Serializar el documento JSON a un string
  String jsonString;
  serializeJson(doc, jsonString);

  // Enviar el JSON al servidor
  enviarDatosAlServidor(jsonString);

  // Mostrar el JSON en el monitor serial
  Serial.println(jsonString);

  // Pausar brevemente antes de la próxima lectura
  delay(1000);
}

// Función para enviar datos al servidor

void enviarDatosAlServidor(String jsonString) {
  sim7600.println("AT+HTTPINIT");
  delay(1000);
  sim7600.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
  sim7600.println("AT+HTTPPARA=\"URL\",\"https://d3nm9bfn-3300.use2.devtunnels.ms/api/1.0/sensores_vista\"");
  delay(1000);
  sim7600.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(1000);
  sim7600.print("AT+HTTPDATA=");
  sim7600.print(jsonString.length());
  sim7600.println(",10000");
  delay(1000);
  sim7600.println(jsonString);
  delay(1000);
  sim7600.println("AT+HTTPACTION=1");
  delay(5000);  // Espera la respuesta del servidor
  sim7600.println("AT+HTTPREAD");
  delay(1000);
  sim7600.println("AT+HTTPTERM");
  delay(1000);
}

// Función para convertir voltaje a concentración de oxígeno en mg/L
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