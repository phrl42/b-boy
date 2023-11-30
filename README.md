# GameBoy Color Emulator

Currently Work In Progress!

# Resources:
<a href="https://en.wikipedia.org/wiki/Game_Boy_Color">Wikipedia</a>
<a href="https://gbdev.io/pandocs/Specifications.html">Reference</a>

# To-Do

## Passing tests
- [X] 01-special
- [ ] 02-interrupts
- [ ] 03-op sp,hl
- [ ] 04-ld r,imm
- [X] 05-op rp
- [X] 06-ld r,r
- [ ] 07-jr,jp,call,ret,rst
- [ ] 08-misc instrs
- [ ] 09-op r,r
- [ ] 10-bit ops
- [ ] 11-op a,(hl)



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
