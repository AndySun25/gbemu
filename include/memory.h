// 0x0000-0x00FF Restart and interrupt vector table
// 0x0100-0x014F Catridge header
// 0x0150-0x3FFF ROM bank 0 (fixed)
unsigned char fcart[0x4000];

// 0x4000-0x7FFF ROM bank n (switchable)
// TODO implement proper rom banking
unsigned char rom_bank[0x4000];

// 0x8000-0x97FF Character RAM
// 0x9800-0x9BFF BG map data 1
// 0x9C00-0x9FFF BG map data 2
unsigned char vram[0x2000];

// 0xA000-0xBFFF External (cartridge) RAM
// TODO implement proper ram banking
unsigned char ram_bank[0x2000];

// 0xC000-0xDFFF Internal work RAM
unsigned char iram[0x2000];

// 0xE000-0xFDFF ECHO RAM
unsigned char echo[0x1E00];

// 0xFE00-0xFE9F Object attribute memory
unsigned char oam[100];

// 0xFEA0-0xFEFF DO NOT USE

// 0xFF00-0xFF7F Hardware I/O registers
unsigned char io[0x80];

// 0xFF80-0xFFFE High RAM area
unsigned char hram[0x80];

// unsigned char mbc_type;
// unsigned char current_rom_bank;
// unsigned char current_ram_bank;

void writeByte(unsigned short address, unsigned char n);
void writeShort(unsigned short address, unsigned short nn);
unsigned char readByte(unsigned short address);
unsigned short readShort(unsigned short address);

void pushStack(unsigned short n);
unsigned short popStack(void);
