//
//   FILE:  dht11_test1.pde
// PURPOSE: DHT11 library test sketch for Arduino
//

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}

//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
	return celsius + 273.15;
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm
double dewPoint(double celsius, double humidity)
{
	double A0= 373.15/(273.15 + celsius);
	double SUM = -7.90298 * (A0-1);
	SUM += 5.02808 * log10(A0);
	SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
	SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
	SUM += log10(1013.246);
	double VP = pow(10, SUM-3) * humidity;
	double T = log(VP/0.61078);   // temp var
	return (241.88 * T) / (17.558-T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity/100);
	double Td = (b * temp) / (a - temp);
	return Td;
}


#include <GPBuinoS_DHT11.h>

DHT11 dht11;

#define DHT11PIN 10

void setup()
{
  #ifdef FREERTOS_ARDUINO
  initMainLoopStackSize(100);
  #endif

  Serial.begin(115200);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
}

void loop()
{
  Serial.println("\n");

  int chk = dht11.read(DHT11PIN);

  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK:
		Serial.println("OK");
		break;
    case DHTLIB_ERROR_CHECKSUM:
		Serial.println("Checksum error");
		break;
    case DHTLIB_ERROR_TIMEOUT:
		Serial.println("Time out error");
		break;
    default:
		Serial.println("Unknown error");
		break;
  }

  Serial.print("Humidity (%): ");
  Serial.println((float)dht11.humidity, 2);

  Serial.print("Temperature (oC): ");
  Serial.println((float)dht11.temperature, 2);

  Serial.print("Temperature (oF): ");
  Serial.println(Fahrenheit(dht11.temperature), 2);

  Serial.print("Temperature (K): ");
  Serial.println(Kelvin(dht11.temperature), 2);

  Serial.print("Dew Point (oC): ");
  Serial.println(dewPoint(dht11.temperature, dht11.humidity));

  Serial.print("Dew PointFast (oC): ");
  Serial.println(dewPointFast(dht11.temperature, dht11.humidity));

  delay(1000);
}
//
// END OF FILE
//
