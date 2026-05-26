---
title: Careful handling of cycle and scanline increments
date: 2026-05-25
tags: [ppu, emulation, timing]
type: performance
project: DapperBanana/NEMU
---

Incrementing the PPU's cycle and scanline counters in the clock function is a good start. However, pay very close attention to how these counters wrap around. NES PPU timing is intricate, and small errors can lead to visual glitches or incorrect behavior. For example, scanlines don't increment on every cycle, and certain cycles are skipped during vblank. Consider using a lookup table or a well-defined state machine to manage these increments correctly for long-term maintainability and performance.
