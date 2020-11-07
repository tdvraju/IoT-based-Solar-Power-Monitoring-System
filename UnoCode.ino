#include<SoftwareSerial.h>
SoftwareSerial uno(3,2); // for communicating with node mcu
int curr = A0, voltage = A1; // for getting sensor data
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  uno.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int currTot =0,volTot = 0;
  for(int i=0;i<100;i++){
      currTot += analogRead(curr);
      volTot += analogRead(voltage);
      delay(1);
  } // taking aggregates to make sure that there are no wrong/abrupt values //accounted
  currTot /= 100;
  volTot /= 100;
  Serial.print("Current = ");
  Serial.print((2.5 - ((float)currTot * (5.0 / 1024.0)))*0.01); // Adjusting for 20A current sensor
  Serial.print(" || Voltage = ");
  Serial.print(-3* (5.0/1023.0*volTot));
  float power = -3* (5.0/1023.0*volTot) * (2.5 - ((float)currTot * (5.0 / 1024.0)))*0.01;
  Serial.print(" || Power = ");
  Serial.println(power);
  uno.print(String(power));
  uno.print("\n");
  delay(5000);
}
