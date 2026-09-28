#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#define BUTTON 2
#define RED 6
#define GREEN 7
#define SERVO 9
#define NEO_PIXEL 8
#define NUMPIXELS 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEO_PIXEL, NEO_GRB + NEO_KHZ800);
Servo servo;
bool pressed = false;
int state = 0;
int code[3] = {0, 0, 0};
int correct[3] = {10, 3, 5};
int selection;

bool matches(int* code, int n)
{
  for (int i = 0; i < n; i++)
    if (code[i] != correct[i])
      return false;
  return true;
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  for (int i = 3; i < 8; i++)
  	pinMode(i, OUTPUT);
  servo.attach(SERVO);
  pixels.begin();
}

void loop()
{
  selection = round((12*(1023 - analogRead(A0)))/1024);
  Serial.println(selection);
  
  // show selection
  for (int i = 0; i < 12; i++)
  {
    if (i == selection)
  	  pixels.setPixelColor(11 - selection + 2, pixels.Color(255, 0, 255));
  	else
      pixels.setPixelColor(11 - i + 2, pixels.Color(0, 0, 0));
  }
  pixels.show();
  
  // on button press
  if (pressed == false && digitalRead(BUTTON) == LOW)
  {
    pressed = true;
    
    if (0 <= state && state <= 2)
    {
      code[state] = selection;
      state++;
      digitalWrite(state + 2, HIGH);
      pixels.setPixelColor((state + 13) % 16, pixels.Color(0, 0, 255));
      pixels.show();
      
      if (state == 3)
      {
        delay(2000);
      	if (matches(code, 3))
      	{
          digitalWrite(GREEN, HIGH);
          pixels.setPixelColor(1, pixels.Color(0, 255, 0));
          pixels.show();
          servo.write(0);
      	}
      	else
      	{
          digitalWrite(RED, HIGH);
          pixels.setPixelColor(1, pixels.Color(255, 0, 0));
          pixels.show();
          delay(1000);
          for (int i = 3; i < 7; i++)
        	digitalWrite(i, LOW);
          pixels.clear();
          state = 0;
        }
      }
    }
  }
  else if (pressed == true && digitalRead(BUTTON) == HIGH)
    pressed = false;
}
