#ifndef CUTEASM_UTILS_HPP
#define CUTEASM_UTILS_HPP

#include <format>
#include <iostream>
#include <string_view>
#include <vector>


namespace CtUtils {



	struct ErrorCollector {

		bool mHasError = false;
		std::vector<std::string> mErrorStrings;

		public:

		bool
		has_error() {return mHasError;}

		void
		add_error(int line_num, std::string_view line, std::string details) {
			std::string error = std::format(
				"[ERROR] {}\nLine: {}\n{}\n", details, line_num, line
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


	static unsigned int 
	count_lines_up_to_index(std::string_view source, size_t index) {
		if (source.empty()) {
			return 1;
		}

		if (index > source.size()) {
			index = source.size();
		}

		unsigned int line_count = 1;
		for (size_t i = 0; i < index; ++i) {
			if (source[i] == '\n') {
				line_count++;
			}
		}

		return line_count;
	}


	static std::string_view
	get_line_at_index(std::string_view source, size_t index) {
		if (source.empty() || index >= source.size()) {
			return {};
		}

		size_t line_start = source.rfind('\n', index);
		line_start = (line_start == std::string_view::npos) ? 0 : line_start + 1;

		size_t line_end = source.find('\n', index);
		if (line_end == std::string_view::npos) {
			line_end = source.size();
		}

		return source.substr(line_start, line_end - line_start);
	}

}

#endif // CUTEASM_UTILS_HPP
