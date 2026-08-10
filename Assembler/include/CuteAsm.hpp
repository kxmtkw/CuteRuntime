
#ifndef CUTEASM_ASSEMBLER_HPP
#define CUTEASM_ASSEMBLER_HPP

#include <string>


class CtAssembler {
  
	void assemble_string(std::string source, std::string outfile);

	public:

	void assemble(std::string filepath);
};

#endif // CUTEASM_ASSEMBLER_HPP


