
#ifndef CUTEASM_CODEGEN_HPP
#define CUTEASM_CODEGEN_HPP

#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}

#include "spec/instructions.hpp"
#include "tokenizer/tokens.hpp"
#include "utils/utils.hpp"


class CtCodeGen {

	CtImageBuilder mBuilder;

	CtTokenStream mStream;

	std::unordered_set<std::string> mDefinedSymbols;
	
	std::unordered_map<std::string, uint32_t> mJumpAddresses;
	std::unordered_map<std::string, uint32_t> mProcedureMap;

	std::unordered_map<uint32_t, std::string> mPatches;

	bool mMainFound = false;
	
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
	parse_operand(CtInstrOperandType optype);

	// Resolve all jump offsets.
	void
	resolve_jumps();

	// Resolve all remaining patches
	void
	resolve_procedures();

	void
	throw_error(std::string details);

	public:

	void
	generate(CtTokenStream stream, std::string outpath);
};

#endif // CUTEASM_CODEGEN_HPP