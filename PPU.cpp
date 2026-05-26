#include "PPU.h"
#include <cstring>


const uint8_t STATUS_VBLANK_MASK = 0x80;
const uint8_t STATUS_SPRITE_ZERO_HIT_MASK = 0x40;
const uint8_t STATUS_SPRITE_OVERFLOW_MASK = 0x20;


PPU::PPU() {
	for (auto& nt : nameTable)
		for (auto& b : nt) b = 0x00;

	for (auto& pt : patternTable)
		for (auto& b : pt) b = 0x00;
	
	for (auto& b : paletteTable) b = 0x00;

	memset(oamMemory, 0x00, 256);
}

PPU::~PPU() {

}

uint8_t PPU::cpuRead(uint16_t addr, bool bReadOnly) {
	uint8_t data = 0x00;

	switch (addr) {
	case 0x0000: // Control
		break;
	case 0x0001: // Mask
		break;
	case 0x0002: // Status
		data = (status & 0xE0) | (dataBuffer & 0x1F);
		status &= ~STATUS_VBLANK_MASK; // Clear vblank flag on read
		bAddressLatch = false;
		break;
	case 0x0003: // OAM Address
		break;
	case 0x0004: // OAM Data
		data = oamMemory[oamAddress];
		break;
	case 0x0005: // Scroll
		break;
	case 0x0006: // PPU Address
		break;
	case 0x0007: // PPU Data
		data = dataBuffer;
		break;
	}

	return data;
}

void PPU::cpuWrite(uint16_t addr, uint8_t data) {
	switch (addr) {
	case 0x0000: // Control
		break;
	case 0x0001: // Mask
		break;
	case 0x0002: // Status
		// Only VBlank can be cleared by CPU
		break;
	case 0x0003: // OAM Address
		oamAddress = data;
		break;
	case 0x0004: // OAM Data
		oamMemory[oamAddress] = data;
		break;
	case 0x0005: // Scroll
		break;
	case 0x0006: // PPU Address
		break;
	case 0x0007: // PPU Data
		break;
	}
}

void PPU::clock() {
	cycle++;
	if (cycle > 340) {
		cycle = 0;
		scanline++;
		if (scanline > 260) {
			scanline = 0;
		}
	}
}