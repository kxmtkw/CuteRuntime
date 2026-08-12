
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

#include "codegen/codegen.hpp"
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

	if (!mStream.expect_type(CtTokenType::Word, &label)) {
		throw_error("Expected indentifier as label name.");
	}

	mJumpAddresses[label] = mBuilder.image.header.instruction_count;
	
	if (!mStream.expect_token(";")) {
		throw_error("Statement should have ended here. Expected ;");
	}

	return;
}


void
CtCodeGen::parse_instruction() {

	std::string val;

	if (!mStream.expect_type(CtTokenType::Word, &val)) {
		throw_error(std::format("Expected Word."));
	}
	
	if (!ct_instr_map.contains(val)) {
		throw_error(std::format("Unknown Instruction: {}", val));
	}

	auto instr_info = ct_instr_map.at(val);
	CtInstr instr = instr_info.first;
	std::vector<CtInstrOperandType> operands = instr_info.second;

	ct_image_builder_add_instr(&mBuilder, instr);
	

	for (CtInstrOperandType t: operands) {

		if (mStream.expect_token(";")) {
			mStream.backtrack();
			throw_error("Instruction ended too soon.");
		}

		parse_operand(t);
	}

	if (!mStream.expect_token(";")) {
		throw_error("Statement should have ended here. Expected ;");
	}
}


void CtCodeGen::parse_operand(CtInstrOperandType optype) {
	std::string val;

	if (optype == CtInstrOperandType::Slot) {
		if (!mStream.expect_type(CtTokenType::Slot, &val)) {
			throw_error("Expected slot.");
		}	
		byte slot_index = static_cast<byte>(std::stoi(val));
		ct_image_builder_add_u8(&mBuilder, static_cast<uint8_t>(slot_index));

	} 

	else if (optype == CtInstrOperandType::I8) {
		if (!mStream.expect_type(CtTokenType::Int, &val)) {
			throw_error("Expected int8.");
		}
		int8_t number = static_cast<int8_t>(std::stoi(val));
		ct_image_builder_add_u8(&mBuilder, static_cast<uint8_t>(number));
	} 

	else if (optype == CtInstrOperandType::I16) {
		if (!mStream.expect_type(CtTokenType::Int, &val)) {
			throw_error("Expected int16.");
		}

		int16_t number;

		if (!CtUtils::str_to_i16(val, number)) {
			throw_error("Number too large to fit inside int16.");
		}

		ct_image_builder_add_u16(&mBuilder, static_cast<uint16_t>(number));
	} 

	else if (optype == CtInstrOperandType::I32) {
		if (mStream.expect_type(CtTokenType::Int, &val)) {
			int32_t number;

			if (!CtUtils::str_to_i32(val, number)) {
				throw_error("Number too large to fit inside int32.");
			}
			ct_image_builder_add_u32(&mBuilder, static_cast<uint32_t>(number));
			return;

		} else if (mStream.expect_type(CtTokenType::Word, &val)) {
			mPatches[mBuilder.image.header.instruction_count] = val;
			ct_image_builder_add_u32(&mBuilder, 0);
			return;
		}

		throw_error("Expected int32.");
	} 

	else if (optype == CtInstrOperandType::U32) {

		if (mStream.expect_type(CtTokenType::Int, &val)) {
			int32_t number;

			if (!CtUtils::str_to_u32(val, number)) {
				throw_error("Number too large to fit inside uint32.");
			}
			std::cout << "Read: " << number << "\n";
			ct_image_builder_add_u32(&mBuilder, static_cast<uint32_t>(number));
			return;

		} 

		throw_error("Expected uint32.");
	}

	else if (optype == CtInstrOperandType::I64) {
		if (!mStream.expect_type(CtTokenType::Int, &val)) {
			throw_error("Expected int64.");
		}
		int64_t number;

		if (!CtUtils::str_to_i64(val, number)) {
			throw_error("Number too large to fit inside int64.");
		}
		ct_image_builder_add_u64(&mBuilder, static_cast<uint64_t>(number));
	} 

	else if (optype == CtInstrOperandType::F32) {
		if (!mStream.expect_type(CtTokenType::Float, &val)) {
			throw_error("Expected float32.");
		}
		float number;

		if (!CtUtils::str_to_f32(val, number)) {
			throw_error("Number too large to fit inside float32.");
		}

		ct_image_builder_add_f32(&mBuilder, number);
	} 

	else if (optype == CtInstrOperandType::F64) {
		if (!mStream.expect_type(CtTokenType::Float, &val)) {
			throw_error("Expected float64");
		}
		double number;

		if (!CtUtils::str_to_f64(val, number)) {
			throw_error("Number too large to fit inside float64.");
		}
		ct_image_builder_add_f64(&mBuilder, number);
	} 
}


void
CtCodeGen::resolve_jumps() {
	for (auto item: mPatches) {
		uint32_t* ptr = (uint32_t*) ct_image_builder_get_address(&mBuilder, item.first);

		if (!mJumpAddresses.contains(item.second)) {
			throw_error(std::format("Unknown identifier: {}", item.second));
		}

		int jump_location = mJumpAddresses[item.second];
		int current_location = item.first + 4;
		int offset = jump_location - current_location;
		memcpy(ptr, &offset, sizeof(offset));
	}
}

void
CtCodeGen::throw_error(std::string details) {
	CtUtils::raise_error(
		mStream.get_source(),
		mStream.peek().start,
		details
	);
}

void
CtCodeGen::generate(CtTokenStream stream, std::string outpath) {

	mStream = stream;

	ct_image_builder_init(&mBuilder, 16, 16, 64);

	while (!mStream.eof()) {
		
		if (mStream.expect_token("proc")) {
			parse_procedure();
		}
	}

	ct_image_print(&mBuilder.image);
	ct_image_dump(&mBuilder.image, outpath.data());
}

