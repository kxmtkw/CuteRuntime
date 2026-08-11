
<h1 align="center">Cute (,,>﹏<,,)</h1>

`Cute` is a cute little language runtime that I am working on. It comes with its own instruction set, assembly, assembler and the core runtime.

### Features
- `Bytecode compiled` instruction set.
- `64-Bit` architecture.
- `Slot/Register based` VM.
- Objects and `Automatic Memory Management`.
- `Modules` system to add more functionality to the runtime.

### Design Goals
- As fast and optimized as possible without the need of a JIT (for now).
- Featureful runtime.
- Easily extendable.
- Extremely flexible.

### Example

Make a `demo.csm` file in your project. Write the following assembly in it.

```bash
# a loop based implementation of the power function. $0 is base and $1 is the exponent
proc 1: 2 {
	# $0 and $1 are filled with the passed arguments by the engine.

	loadi32 $2 1;
	loadi32 $3 1;

	@loop;

	mul $3 $3 $0;
	
	cmpi $2 $1;
	inc $2;

	jmplt loop;

	retval $3;
}

# procedure 0 is the entry point of the program. It takes 0 arguments.
proc 0: 0 {

	# set up the arguments
	loadi32 $0 5; # the base
	loadi32 $1 4; # the power

	# set up the proc id to be called
	loadi32 $2 1;
	call $2 $0 $4; # here $2 holds the id, $0 is the arg start slot and $4 is the return slot

	out $3 $4; # should print [ uint 125 ]

	loadi32 $3 0;
	halt $3; # exits with code 0
}
```

Then compile the file using:
```bash
cuteasm demo.csm
```

This will output a `demo.cute` file. Simply run that file using the runtime.
```bash
cute demo.cute
# output: [ uint 125 ]
```

### Installing

No official installation exists since the project is still in its infant stage.

1. Clone the repository.
2. Build the project. See [Building Cute](docs/07-building.md).
3. Run `cuteasm` or `cute` to make and run image files respectively.


### Docs
Here is a list of documentation to get you started:

1. [Project Architecture](docs/01-arch.md)
2. [Assembly Lang](docs/02-assembly.md)
3. [Instruction Set](docs/03-instructions.md)
4. [Image Format](docs/04-image.md)
5. [Assembler](docs/05-assembler.md)
6. [Runtime](docs/06-runtime.md)
7. [Building Cute](docs/07-building.md)

