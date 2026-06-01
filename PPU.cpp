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
	ppuAddress = 0x0000;
	ppuAddressLatch = 0x00;
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
		data = (status & 0xE0) | (ppuDataBuffer & 0x1F);
		status &= ~STATUS_VBLANK_MASK; // Clear VBLANK on read
		ppuAddressLatch = 0x00;
		ppuAddressLatchToggle = false;
		break;
	case 0x0003: // OAM Address
		break;
	case 0x0004: // OAM Data
		data = oamMemory[oamAddress];
		break;
	case 0x0005: // Scroll
		break;
	case 0x0006: // Address
		break;
	case 0x0007: // Data
		data = ppuDataBuffer; // Return previous value
		ppuDataBuffer = ppuRead(ppuAddress);
		if ((control & 0x04) == 0) // VRAM increment mode
			ppuAddress += 1;
		else
			ppuAddress += 32;

		ppuAddress &= 0x3FFF;
		break;
	}

	return data;
}

void PPU::cpuWrite(uint16_t addr, uint8_t data) {
	switch (addr) {
	case 0x0000: // Control
		control = data;
		break;
	case 0x0001: // Mask
		mask = data;
		break;
	case 0x0002: // Status - write clears vblank flag
		break;
	case 0x0003: // OAM Address
		oamAddress = data;
		break;
	case 0x0004: // OAM Data
		oamMemory[oamAddress] = data;
		break;
	case 0x0005: // Scroll
		break;
	case 0x0006: // Address
		if (!ppuAddressLatchToggle) {
			ppuAddressLatch = (ppuAddressLatch & 0x00FF) | (data << 8);
		} else {
			ppuAddressLatch = (ppuAddressLatch & 0xFF00) | data;
			ppuAddress = ppuAddressLatch;
			ppuAddress &= 0x3FFF;
		}
		ppuAddressLatchToggle = !ppuAddressLatchToggle;
		break;
	case 0x0007: // Data
		ppuWrite(ppuAddress, data);
		if ((control & 0x04) == 0)
			ppuAddress += 1;
		else
			ppuAddress += 32;

		ppuAddress &= 0x3FFF;
		break;
	}
}

uint8_t PPU::ppuRead(uint16_t addr) {
	uint8_t data = 0x00;
	addr &= 0x3FFF;

	if (cart->ppuRead(addr, data)) {
		
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		data = patternTable[(addr & 0x1000) >> 12][addr & 0x0FFF];
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF) {
		addr &= 0x0FFF;
		if (addr >= 0x0000 && addr <= 0x03FF) {
			data = nameTable[0][addr];
		}
		else if (addr >= 0x0400 && addr <= 0x07FF) {
			data = nameTable[1][addr & 0x03FF];
		}
		else if (addr >= 0x0800 && addr <= 0x0BFF) {
			data = nameTable[2][addr & 0x03FF];
		}
		else if (addr >= 0x0C00 && addr <= 0x0FFF) {
			data = nameTable[3][addr & 0x03FF];
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF) {
		addr &= 0x001F;
		data = paletteTable[addr];
	}

	return data;
}

void PPU::ppuWrite(uint16_t addr, uint8_t data) {
	addr &= 0x3FFF;

	if (cart->ppuWrite(addr, data)) {

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		patternTable[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF) {
		addr &= 0x0FFF;
		if (addr >= 0x0000 && addr <= 0x03FF) {
			nameTable[0][addr] = data;
		}
		else if (addr >= 0x0400 && addr <= 0x07FF) {
			nameTable[1][addr & 0x03FF] = data;
		}
		else if (addr >= 0x0800 && addr <= 0x0BFF) {
			nameTable[2][addr & 0x03FF] = data;
		}
		else if (addr >= 0x0C00 && addr <= 0x0FFF) {
			nameTable[3][addr & 0x03FF] = data;
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF) {
		addr &= 0x001F;
		paletteTable[addr] = data;
	}
}

void PPU::clock() {
	scanline++;
	if (scanline >= 261) {
		scanline = -1;
		frame_complete = true;
		status |= STATUS_VBLANK_MASK;
	}

	if (scanline == 241) {
		status |= STATUS_VBLANK_MASK;
		bus->cpu.nmi();
	}

	cycle++;
	if (cycle >= 340) {
		cycle = 0;
	}
}
