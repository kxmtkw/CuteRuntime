
#include "codegen/codegen.hpp"
#include <cstddef>
#include <cstdint>
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
CtCodeGen::resolve_procedure() {

	mJumpAddresses.clear();
	mPatches.clear();
	
	std::string val;

	if (!mStream.expect_type(CtTokenType::Int, &val)) {
		std::cerr << "Expected integar for procedure id. Got: " << val << "\n";
		exit(1);
	}

	uint32_t id = std::stoul(val);

	if (!(
		mStream.expect_token("(") &&
		mStream.expect_type(CtTokenType::Int, &val) &&
		mStream.expect_token(")")
	)) {
		std::cerr << "Invalid argument formag for procedure " << id << ". Got: " << val << "\n";
		exit(1);
	}

	uint32_t arg_count = std::stoul(val);

	ct_image_builder_new_proc(&mBuilder, id, arg_count);

	if (!mStream.expect_token("{")) {
		std::cerr << "Expected { after procedure declaration." << "\n";
		exit(1);
	}

	while (true) {

		if (mStream.eof()) {
			std::cerr << "Procedure did not end with }. File ended too soon." << "\n";
			exit(1);
		}

		if (mStream.expect_token("}")) {
			break;
		}
	
		parse_instruction();
	}

	for (auto item: mPatches) {
		uint32_t* ptr = (uint32_t*) ct_image_builder_get_address(&mBuilder, item.first);
		int jump_location = mJumpAddresses[item.second];
		int current_location = item.first;
		int offset = jump_location - current_location;
		memcpy(ptr, &offset, sizeof(offset));
	}
}

void
CtCodeGen::parse_instruction() {

	std::string val;

	if (mStream.expect_token("@")) {

		std::string label;

		mStream.expect_type(CtTokenType::Word, &label);

		mJumpAddresses[label] = mBuilder.image.header.instruction_count;
		
		mStream.expect_token(";");

		return;
	}

	if (!mStream.expect_type(CtTokenType::Word, &val)) {
		
	}
	
	if (!CtInstrMap.contains(val)) {
		std::cerr << "Unknown instruction: " << val << "\n";
		exit(1);
	}

	CtInstrSize instr = CtInstrMap.at(val);

	ct_image_builder_add_instr(&mBuilder, instr);
	
	while (!mStream.expect_token(";")) {

		if (mStream.expect_token("$")) {

			mStream.expect_type(CtTokenType::Int, &val);
			byte slot_index = (byte) std::stoi(val);
			ct_image_builder_add_u8(&mBuilder, (uint8_t) slot_index);

		} else if (mStream.expect_type(CtTokenType::Int, &val)) {

			int number = std::stoi(val);
			ct_image_builder_add_u32(&mBuilder, number);

		} else if (mStream.expect_type(CtTokenType::Float, &val)) {
			
			float number = std::stof(val);
			ct_image_builder_add_f32(&mBuilder, number);

		} else if (mStream.expect_type(CtTokenType::Word, &val)) {
			
			if (mJumpAddresses.contains(val)) {
				
				int jump_location = mJumpAddresses[val];
				int current_location = mBuilder.image.header.instruction_count + 4;
				int offset = jump_location - current_location;

				ct_image_builder_add_u32(&mBuilder, offset);

			} else {

				mPatches[mBuilder.image.header.instruction_count] = val;
				ct_image_builder_add_u32(&mBuilder, 0);
			}
		}
	}
}


void
CtCodeGen::generate(CtTokenStream stream, std::string outpath) {

	mStream = stream;

	ct_image_builder_init(&mBuilder, 16, 64);

	while (!mStream.eof()) {
		
		if (mStream.expect_token("proc")) {
			resolve_procedure();
		}
	}

	ct_image_dump(&mBuilder.image, outpath.data());
}

