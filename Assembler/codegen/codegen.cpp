
#include "codegen/codegen.hpp"
#include <cstddef>
#include <memory>
#include <string.h>
#include <string>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}

#include "tokenizer/tokens.hpp"
#include "spec/instructions.hpp"
#include "spec/program.hpp"


static inline void
add_bytes_to_vector(std::vector<byte>& vector, unsigned int size, void* src) {
	size_t old_size = vector.size();
	vector.resize(vector.size() + size);
	byte* ptr = vector.data() + old_size;
	memcpy(ptr, src, size);
}



void
CtCodeGen::write_image(std::string outpath) {

	std::vector<CtImageProcedure> image_procedures(mProgram->procedures.size());

	for (auto& proc: mProgram->procedures) {
		
		if (proc->id >= image_procedures.size()) {
			image_procedures.resize(proc->id);
		}

		CtImageProcedure image_proc = {.bytecode_index = proc->address, .arg_count = proc->arg_count};
		image_procedures[proc->id] = image_proc;
	}

	mImage.header.instruction_count = mProgram->instructions.size();
	mImage.header.procedure_count = mProgram->procedures.size();

	mImage.instruction_pool = (CtInstrSize*) mProgram->instructions.data();
	mImage.procedure_table = image_procedures.data();
	
	ct_image_write(&mImage, outpath.data());
}



std::unique_ptr<CtProcedure>
CtCodeGen::resolve_procedure() {

	auto procedure = std::make_unique<CtProcedure>();

	mJumpAddresses.clear();
	mPatches.clear();
	
	std::string val;

	if (!mStream.expect_type(CtTokenType::Int, &val)) {
		// invalid
	}

	procedure->id = std::stoul(val);

	if (!(
		mStream.expect_token("(") &&
		mStream.expect_type(CtTokenType::Int, &val) &&
		mStream.expect_token(")")
	)) {
		// invalid
	}

	procedure->arg_count = std::stoul(val);
	procedure->address = mProgram->instructions.size();

	if (!mStream.expect_token("{")) {
		// invalid
	}

	while (!mStream.eof()) {

		if (mStream.expect_token("}")) {
			break;
		}
	
		parse_instruction();
	}

	for (auto item: mPatches) {
		byte* ptr = mProgram->instructions.data() + item.first;
		int jump_location = mJumpAddresses[item.second];
		int current_location = item.first;
		int offset = jump_location - current_location;
		memcpy(ptr, &offset, sizeof(offset));
	}

	return procedure;
}

void
CtCodeGen::parse_instruction() {

	std::string val;

	if (mStream.expect_token("@")) {

		std::string label;

		mStream.expect_type(CtTokenType::Word, &label);

		mJumpAddresses[label] = mProgram->instructions.size();
		
		mStream.expect_token(";");

		return;
	}

	if (!mStream.expect_type(CtTokenType::Word, &val)) {
		
	}
	
	if (!CtInstrMap.contains(val)) {
		// error
	}

	CtInstrSize instr = CtInstrMap.at(val);

	mProgram->instructions.push_back((byte)instr);
	
	while (!mStream.expect_token(";")) {

		if (mStream.expect_token("$")) {

			mStream.expect_type(CtTokenType::Int, &val);
			byte slot_index = (byte) std::stoi(val);
			mProgram->instructions.push_back(slot_index);

		} else if (mStream.expect_type(CtTokenType::Int, &val)) {

			int number = std::stoi(val);
			add_bytes_to_vector(mProgram->instructions, 4, &number);

		} else if (mStream.expect_type(CtTokenType::Float, &val)) {
			
			float number = std::stof(val);
			add_bytes_to_vector(mProgram->instructions, 4, &number);

		} else if (mStream.expect_type(CtTokenType::Word, &val)) {
			
			if (mJumpAddresses.contains(val)) {
				
				int jump_location = mJumpAddresses[val];
				int current_location = mProgram->instructions.size() + 4;
				int offset = jump_location - current_location;

				add_bytes_to_vector(mProgram->instructions, 4, &offset);

			} else {

				mPatches[mProgram->instructions.size()] = val;
				int offset = 0;
				add_bytes_to_vector(mProgram->instructions, 4, &offset);

			}

		}

	}
}


void
CtCodeGen::generate(CtTokenStream stream, std::string outpath) {

	mStream = stream;
	mProgram = std::make_unique<CtProgram>();

	while (!mStream.eof()) {
		
		if (mStream.expect_token("proc")) {

			auto procedure = resolve_procedure();
			mProgram->procedures.push_back(std::move(procedure));

		}
	}

	write_image(outpath);
}

