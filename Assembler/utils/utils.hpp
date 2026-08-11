#ifndef CUTEASM_UTILS_HPP
#define CUTEASM_UTILS_HPP

#include <cstdint>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sstream>


namespace CtUtils {


	static std::pair<unsigned int, unsigned int>
	get_character_location(const std::string& source, size_t index) {

		unsigned int line_count = 1;
		unsigned int last_line_at = 0;

		for (size_t i = 0; i < index; ++i) {
			if (source[i] == '\n') {
				line_count++;
				last_line_at = i;
			}
		}

		return {line_count, index - last_line_at};
	}


	static std::string
	get_line_at_index(const std::string& source, unsigned int line) {

		std::istringstream stream(source);
		std::string line_str;
		unsigned int current_line = 1;
	
		while (std::getline(stream, line_str)) {
			if (current_line == line) {
				return line_str;
			}
			current_line++;
		}
	
		return ""; 
	}

	static void 
	raise_error(const std::string& source, unsigned int char_index, std::string details) {

		auto location = get_character_location(source, char_index);
		unsigned int line = location.first;
		unsigned int col = location.second;

		std::string line_str = get_line_at_index(source, line);

		std::cout << std::format("[Error] {}\nLine: {} Col: {}\n{}\n", details, line, col, line_str);
		exit(1);
	};

	static bool 
	str_to_i16(const std::string& str, int16_t& i) {

		try {
			int64_t val = std::stoll(str);
	
			if (val < INT16_MIN || val > INT16_MAX) {
				return false;
			}
	
			i = (int16_t) val;

			return true;
	
		} catch (const std::out_of_range&) {
			return false;
		}
	}

	static bool 
	str_to_i32(const std::string& str, int32_t& i) {

		try {
			int64_t val = std::stoll(str);
	
			if (val < INT32_MIN || val > INT32_MAX) {
				return false;
			}
	
			i = (int16_t) val;
			
			return true;
	
		} catch (const std::out_of_range&) {
			return false;
		}
	}

	static bool 
	str_to_i64(const std::string& str, int64_t& i) {

		try {

			int64_t val = std::stoll(str);
			i = (int64_t) val;
			return true;

		} catch (const std::out_of_range&) {
			return false;
		}
	}

	static bool 
	str_to_f32(const std::string& str, float& f) {

		try {
			f = std::stof(str);
			return true;
		} catch (const std::out_of_range&) {
			return false;
		}

	}

	static bool 
	str_to_f64(const std::string& str, double& f) {

		try {
			f = std::stod(str);
			return true;
		} catch (const std::out_of_range&) {
			return false;
		}
		
	}
}

#endif // CUTEASM_UTILS_HPP
