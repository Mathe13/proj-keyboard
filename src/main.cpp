#include <Arduino.h>
#include <bit_handle.h>

#define  CLK_PIN   2 
#define  DATA_PIN  8

#define BUFFER_SIZE 4
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;

static volatile uint8_t received_char;
static volatile bool print_dado;


void ps2interrupt(void);

void setup()
{
  Serial.begin(9600);

  Serial.println("Start...");
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DATA_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLK_PIN), ps2interrupt, FALLING);
}

void loop() {
  if(print_dado){
    switch (received_char)
    {
      case 0x15:
        Serial.write("Q");
        break;
      case 0x1D:
        Serial.write("W");
        break;
      case 0x24:
        Serial.write("E");
        break;
      case 0x2D:
        Serial.write("R");
        break;
      case 0x2C:
        Serial.write("T");
        break;
      case 0x35:
        Serial.write("Y");
        break;
      case 0x3C:
        Serial.write("U");
        break;
      case 0x43:
        Serial.write("I");
        break;
      case 0x44:
        Serial.write("O");
        break;
      case 0x4D:
        Serial.write("P");
        break;
      case 0x1C:
        Serial.write("A");
        break;
      case 0x1B:
        Serial.write("S");
        break;
      case 0x23:
        Serial.write("D");
        break;
      case 0x2B:
        Serial.write("F");
        break;
      case 0x34:
        Serial.write("G");
        break;
      case 0x33:
        Serial.write("H");
        break;
      case 0x3B:
        Serial.write("J");
        break;
      case 0x42:
        Serial.write("K");
        break;
      case 0x4B:
        Serial.write("L");
        break;
      case 0x1A:
        Serial.write("Z");
        break;
      case 0x22:
        Serial.write("X");
        break;
      case 0x21:
        Serial.write("C");
        break;
      case 0x2A:
        Serial.write("V");
        break;
      case 0x32:
        Serial.write("B");
        break;
      case 0x31:
        Serial.write("N");
        break;
      case 0x3A:
        Serial.write("M");
        break;
      case 0x4C:
        Serial.write("Ç");
        break;
      case 0x5A:
        Serial.write("\n");
        break;
      case 0x29:
        Serial.write(" ");
        break;
      case 0x45:
        Serial.write("0");
        break;
      case 0x16:
        Serial.write("1");
        break;
      case 0x1E:
        Serial.write("2");
        break;
      case 0x26:
        Serial.write("3");
        break;
      case 0x25:
        Serial.write("4");
        break;
      case 0x2E:
        Serial.write("5");
        break;
      case 0x36:
        Serial.write("6");
        break;
      case 0x3D:
        Serial.write("7");
        break;
      case 0x3E:
        Serial.write("8");
        break;
      case 0x46:
        Serial.write("9");
        break;
      // Outros casos...
      default:
        // Caso não haja um mapeamento para o código recebido
        Serial.print("Unknown: ");
        Serial.print(received_char,HEX);
        Serial.print("\n");
        break;
    }
      print_dado = false;
  }
  delay(100);

}

void ps2interrupt(void)
{
  static uint8_t bitcount = 0;
  static uint8_t incoming = 0;
  static uint32_t prev_ms = 0;
  uint32_t now_ms;
  uint8_t n, val;

  val = digitalRead(DATA_PIN);
  now_ms = millis();
  if (now_ms - prev_ms > 250)
  {
    bitcount = 0;
    incoming = 0;
  }
  prev_ms = now_ms;
  n = bitcount - 1;
  if (n <= 7)
  {
    incoming |= (val << n);
  }
  bitcount++;
  if (bitcount == 11)
  {
    uint8_t i = head + 1;
    if (i >= BUFFER_SIZE){
      i = head = 1;
    }
    if (i != tail)
    {
      buffer[i] = incoming;
      head = i;
    }
    if (i == 3 && buffer[1] == buffer[3] && buffer[2] == 0xf0)
    {
      // Serial.println(buffer[1],HEX);
      // Serial.println(buffer[2], HEX);
      // Serial.println(buffer[3], HEX);
      received_char = buffer[3];
      print_dado = true;

    }
    bitcount = 0;
    incoming = 0;
  }
}