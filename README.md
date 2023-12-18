# GameBoy Color Emulator

Currently Work In Progress!

<img src="screenshot.png"></a>
# Resources:
<a href="https://en.wikipedia.org/wiki/Game_Boy_Color">Wikipedia</a>
<a href="https://gbdev.io/pandocs/Specifications.html">Reference</a>

# To-Do
- [ ] fix sprite bugs
- [ ] port to android
- [ ] fix timing
- [ ] more mbc support for pokemon
- [ ] sound
- [ ] saving and save states

## Passing tests
- [X] 01-special
- [X] 02-interrupts
- [X] 03-op sp,hl
- [X] 04-ld r,imm
- [X] 05-op rp
- [X] 06-ld r,r
- [X] 07-jr,jp,call,ret,rst
- [X] 08-misc instrs
- [X] 09-op r,r
- [X] 10-bit ops
- [X] 11-op a,(hl)

# Instructions

Generate build files:

## Windows:

```run.bat```

Then open the Visual Studio Solution and compile the "Sandbox" Project.

## Linux:

You should install premake5 on linux.

```premake5 gmake2```

```make```

Run game binary:

```./run.sh```

# Info
This is Linux and Windows compatible. All dependencies are within this repository.
