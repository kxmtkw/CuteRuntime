
#ifndef CUTEASM_CODEGEN_HPP
#define CUTEASM_CODEGEN_HPP

#include <cstdint>
#include <unordered_map>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}

#include "tokenizer/tokens.hpp"


class CtCodeGen {

	CtImageBuilder mBuilder;

	CtTokenStream mStream;

	std::unordered_map<std::string, uint32_t> mJumpAddresses;
	std::unordered_map<uint32_t, std::string> mPatches;

	// Parse a procedure
	void
	parse_procedure();

	// Parse a procedure statement
	void
	parse_procedure_statement();

	// Parse a label
	void
	parse_label();

	// Parse an instruction
	void
	parse_instruction();

	// Parse an expression inside an instruction
	void
	parse_expression();

	// Resolve all jump offsets.
	void
	resolve_jumps();

	public:

	void
	generate(CtTokenStream stream, std::string outpath);
};

#endif // CUTEASM_CODEGEN_HPP