/*Dev: Marcos A. Riveros
mriveros.py@gmail.com
Sin derechos de autor -  copia y usa moderadamente
Descripcion: este codigo esta adaptado para lector UHF especificamente
INVELION FACTORY INDUSTRIES.
El codigo funciona de manera correcta y expone la lectura de manera precisa en el monitor serial.*/

unsigned char ReadMulti[10] = {0XAA,0X00,0X27,0X00,0X03,0X22,0XFF,0XFF,0X4A,0XDD};
unsigned int timeSec = 0;
unsigned int timemin = 0;
unsigned int dataAdd = 0;
unsigned int incomedate = 0;
unsigned int parState = 0;
unsigned int codeState = 0;


void setup() {
  // Configuración del puerto serie y del LED
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hola mundo.");
  Serial.write(ReadMulti,10);

 }

void loop() {
  // Leer el comando en un intervalo de tiempo
  timeSec++;
  if (timeSec >= 50000) {
    timemin++;
    timeSec = 0;
    if (timemin >= 20) {
      timemin = 0;
      // Enviar comando de lectura múltiple
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.write(ReadMulti, 10);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  // Procesar datos recibidos por el puerto serie
  if (Serial.available() > 0) {
    incomedate = Serial.read(); // Obtener los datos recibidos por el puerto serie
    if ((incomedate == 0x02) & (parState == 0)) {
      parState = 1;
    } else if ((parState == 1) & (incomedate == 0x22) & (codeState == 0)) {
      codeState = 1;
      dataAdd = 3;
    } else if (codeState == 1) {
      dataAdd++;
      if (dataAdd == 6) {
        Serial.print("RSSI:");
        Serial.println(incomedate, HEX);
      } else if ((dataAdd == 7) | (dataAdd == 8)) {
        if (dataAdd == 7) {
          Serial.print("PC:");
          Serial.print(incomedate, HEX);
        } else {
          Serial.println(incomedate, HEX);
        }
      } else if ((dataAdd >= 9) & (dataAdd <= 20)) {
        if (dataAdd == 9) {
          Serial.print("EPC:");
        }
        Serial.print(incomedate, HEX);
      } else if (dataAdd >= 21) {
        Serial.println("");
        dataAdd = 0;
        parState = 0;
        codeState = 0;
      }
    } else {
      dataAdd = 0;
      parState = 0;
      codeState = 0;
    }
  }
  
}
