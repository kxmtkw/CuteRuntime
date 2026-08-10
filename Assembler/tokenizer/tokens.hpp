
#ifndef CUTEASM_TOKENS_HPP
#define CUTEASM_TOKENS_HPP

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

enum class CtTokenType {
	EndOfFile,
	Word,
	Int,
	Float,
	String,
	Char,
	Symbol,
	Slot
};

struct CtToken {

	CtTokenType type;
	uint start;
	uint len;

	CtToken() = default;
	CtToken(CtTokenType t, uint s, uint l): type(t), start(s), len(l) {};
};


static inline std::string 
_ct_token_to_string(CtTokenType type) {
    switch (type) {
        case CtTokenType::EndOfFile: return "EndOfFile";
        case CtTokenType::Word:      return "Word";
        case CtTokenType::Int:       return "Int";
		case CtTokenType::Float:     return "Float";
        case CtTokenType::String:    return "String";
		case CtTokenType::Char:      return "Char";
        case CtTokenType::Symbol:    return "Symbol";
		case CtTokenType::Slot:      return "Slot";
        default:                     return "Unknown";
    }
}


class CtTokenStream {

	std::string mSource;
	std::vector<CtToken> mTokens;
	uint mCurrent;

	// resolve backslashes of string tokens
	std::string resolve_string(const std::string& str);

public:

	const std::string&
	get_source() {return mSource;}

	// get the next token
	CtToken next();
	// take a look at the next token
	CtToken peek();
	// backtrack by one token
	void backtrack();
	// back to index 0
	void reset();
	// check whether eof hit
	bool eof();

	// get the value of the token.
	std::string get_value(const CtToken& token);

	// expect a certain token type and write its value to the string provided.
	bool expect_type(CtTokenType type, std::string* dest);

	// expect a certain string literal
	bool expect_token(const std::string& dest);
	
	CtTokenStream() = default;
	CtTokenStream(std::string src, std::vector<CtToken> tokens): 
	mSource(src), mTokens(std::move(tokens)), mCurrent(0) {}
};



#endif // CUTEASM_TOKENS_HPP