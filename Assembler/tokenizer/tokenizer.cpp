#include <cctype>
#include <string>
#include <vector>

#include "utils/utils.hpp"

#include "tokens.hpp"
#include "tokenizer.hpp"



char CtTokenizer::next() {
	if (mCurrent < mSize) {
		return mSource[mCurrent++];
	}
	return '\0';
}


char CtTokenizer::peek() {
	if (mCurrent < mSize) {
		return mSource[mCurrent];
	}
	return '\0';
}


void CtTokenizer::backtrack() {
	mCurrent--;
}


bool CtTokenizer::eof() {
	return mCurrent >= mSize;
}


void CtTokenizer::eat_whitspace() {

	char c;
	c = next();

	while ((c == ' ' or c == '\n' or c == '\t') and !eof()) {
		c = next();
	}

	backtrack();
}


void CtTokenizer::read_comment() {
	while (peek() != '\n') { next(); }
}


void CtTokenizer::tokenize_word() {
	char c;
	uint start = mCurrent;

	while (mCurrent < mSize) {
		c = peek();

		if (std::isalnum(c) or c == '_') {
			next();
			continue;
		}
		break;
	}
	mTokens.emplace_back(CtToken(CtTokenType::Word, start, mCurrent-start));
}


void CtTokenizer::tokenize_number() {

	uint start = mCurrent;
	char c;
	bool is_float = false;

	while (!eof()) {

		c = peek();

		if (std::isdigit(c)) {
			next();
			continue;
		}

		if (c == '-' and start == mCurrent) {
			next();
			continue;
		}

		if (c == '.') {

			if (is_float) {
				throw_error("Illegal symbol.");
			}

			is_float = true;
			next();
			continue;
		}


		if (std::isalpha(c)) {
			throw_error("Did not expect alphabetic character.");
		}
		
		break;
	}

	mTokens.emplace_back(CtToken(is_float ? CtTokenType::Float: CtTokenType::Int, start, mCurrent-start));
}


void CtTokenizer::tokenize_char() {

	char c = next(); // consume the '

	uint start = mCurrent;
	next(); // consume the char

	c = next();
	
	if (c != '\'') {
		throw_error("Unterminated char or Too long char.");
	}

	mTokens.emplace_back(CtToken(CtTokenType::Char, start, 1));
};


void CtTokenizer::tokenize_string() {

	char c = next(); // consume the "

	uint start = mCurrent;

	c = next();

	while (c != '\"') {
		
		c = next();

		if (eof()) {
			throw_error("Unterminated string.");
		}

		if (c == '\\') {
			next();
		}
	}

	mTokens.emplace_back(CtToken(CtTokenType::String, start, mCurrent-start-1));
}
	

void CtTokenizer::tokenize_symbol() {
	char c = next();
	mTokens.emplace_back(CtToken(CtTokenType::Symbol, mCurrent-1, 1));
}


void CtTokenizer::tokenize_slot() {

	next(); // consume the the $

	uint start = mCurrent;
	char c;

	while (!eof()) {

		c = peek();

		if (std::isdigit(c)) {
			next();
			continue;
		}

		if (is_whitespace(c)) {
			break;
		}
		
		if (std::isalpha(c)) {
			throw_error("Illegal alphabetic character.");
		}

		break;
	}

	if (start == mCurrent) {
		throw_error("Empty Slot.");
	}

	mTokens.emplace_back(CtToken(CtTokenType::Slot, start, mCurrent-start));
}


void CtTokenizer::throw_error(std::string details) {
	CtUtils::raise_error(
		mSource, 
		mCurrent, 
		details
	);
}


CtTokenStream CtTokenizer::tokenize(std::string source) {

	mSource = source;
	mCurrent = 0;
	mSize = mSource.size();
	mTokens = {};

	char c;

	while (mCurrent < mSize) {

		eat_whitspace();

		c = peek();

		if (std::isalpha(c) or c == '_') {
			tokenize_word();
		}
		else if (std::isdigit(c)) {
			tokenize_number();
		}
		else if (c == '-') {
			next();
			if (std::isdigit(peek())) {
				backtrack();
				tokenize_number();
			} else {
				backtrack();
				tokenize_symbol();
			}
		}
		else if (c == '#') {
			read_comment();
		}
		else if (c == '$') {
			tokenize_slot();
		}
		else if (c == '\'') {
			tokenize_char();
		}
		else if (c == '\"') {
			tokenize_string();
		}
		else {
			tokenize_symbol();
		}
	}

	mTokens.emplace_back(CtToken(CtTokenType::EndOfFile, 0 ,0));

	return CtTokenStream(mSource, mTokens);
}