---
title: Address latch reset timing matters for PPU
date: 2026-05-31
tags: [ppu, addressing, timing]
type: gotcha
project: DapperBanana/NEMU
---

The PPU's address register is implemented as a pair of latches. Writing to $2006 loads a high byte, then a low byte. The second write triggers the actual address update. Resetting this latch during VBLANK is crucial. If the game writes only *one* byte to $2006 right before VBLANK, the high byte is latched, but the low byte is never written because the reset occurs first. This can cause the PPU to read from an unexpected location after VBLANK. Many NES games depend on this behavior to perform split screen effects or dynamic updates to nametables during rendering.
