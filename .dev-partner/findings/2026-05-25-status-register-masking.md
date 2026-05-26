---
title: Status register reads should mask irrelevant bits
date: 2026-05-25
tags: [ppu, emulation, hardware]
type: gotcha
project: DapperBanana/NEMU
---

When reading the PPU status register, it's crucial to mask out bits that aren't actually exposed to the CPU. The upper bits (VBlank, Sprite 0 Hit, Sprite Overflow) are valid, but the lower bits are typically reflections of the last write to the PPU's address register. Failing to mask these lower bits can lead to unexpected behavior and make debugging harder, as the CPU might misinterpret the status based on residual data.
