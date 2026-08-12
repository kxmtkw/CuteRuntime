
#ifndef CUTEASM_INSTRUCTIONS_HPP
#define CUTEASM_INSTRUCTIONS_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}

enum class CtInstrOperandType {
	Slot,
	I8,
	I16,
	I32,
	U32,
	I64,
	F32,
	F64
};

static unsigned int 
ct_instr_operand_size(CtInstrOperandType type) {
	switch (type) {
		case CtInstrOperandType::Slot: return 1;
		case CtInstrOperandType::I8:   return 1;
		case CtInstrOperandType::I16:  return 2;
		case CtInstrOperandType::I32:  return 4;
		case CtInstrOperandType::I64:  return 8;
		case CtInstrOperandType::F32:  return 4;
		case CtInstrOperandType::F64:  return 8;
	}
	return 0;
}


static const std::map<std::string, std::pair<CtInstr, std::vector<CtInstrOperandType>>> ct_instr_map = {

	{"halt",        {CT_INSTR_HALT,        {CtInstrOperandType::Slot}}},
	{"null",        {CT_INSTR_NULL,        {}}},

	{"out",         {CT_INSTR_OUT,         {CtInstrOperandType::I8, CtInstrOperandType::Slot}}},

	{"mov",         {CT_INSTR_MOV,         {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},

	{"loadi16",        {CT_INSTR_LOAD_I16,    {CtInstrOperandType::Slot, CtInstrOperandType::I16}}},
	{"loadi32",        {CT_INSTR_LOAD_I32,    {CtInstrOperandType::Slot, CtInstrOperandType::I32}}},
	{"loadu32",        {CT_INSTR_LOAD_U32,    {CtInstrOperandType::Slot, CtInstrOperandType::U32}}},
	{"loadf32",        {CT_INSTR_LOAD_F32,    {CtInstrOperandType::Slot, CtInstrOperandType::F32}}},

	{"i2f",         {CT_INSTR_CAST_I2F,    {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"f2i",         {CT_INSTR_CAST_F2I,    {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"u2f",         {CT_INSTR_CAST_U2F,    {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"f2u",         {CT_INSTR_CAST_F2U,    {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},

	{"addi",        {CT_INSTR_ADDI,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"subi",        {CT_INSTR_SUBI,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"muli",        {CT_INSTR_MULI,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"divi",        {CT_INSTR_DIVI,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"modi",        {CT_INSTR_MODI,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"negi",        {CT_INSTR_NEGI,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"absi",        {CT_INSTR_ABSI,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},

	{"divu",        {CT_INSTR_DIVU,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"modu",        {CT_INSTR_MODU,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},

	{"inc",         {CT_INSTR_INC,         {CtInstrOperandType::Slot}}},
	{"dec",         {CT_INSTR_DEC,         {CtInstrOperandType::Slot}}},

	{"addf",        {CT_INSTR_ADDF,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"subf",        {CT_INSTR_SUBF,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"mulf",        {CT_INSTR_MULF,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"divf",        {CT_INSTR_DIVF,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"negf",        {CT_INSTR_NEGF,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"absf",        {CT_INSTR_ABSF,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},

	{"and",         {CT_INSTR_LOGIC_AND,   {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"or",          {CT_INSTR_LOGIC_OR,    {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"not",         {CT_INSTR_LOGIC_NOT,   {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"xor",         {CT_INSTR_LOGIC_XOR,   {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},

	{"band",        {CT_INSTR_BIT_AND,     {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"bor",         {CT_INSTR_BIT_OR,      {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"bxor",        {CT_INSTR_BIT_XOR,     {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"bnot",        {CT_INSTR_BIT_NOT,     {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"bshl",        {CT_INSTR_BIT_SHL,     {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"bshr",        {CT_INSTR_BIT_SHR,     {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"bshra",       {CT_INSTR_BIT_SHRA,    {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},

	{"cmpi",        {CT_INSTR_CMPI,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"cmpu",        {CT_INSTR_CMPU,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"cmpf",        {CT_INSTR_CMPF,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"eq",          {CT_INSTR_EQ,          {CtInstrOperandType::Slot}}},
	{"ne",          {CT_INSTR_NOT_EQ,      {CtInstrOperandType::Slot}}},
	{"lt",          {CT_INSTR_LESS,        {CtInstrOperandType::Slot}}},
	{"le",          {CT_INSTR_LESS_EQ,     {CtInstrOperandType::Slot}}},
	{"gt",          {CT_INSTR_GREATER,     {CtInstrOperandType::Slot}}},
	{"ge",          {CT_INSTR_GREATER_EQ,  {CtInstrOperandType::Slot}}},

	{"jmp",         {CT_INSTR_JMP,         {CtInstrOperandType::I32}}},
	{"jmpeq",       {CT_INSTR_JMP_EQ,      {CtInstrOperandType::I32}}},
	{"jmpne",       {CT_INSTR_JMP_NE,      {CtInstrOperandType::I32}}},
	{"jmpgt",       {CT_INSTR_JMP_GT,      {CtInstrOperandType::I32}}},
	{"jmpge",       {CT_INSTR_JMP_GE,      {CtInstrOperandType::I32}}},
	{"jmplt",       {CT_INSTR_JMP_LT,      {CtInstrOperandType::I32}}},
	{"jmple",       {CT_INSTR_JMP_LE,      {CtInstrOperandType::I32}}},
	{"jmpif",       {CT_INSTR_JMP_IF,      {CtInstrOperandType::Slot, CtInstrOperandType::I32}}},
	{"jmpifn",      {CT_INSTR_JMP_IFNOT,   {CtInstrOperandType::Slot, CtInstrOperandType::I32}}},
	
	{"call",        {CT_INSTR_CALL,        {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"ret",         {CT_INSTR_RETURN,      {}}},
	{"retval",      {CT_INSTR_RETURN_VAL,  {CtInstrOperandType::Slot}}},
	{"modcall",     {CT_INSTR_MOD_CALL,    {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},

	{"connew",      {CT_INSTR_CON_NEW,     {CtInstrOperandType::Slot, CtInstrOperandType::I32}}},
	{"conget",      {CT_INSTR_CON_GET,     {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"conset",      {CT_INSTR_CON_SET,     {CtInstrOperandType::Slot, CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"consize",     {CT_INSTR_CON_SIZE,    {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}},
	{"concopy",     {CT_INSTR_CON_COPY,    {CtInstrOperandType::Slot, CtInstrOperandType::Slot}}}
};

#endif // CUTEASM_INSTRUCTIONS_HPP