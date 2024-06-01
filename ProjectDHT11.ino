#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h> 

// definir a LCD address para 0x27 for a 16 letras and 2 linhas de display
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTTYPE DHT11
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run 'make menuconfig' to and enable it
#endif
BluetoothSerial SerialBT;
char valor,valor1;

const int pinoDHT11 = 0; //PINO ANALÓGICO UTILIZADO PELO DHT11
const int PinLedVerde =19 ;
const int PinLedVermelho = 23;
const int Sensortouch = 2;
const int TemperaturaMax = 90;
const int Buzzer= 17;
 int ligado= 1;
 int touch=0;
 
unsigned long Tempo = millis();
unsigned long Tempo2 = millis();
unsigned long Tempo3 = millis();
unsigned long Tempo4 = millis();

DHT dht(pinoDHT11, DHTTYPE);



void setup(){
  pinMode(Sensortouch,INPUT);
  pinMode(pinoDHT11,INPUT);
  pinMode(PinLedVerde,OUTPUT);
  pinMode(PinLedVermelho,OUTPUT);
  digitalWrite(PinLedVerde,LOW);
  digitalWrite(PinLedVermelho,LOW);
  pinMode(Buzzer,OUTPUT);
  digitalWrite(Buzzer,LOW);
  
  Serial.begin(115200); //INICIALIZA A SERIAL
  dht.begin();

  lcd.begin();
  lcd.setBacklight(LOW);
  SerialBT.begin("ESP32_Teste");
  Serial.println("Ligação disponível");

}
void loop(){
     valor =  SerialBT.read();
     valor1 =  Serial.read();
    if(Serial.available()){
    SerialBT.write(valor1);
    Serial.println(valor1);
    if (valor1=='0') {
      touch=0;
      digitalWrite(PinLedVerde,LOW);
      digitalWrite(PinLedVermelho,LOW);
    }
    if (valor1=='1') {
      touch=1;
    }
  }

  if (SerialBT.available()) {
    if (valor=='0') {
      touch=0;
      digitalWrite(PinLedVerde,LOW);
      digitalWrite(PinLedVermelho,LOW);
    }
    if (valor=='1') {
      touch=1;
    }
  }
  
  
  if(touchRead(T3)<50){
     if( touch==1){
      touch=0;
      digitalWrite(PinLedVerde,LOW);
      digitalWrite(PinLedVermelho,LOW);
      do{
          //delay(500)
      }while((millis() - Tempo)<600);
    }
    else{
      touch=1;
      do{
          //delay(500)
      }while((millis() - Tempo)<600);
    }
   }

    if(touch==1){
      Tempo2 = millis();
    if (digitalRead(Sensortouch) == HIGH){
      digitalWrite(Buzzer,HIGH);
      do{
          //delay(500)
      }while((millis() - Tempo2)<600);
    if( ligado==1){
      ligado=0;
      digitalWrite(Buzzer,LOW);
    }
    else{
      ligado=1;
      digitalWrite(Buzzer,LOW);
    }
  }
    
  int h = dht.readHumidity();
  int t= dht.readTemperature();


  if(t>TemperaturaMax){
    digitalWrite(PinLedVerde,LOW);
    digitalWrite(PinLedVermelho,HIGH);
  }
  else{
    digitalWrite(PinLedVermelho,LOW);
    digitalWrite(PinLedVerde,HIGH);
  }
 Serial.println(); 
  Serial.print("Humidade: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(h); //IMPRIME NA SERIAL O VALOR DE HUMIDADE
  Serial.print("%"); //ESCREVE O TEXTO EM SEGUIDA
  Serial.print(" / Temperatura: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(t); //IMPRIME NA SERIAL O VALOR DE TEMPERATURA
  Serial.println("*C"); //IMPRIME O TEXTO NA SERIAL 

  if (SerialBT.connected()) {
  String humidade="Humidade: ";
  String temperatura=" / Temperatura:  ";
  String grau=" *C\n";
  String hString=String(h);
  String tString=String(t);
  SerialBT.write((uint8_t*)humidade.c_str(), humidade.length());
  SerialBT.write((uint8_t*)hString.c_str(), hString.length());
  SerialBT.write((uint8_t*)temperatura.c_str(), temperatura.length());
  SerialBT.write((uint8_t*)tString.c_str(), tString.length());
  SerialBT.write((uint8_t*)grau.c_str(), grau.length());
  }   
 
  if(ligado==1){
  
  Serial.println("Ecrã ligado"); 
  
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.setCursor(0,0); // go to start of 1nd line
  lcd.print("Humidade:");
  lcd.print(h); 
  lcd.print("%");
  lcd.setCursor(0,1); // go to start of 2nd line
  lcd.print("Temperatura:");
  lcd.print(t); 
  lcd.print("*C");

   Tempo3 = millis();
  do{
   //delay(2000)
    }while((millis() - Tempo3)<2100);
  lcd.setBacklight(LOW);
  }
  if(ligado==0){
  Serial.println("Ecrã Desligado");
  lcd.clear();
  Tempo4 = millis();
  do{
  //delay(3000); 
  }while((millis() - Tempo4)<2100);
  }
  Tempo = millis();
  }

} 

