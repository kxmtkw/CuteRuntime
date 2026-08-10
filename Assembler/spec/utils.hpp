#ifndef CUTEASM_UTILS_HPP
#define CUTEASM_UTILS_HPP

#include <string_view>

static unsigned int 
ct_utils_count_lines_up_to_index(std::string_view source, size_t index) {
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
ct_utils_get_line_at_index(std::string_view source, size_t index) {
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

#endif // CUTEASM_UTILS_HPP
