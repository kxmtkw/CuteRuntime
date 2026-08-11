
# Architecture

### Overview

`Cute` is a 64-bit register-based virtual machine toolchain consisting of two primary binaries:

1. `cuteasm`: Compiles `.csm` assembly text files into `.cute` executable images.
2. `cute`: The runtime that loads and executes `.cute` images.


### Toolchain Pipeline
```
Source Code (.csm)
		|
	    v
┌───────────────┐
│   Cute Asm    │  (Compiling assembly file/s to a single image file)
└───────────────┘
		|
		v
Executable Image (.cute)
		|
		v
┌────────────────┐
│  Cute Runtime  │  (Executing image file)
└────────────────┘
```

### Project Structure
``` bash
.
├── Assembler # Main Assembler Source Code
│   ├── assembler # asssembler Core
│   ├── codegen  # writing bytecode and resolving symbols
│   ├── include # public header
│   ├── spec # instruction maps and program repr
│   └── tokenizer # lexing asm files
│
├── Runtime # Main Engine Source Code
│   ├── common # common headers needed throughout the engine
│   ├── container # sub class of object, exposed to the asm
│   ├── core # heart of the runtime
│   ├── include # public header
│   ├── modules # module system for extended functionality
│   ├── objects # objects subsystem, gc, object defintions and manager
│   └── utils # small utils needed in the engine
│
├── Instr # Main Instruction Set Definition and Image Writing/Reading
│   └── include # public header
│
├── main # Main binary entry points: cute and cuteasm
│
├── dev # Place for testing
│
└── docs # Documentation
    └── records # list of recorded design choices
```

### Building

It is required that you build the engine using either `gcc` or `clang`.
The build system recommended is `ZeroBuild`.
See more on how to build [here](07-building.md).