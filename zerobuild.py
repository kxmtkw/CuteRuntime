from zero import *

# Options

debug = UserOptions.get("debug")
debug = True if debug == "true" else False

DEBUG_MACRO = Macro("CT_CONF_DEBUG")


#  Configuration

build = Build()

build.default_compiler = "gcc"
build.directory = "build"
build.arguments = Flags.Wall, Flags.Wextra, Flags.g

build.compilers.arguments["gcc"] = Flags.std_c17
build.compilers.arguments["g++"] = Flags.std_cpp20

build.export_compile_commands = True


# Cute Instructions

CuteInstr = StaticLibrary()
CuteInstr.source = Source(
	Path("Instr") / "image.c"
)
CuteInstr.headers.public = Path("Instr") / "include"


# Cute Engine

CuteRuntime = StaticLibrary()

src = Path("Runtime")
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

CuteRuntime.link(CuteInstr)

if debug:
	CuteRuntime.arguments = DEBUG_MACRO


# cute binary

cute = Executable()
cute.source = Source("main/runtime.c")
cute.link(CuteRuntime)


# Cute Assembler

CuteAsm = StaticLibrary()
CuteAsm.compiler = "g++"

src = Path("Assembler")

CuteAsm.headers.private = src
CuteAsm.headers.public = src / "include"
CuteAsm.source = Source(
	src / "tokenizer" / "tokenizer.cpp",
	src / "tokenizer" / "stream.cpp",
	src / "codegen" / "codegen.cpp",
	src / "assembler" / "assembler.cpp"
)

CuteAsm.link(CuteInstr)


# cuteasm binary

cuteasm = Executable()
cuteasm.compiler = CuteAsm.compiler
cuteasm.source = Source("main/assembler.cpp")
cuteasm.link(CuteAsm)
