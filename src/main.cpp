#include <Arduino.h>

#define A   2
#define B   4
#define C   7
#define D   6
#define E   5
#define F   3
#define G   8
#define Dig1  9
#define Dig2  10
#define Dig3  11
#define Dig4  12

float t;

uint8_t display_buffer[30] =    {0, 0, 1, 0, 0, 1, 0,    1, 0, 1, 1, 1, 0, 1,    1, 0, 1, 0, 1, 1, 1,    0, 1, 1, 0, 0, 1, 1};
uint8_t anodes_buffer[30] =     {9, 9, 9, 9, 9, 9, 9,    10,10,10,10,10,10,10,   11,11,11,11,11,11,11,    12,12,12,12,12,12,12};
uint8_t current_io_buffer[30] = {2, 3, 4, 5, 6, 7, 8,    2, 3, 4, 5, 6, 7, 8,    2, 3, 4, 5, 6, 7, 8,    2, 3, 4, 5, 6, 7, 8};
uint8_t previous_io_buffer[30] = {8, 2, 3, 4, 5, 6, 7,    8, 2, 3, 4, 5, 6, 7,    8, 2, 3, 4, 5, 6, 7,    8, 2, 3, 4, 5, 6, 7};

uint8_t charminus[7] = {0, 0, 0, 0, 0, 0, 1};
uint8_t charnull[7] = {0, 0, 0, 0, 0, 0, 0};
uint8_t char0[7] = {1, 1, 1, 1, 1, 1, 0};
uint8_t char1[7] = {0, 0, 1, 0, 0, 1, 0};
uint8_t char2[7] = {1, 0, 1, 1, 1, 0, 1};
uint8_t char3[7] = {1, 0, 1, 0, 1, 1, 1};
uint8_t char4[7] = {0, 1, 1, 0, 0, 1, 1};
uint8_t char5[7] = {1, 1, 0, 0, 1, 1, 1};
uint8_t char6[7] = {1, 1, 0, 1, 1, 1, 1};
uint8_t char7[7] = {1, 0, 1, 0, 0, 1, 0};
uint8_t char8[7] = {1, 1, 1, 1, 1, 1, 1};
uint8_t char9[7] = {1, 1, 1, 0, 1, 1, 1};

uint8_t buffer_pointer = 0;
uint8_t current_digit = 12;
uint8_t io_crrection=0;

unsigned long myTime;

ISR(TIMER1_OVF_vect)
{
    TCNT1 = 0xE000;
    digitalWrite(13, HIGH);
    myTime = millis();
    digitalWrite(previous_io_buffer[buffer_pointer], HIGH);    
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(anodes_buffer[buffer_pointer], HIGH);
    if(display_buffer[buffer_pointer] == 1) digitalWrite(current_io_buffer[buffer_pointer], LOW); else digitalWrite(current_io_buffer[buffer_pointer], HIGH);
    if(++buffer_pointer>28) buffer_pointer=0;
    digitalWrite(13, LOW);
}

/// @brief Clears buffer per digit
/// @param Digit
/// @param Buffer
void clear_buffer(uint8_t digit, uint8_t *out)
{
  out+=(digit-1)*7;
  for (uint8_t i = 0; i<8; i++)
  {
    *out++ = 0x00;
  }
  
}

void prepare_digit(uint8_t digit, char num , uint8_t *out)
{
uint8_t *char_index;

  switch (num)
  {
    case '0': char_index = &char0[0]; break;
    case '1': char_index = &char1[0]; break;
    case '2': char_index = &char2[0]; break;
    case '3': char_index = &char3[0]; break;
    case '4': char_index = &char4[0]; break;
    case '5': char_index = &char5[0]; break;
    case '6': char_index = &char6[0]; break;
    case '7': char_index = &char7[0]; break;
    case '8': char_index = &char8[0]; break;
    case '9': char_index = &char9[0]; break;
    case ' ': char_index = &charnull[0]; break;
    case '-': char_index = &charminus[0]; break;
  }

  out+=(digit-1)*7;
  for (uint8_t i = 0; i<8; i++)
  {
    *out++ = *char_index++;
  }
  
}



void setup() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1<<CS10);
  TIMSK1 |= (1<<TOIE1);
  TCNT1 = 0xE000;
  Serial.begin(9600);
  for(int i=2; i<=13; ++i)
  {
    pinMode(i, OUTPUT);
  }
}

void loop() 
{
  double Vin = analogRead(A0)*0.00488;
  double temp_float = ((308*Vin)/5)-49;
  char temp_inetger[4];
  sprintf(temp_inetger, "%4d", round(temp_float));
  Serial.println(temp_inetger);
  Serial.println(temp_float);
  prepare_digit(1, temp_inetger[0], display_buffer);
  prepare_digit(2, temp_inetger[1], display_buffer);
  prepare_digit(3, temp_inetger[2], display_buffer);
  prepare_digit(4, temp_inetger[3], display_buffer);
  delay(300);
}