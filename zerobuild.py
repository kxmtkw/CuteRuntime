from zero import *

build = Build()

build.compiler = "gcc"
build.directory = "build"
build.arguments = Flags.Wall, Flags.Wextra, Flags.g

build.export_compile_commands = True

# Cute Instructions

CuteInstr = StaticLibrary()
CuteInstr.source = Source(
	Path("Instr") / "image.c"
)
CuteInstr.headers.public = Path("Instr") / "include"


# Cute Engine

CuteRuntime = StaticLibrary()
CuteRuntime.compiler = "gcc"

src = Path("Runtime")

CuteRuntime.link(CuteInstr)
CuteRuntime.headers.public = src / "include"
CuteRuntime.headers.private = src

CuteRuntime.source = Source(
	src / "core" / "core.c",
	src / "core" / "exec.c",
	src / "core" / "context.c",
	src / "objects" / "manager.c",
	src / "container" / "container.c",
	src / "utils" / "utils.c",

	src / "lib" / "buffer.c",
)


# cute binary

cute = Executable()
cute.link(CuteRuntime)
cute.source = Source("main/runtime.c")


# Cute Assembler

CuteAsm = StaticLibrary()
CuteAsm.compiler = "g++"

src = Path("Assembler")

CuteAsm.headers.private = src
CuteAsm.headers.public = src / "include"
CuteAsm.link(CuteInstr)

CuteAsm.source = Source(
	src / "tokenizer" / "tokenizer.cpp",
	src / "tokenizer" / "stream.cpp",
	src / "codegen" / "codegen.cpp",
	src / "assembler" / "assembler.cpp"
)


# cuteasm binary

cuteasm = Executable()
cuteasm.compiler = CuteAsm.compiler
cuteasm.link(CuteAsm)
cuteasm.source = Source("main/assembler.cpp")
