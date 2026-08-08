
#ifndef CUTEASM_CODEGEN_HPP
#define CUTEASM_CODEGEN_HPP

#include <map>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}

#include "spec/program.hpp"
#include "tokenizer/tokens.hpp"


class CtCodeGen {

	CtImage mImage;

	CtTokenStream mStream;
	std::unique_ptr<CtProgram> mProgram = nullptr;

	std::unordered_map<std::string, unsigned int> mJumpAddresses;
	std::unordered_map<unsigned int, std::string> mPatches;
	

	std::unique_ptr<CtProcedure>
	resolve_procedure();

	void
	parse_instruction();

	
	void
	write_image(std::string outpath);


	public:

	void
	generate(CtTokenStream stream, std::string outpath);
};

#endif // CUTEASM_CODEGEN_HPP