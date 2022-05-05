# Furnace Tracker For Macintosh OSX
### OSX 12.3 "Monterey"

![screenshot](papers/screenshot1.png)

This is a fork of a multi-system chiptune tracker for Windows and Debian Linux Distros ported over to the latest version of Mac OSX.


[main] (https://github.com/tildearrow/furnace)| [downloads](#downloads) | [discussion/help](#quick-references) | [developer info](#developer-info) | [unofficial packages](#unofficial-packages) | [FAQ](#frequently-asked-questions)

***

## disclaimer
[furnace tracker](https://github.com/tildearrow/furnace) is owned by [tildearrow](https://github.com/tildearrow). this fork was made for mac users who don't want to compile/build furnace every time it updates.


## downloads

check out the [Releases](https://github.com/tildearrow/furnace/releases) page. available for Windows, macOS and Linux (AppImage).

[see here](https://nightly.link/tildearrow/furnace/workflows/build/master) for unstable developer builds.

## features

- supports the following systems:
  - Sega Genesis
  - Sega Master System
  - Game Boy
  - PC Engine
  - NES
  - Commodore 64
  - Yamaha YM2151 (plus PCM)
  - Neo Geo
  - AY-3-8910 (ZX Spectrum, Atari ST, etc.)
  - Microchip AY8930
  - Philips SAA1099
  - Amiga
  - TIA (Atari 2600/7800)
- multiple sound chips in a single song!
- DefleMask compatibility - loads .dmf modules, .dmp instruments and .dmw wavetables
- clean-room design (guesswork and ABX tests only, no decompilation involved)
- bug/quirk implementation for increased playback accuracy
- VGM and audio file export
- accurate emulation cores whether possible (Nuked, MAME, SameBoy, Mednafen PCE, puNES, reSID, Stella, SAASound and ymfm)
- additional features on top:
  - FM macros!
  - negative octaves
  - arbitrary pitch samples
  - sample loop points
  - SSG envelopes in Neo Geo
  - full duty/cutoff range in C64
  - ability to change tempo mid-song with `Cxxx` effect (`xxx` between `000` and `3ff`)
- open-source under GPLv2 or later.

***
# quick references

 - **discussion**: see the [Discussions](https://github.com/tildearrow/furnace/discussions) section, or (preferably) the [official Discord server](https://discord.gg/EfrwT2wq7z).
 - **help**: check out the [documentation](papers/doc/README.md). it's mostly incomplete, but has details on effects.

# notes

> how do I use Neo Geo SSG envelopes?

the following effects are provided:

- `22xy`: set envelope mode.
  - `x` sets the envelope shape, which may be one of the following:
    - `0: \___` decay
    - `4: /___` attack once
    - `8: \\\\` saw
    - `9: \___` decay
    - `A: \/\/` inverse obelisco
    - `B: \¯¯¯` decay once
    - `C: ////` inverse saw
    - `D: /¯¯¯` attack
    - `E: /\/\` obelisco
    - `F: /___` attack once
  - if `y` is 1 then the envelope will affect this channel.
- `23xx`: set envelope period low byte.
- `24xx`: set envelope period high byte.
- `25xx`: slide envelope period up.
- `26xx`: slide envelope period down.

a lower envelope period will make the envelope run faster.

***
# frequently asked questions

> how do I use C64 absolute filter/duty?

on Instrument Editor in the C64 tab there are two options to toggle these.
also provided are two effects:

- `3xxx`: set fine duty.
- `4xxx`: set fine cutoff. `xxx` range is 000-7ff.
additionally, you can change the cutoff and/or duty as a macro inside an instrument by clicking the `absolute cutoff macro` and/or `absolute duty macro` checkbox at the bottom of the instrument. (for the filter, you also need to click the checkbox that says `volume macro is cutoff macro`.)

> Q: how do I use PCM on a PCM-capable system?

A: Two possibilities: the recommended way is via creating the "Amiga/Sample" type instrument and assigning sample to it, or via old, Deflemask-compatible method, using `17xx` effect

> Q: my song sounds very odd at a certain point

A: file a bug report. use the Issues page. it's probably another playback inaccuracy.

> Q: my song sounds correct, but it doesn't in DefleMask

A: file a bug report **here**. it still is a playback inaccuracy.

> Q: my C64 song sounds terrible after saving as .dmf!

A: that's a limitation of the DefleMask format. save in Furnace song format instead (.fur).

> Q: how do I solo channels?

A: right click on the channel name.

***
# footnotes

copyright (C) 2021-2022 tildearrow and contributors.

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


despite the fact this program works with the .dmf file format, it is NOT affiliated with Delek or DefleMask in any way, nor it is a replacement for the original program.
