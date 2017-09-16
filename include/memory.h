// 0x0000-0x00FF Restart and interrupt vector table
// 0x0100-0x014F Catridge header
// 0x0150-0x3FFF ROM bank 0 (fixed)
// 0x4000-0x7FFF ROM bank n (switchable)
#define CART 0x0000

// 0x8000-0x97FF Character RAM
// 0x9800-0x9BFF BG map data 1
// 0x9C00-0x9FFF BG map data 2
#define VRAM 0x8000

// 0xA000-0xBFFF External (cartridge) RAM
#define ERAM 0xA000

// 0xC000-0xDFFF Internal work RAM
#define IRAM 0xC000

// 0xE000-0xFDFF DO NOT USE

// 0xFE00-0xFE9F Object attribute memory
#define OAM 0xFE00

// 0xFEA0-0xFEFF DO NOT USE

// 0xFF00-0xFF7F Hardware I/O registers
#define IO 0xFF00

// 0xFF80-0xFFFE High RAM area
#define HRAM 0xFF80

extern unsigned char memory[0xFFFF];

void write_byte(unsigned short address, unsigned char n);
unsigned char read_byte(unsigned short address);

void push_stack(unsigned char n);
unsigned char pop_stack(void);
