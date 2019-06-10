#include <stdint.h>

// Intel HEX format line byte count
#define IHEX_LINE_BYTES 16

// Set MAX_ADDR to the largest address you need
// to read. For example, for the 27C512 chips,
// you'll want to use a MAX_ADDR of 65536.
// (That's 512 * 1024 / 8.)
// A 27C256 would be 256 kilobits, or 256 * 1024 / 8 =
// 32768.
#define MAX_ADDR 65536L

// On my board, I've connected pins 26-41
// to the A0-A15 lines, and pins 2-10 to the
// Q0-Q7 lines. You'll want to change these
// pin choices to match your setup.
#define A0 26
#define Q0 2

// When you're all wired up, hit the reset button
// to start dumping the hex codes.

void setup() {
  for (int i = A0; i < A0+16; i++) {
    digitalWrite(i,LOW);
    pinMode(i, OUTPUT);
  }
  for (int i = Q0; i < Q0+8; i++) {
    digitalWrite(i,HIGH);
    pinMode(i, INPUT);
  }
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("start of dump");
  Serial.println(" ");
  delay(1000);
}

void writeAddr(uint32_t addr) {
  uint32_t mask = 0x01;
  for (int i = A0; i < A0+16; i++) {
    if ((mask & addr) != 0) {
      digitalWrite(i,HIGH);
    } else { 
      digitalWrite(i,LOW);
    }
    mask = mask << 1;
  }
}


uint8_t readByte() {
  uint8_t data = 0;
  uint8_t mask = 0x1;
  for (int i = Q0; i < Q0+8; i++) {
    if (digitalRead(i) == HIGH) {
      data |= mask;
    }
    mask = mask << 1;
  }
  return data;
}

void print_padded_hex(const uint8_t i)
{
  const char ch[16] = { '0', '1', '2', '3', '4', '5', '6', '7', 
                        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  Serial.print(ch[i >> 4]);
  Serial.print(ch[i & 0xF]);
}

void loop() {
  uint32_t addr = 0;
  while (addr < MAX_ADDR) {
    const uint8_t addr_high = (addr & 0xFF00) >> 8;
    const uint8_t addr_low  = addr & 0xFF;
    uint32_t checksum = IHEX_LINE_BYTES + addr_high + addr_low;
    
    Serial.print(":");
    Serial.print(IHEX_LINE_BYTES, HEX);
    
    print_padded_hex(addr_high);
    print_padded_hex(addr_low);
    
    Serial.print("00");
    
    for (uint8_t i = 0; i < IHEX_LINE_BYTES; ++i) {
      writeAddr(addr);
      const uint8_t b = readByte();

      checksum += b;
      print_padded_hex(b);
      //Serial.print(" ");

      addr += 1;
    }

    checksum = ~(checksum & 0xFF) + 1;
    print_padded_hex(checksum);

    Serial.print('\n');
  }
  Serial.print(":00000001FF");
  Serial.println(" ");
  Serial.println("dump finished!");
  while (1) {}
}
