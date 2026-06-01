#pragma once

#include <cstdint>
#include "Cartridge.h"
#include "Bus.h"

class PPU
{
public:
	PPU();
	~PPU();

public:
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t ppuRead(uint16_t addr);
	void ppuWrite(uint16_t addr, uint8_t data);

	void clock();

public:
	uint8_t patternTable[2][4096];
	uint8_t nameTable[4][1024];
	uint8_t paletteTable[32];
	uint8_t oamMemory[256];

	uint8_t status = 0x00; // PPU Status Register
	uint8_t mask = 0x00;   // PPU Mask Register
	uint8_t control = 0x00;// PPU Control Register
	uint8_t oamAddress = 0x00; // OAM Address Register

	uint16_t ppuAddress = 0x0000; // PPU Address Register
	uint16_t ppuAddressLatch = 0x0000;

	uint8_t ppuDataBuffer = 0x00; // PPU Data buffer (for reads)

	bool frame_complete = false;

	int scanline = 0;
	int cycle = 0;

	Cartridge* cart = nullptr;
	Bus* bus = nullptr;

private:
	uint8_t ppuAddressLatchCounter = 0;
};
