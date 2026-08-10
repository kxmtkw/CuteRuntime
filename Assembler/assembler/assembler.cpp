#include <fstream>
#include <iostream>
#include <sstream>
#include <format>
#include <filesystem>

#include "tokenizer/tokenizer.hpp"
#include "tokenizer/tokens.hpp"
#include "codegen/codegen.hpp"

#include "utils/utils.hpp"

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

	CtUtils::ErrorCollector error;
	CtTokenizer tokenizer(error);

	auto stream = tokenizer.tokenize(std::move(source));

	if (error.has_error()) {
		error.print();
		exit(1);
	}

	CtCodeGen generator(error);
	generator.generate(std::move(stream), outfile);

	if (error.has_error()) {
		error.print();
		exit(1);
	}
}