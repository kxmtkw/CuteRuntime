#include <fstream>
#include <iostream>
#include <sstream>
#include <format>
#include <filesystem>

#include "tokenizer/tokenizer.hpp"
#include "tokenizer/tokens.hpp"
#include "codegen/codegen.hpp"


#include "CuteAsm.hpp"


void CtAssembler::assemble(std::string filepath) {

	std::ifstream file(filepath);

	if (!file.is_open()) {
		std::cerr << "Could not open file: " << filepath << std::endl;
		exit(1);
	}

	std::stringstream ss;
	ss << file.rdbuf();
	std::string content = ss.str();

	filepath = std::filesystem::path(filepath).replace_extension(".cute").string();
	assemble_string(std::move(content), filepath);
};


void CtAssembler::assemble_string(std::string source, std::string outfile) {

	CtTokenizer tokenizer;

	auto stream = tokenizer.tokenize(std::move(source));

	while (stream.peek().type != CtTokenType::EndOfFile) {
		CtToken token = stream.next();
		std::cout << _ct_token_to_string(token.type) << " " << stream.get_value(token) << std::endl;
	}

	stream.reset();

	CtCodeGen generator;
	generator.generate(std::move(stream), outfile);
}