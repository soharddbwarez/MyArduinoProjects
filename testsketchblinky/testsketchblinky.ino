int rxPin=0;

void setup()
{
 pinMode(rxPin, OUTPUT);
}

void loop()
{
 digitalWrite(rxPin, HIGH);
 delay(1000);
 digitalWrite(rxPin, LOW);
 delay(1000);
}
