
#ifndef CUTEASM_CODEGEN_HPP
#define CUTEASM_CODEGEN_HPP

#include <cstdint>
#include <unordered_map>

extern "C" {
	#include "CuteInstr.h"
}

#include "tokenizer/tokens.hpp"


class CtCodeGen {

	CtImageBuilder mBuilder;

	CtTokenStream mStream;

	std::unordered_map<std::string, uint32_t> mJumpAddresses;
	std::unordered_map<uint32_t, std::string> mPatches;
	
	void
	resolve_procedure();

	void
	parse_instruction();

	public:

	void
	generate(CtTokenStream stream, std::string outpath);
};

#endif // CUTEASM_CODEGEN_HPP