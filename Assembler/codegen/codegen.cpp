
#include "codegen/codegen.hpp"
#include "utils/utils.hpp"
#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <memory>
#include <string.h>
#include <string>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}

#include "tokenizer/tokens.hpp"
#include "spec/instructions.hpp"

using std::byte;


void
CtCodeGen::parse_procedure() {

	mJumpAddresses.clear();
	mPatches.clear();
	
	std::string val;

	if (!mStream.expect_type(CtTokenType::Int, &val)) {
		throw_error("Expected integar for procedure id.");
		return;
	}

	uint32_t id = std::stoul(val);

	if (!(mStream.expect_token(":") &&mStream.expect_type(CtTokenType::Int, &val))) {
		throw_error("Invalid argument format.");
		return;
	}

	uint32_t arg_count = std::stoul(val);

	ct_image_builder_new_proc(&mBuilder, id, arg_count);

	if (!mStream.expect_token("{")) {
		throw_error("Expected { after procedure declaration.");
		return;
	}

	while (true) {

		if (mStream.eof()) {
			throw_error("Procedure did not end with }. File ended too soon.");
			return;
			exit(1);
		}

		if (mStream.expect_token("}")) {
			break;
		}
	
		parse_procedure_statement();
	}

	resolve_jumps();
}


void
CtCodeGen::parse_procedure_statement() {

	if (mStream.expect_token("@")) {
		parse_label();		
	} else {
		parse_instruction();
	}
}


void
CtCodeGen::parse_label() {
	std::string label;

	mStream.expect_type(CtTokenType::Word, &label);

	mJumpAddresses[label] = mBuilder.image.header.instruction_count;
	
	mStream.expect_token(";");

	return;
}


void
CtCodeGen::parse_instruction() {

	std::string val;

	if (!mStream.expect_type(CtTokenType::Word, &val)) {
		throw_error(std::format("Expected Word."));
	}
	
	if (!CtInstrMap.contains(val)) {
		throw_error(std::format("Unknown Instruction: {}", val));
		return;
	}

	CtInstrSize instr = CtInstrMap.at(val);

	ct_image_builder_add_instr(&mBuilder, instr);
	
	while (!mStream.expect_token(";")) {
		parse_expression();
	}
}


void
CtCodeGen::parse_expression() {

	std::string val;

	if (mStream.expect_type(CtTokenType::Slot, &val)) {
		
		byte slot_index = (byte) std::stoi(val);
		ct_image_builder_add_u8(&mBuilder, (uint8_t) slot_index);

	} else if (mStream.expect_type(CtTokenType::Int, &val)) {

		int number = std::stoi(val);
		ct_image_builder_add_u32(&mBuilder, number);

	} else if (mStream.expect_type(CtTokenType::Float, &val)) {
		
		float number = std::stof(val);
		ct_image_builder_add_f32(&mBuilder, number);

	} else if (mStream.expect_type(CtTokenType::Word, &val)) {
		mPatches[mBuilder.image.header.instruction_count] = val;
		ct_image_builder_add_u32(&mBuilder, 0);
	}
}


void
CtCodeGen::resolve_jumps() {
	for (auto item: mPatches) {
		uint32_t* ptr = (uint32_t*) ct_image_builder_get_address(&mBuilder, item.first);

		if (!mJumpAddresses.contains(item.second)) {
			throw_error("Unknown Word.");
			return;
		}

		int jump_location = mJumpAddresses[item.second];
		int current_location = item.first + 4;
		int offset = jump_location - current_location;
		memcpy(ptr, &offset, sizeof(offset));
	}
}

void
CtCodeGen::throw_error(std::string details) {
	unsigned int index = mStream.peek().start;
	mError.add_error(
		CtUtils::count_lines_up_to_index(mStream.get_source(), index), 
		CtUtils::get_line_at_index(mStream.get_source(), index), 
		details
	);
	mError.print();
	exit(1);
}

void
CtCodeGen::generate(CtTokenStream stream, std::string outpath) {

	mStream = stream;

	ct_image_builder_init(&mBuilder, 16, 64);

	while (!mStream.eof()) {
		
		if (mStream.expect_token("proc")) {
			parse_procedure();
		}
	}

	ct_image_dump(&mBuilder.image, outpath.data());
}

