#include <DHT.h>
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int moisture_pin = A0;
int W_monitoring_motor_pin = A2;
int valve_pin = A1;
int W_monioring_high_pin = 2;
int W_monioring_low_pin = 3;


float moisture();
void ValveOn();
void ValveOff();
void W_ValveOn();
void W_ValveOff();
float DHT_seonsor();

void setup() 
{
  pinMode(moisture_pin,INPUT);
  pinMode(W_monitoring_motor_pin,OUTPUT);
  pinMode(valve_pin,OUTPUT);
  pinMode(W_monioring_low_pin, INPUT_PULLUP);
  dht.begin();
  attachInterrupt(digitalPinToInterrupt(W_monioring_low_pin), W_ValveOn, FALLING);
  pinMode(W_monioring_high_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(W_monioring_high_pin), W_ValveOff, RISING);
  Serial.begin(9600);
}

void loop() 
{
  float moist_value = moisture();
  float DHT_value = DHT_seonsor();
  if(moist_value<= 30 )
  {
    ValveOn();
  }
  else
  {
    ValveOff();
  }
}

void ValveOn()
{
  analogWrite(valve_pin,50);
  Serial.println("Drip motor turned on");
}

void ValveOff() 
{
  analogWrite(valve_pin,0);
  Serial.println("Drip motor turned off");
}

float moisture()
{
   float output_value=0;
   for(int i=1;i<=100;i++)
   {
    output_value+=analogRead(moisture_pin);
   }
   output_value = map(output_value/100,1025,150,0,100);
   Serial.println("Moisture sensor(scaled=0 to 100:");
   Serial.println(output_value);
   return(output_value); 
}

void W_ValveOn()
{
  analogWrite(W_monitoring_motor_pin,150);
  Serial.println("water monitor motor turned on");
}


void W_ValveOff()
{
  analogWrite(W_monitoring_motor_pin,0);  
  Serial.println("water monitor motor turned off");
}

float DHT_seonsor()
{
  delay (1000);
  float h=dht.readHumidity();
  float t=dht.readTemperature();
  float f=dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 0;
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  
      
      //display dht data:
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  return hic;
}
