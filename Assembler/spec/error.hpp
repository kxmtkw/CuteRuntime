#ifndef CUTEASM_ERROR_H
#define CUTEASM_ERROR_H

#include <format>
#include <iostream>
#include <string>
#include <vector>


struct CtErrorCollector {

	bool mHasError = false;
	std::vector<std::string> mErrorStrings;

	public:

	bool
	has_error() {return mHasError;}

	void
	add_error(uint line_num,std::string_view line, std::string details) {
		std::string error = std::format(
			"Line: {}\n|{}\n>> {}\n", line_num, line, details
		);
		mHasError = true;
		mErrorStrings.push_back(error);
	}

	void
	print() {
		for (auto str: mErrorStrings) {
			std::cout << str;
		}
	}
};

#endif // CUTEASM_ERROR_H
