#include "Bus.h"
#include <cstring>


Bus::Bus() {

	cpu.ConnectBus(this);

	for (auto& i : cpuRam) i = 0x00;

}

Bus::~Bus() {

}

void Bus::write(uint16_t addr, uint8_t data) {
	if (cart->cpuWrite(addr, data)) {
		// Cartridge handled it
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		cpuRam[addr & 0x07FF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF) {
		ppu.cpuWrite(addr & 0x0007, data);
	}
	else if (addr == 0x4014) {
		// DMA transfer
		uint16_t dma_address = data << 8;
		for (int i = 0; i < 256; i++) {
			ppu.oamMemory[i] = read(dma_address + i);
		}
	}
}

uint8_t Bus::read(uint16_t addr) {
	if (cart->cpuRead(addr, data)) {
		// Cartridge handled it
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		data = cpuRam[addr & 0x07FF];
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF) {
		data = ppu.cpuRead(addr & 0x0007, false);
	}
	return data;
}

void Bus::clock() {
	ppu.clock();
	
	if (systemClockCounter % 3 == 0) {
		cpu.clock();
	}

	systemClockCounter++;
}