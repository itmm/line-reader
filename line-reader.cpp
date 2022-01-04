#include <sstream>
#include "line-reader.h"

File_Position File_Position::parse_line_macro(const std::string &line) {
	do {
		if (line.substr(0, 6) != "#line ") { break; }

		int idx { 6 };
		int pos { 0 };
		while (idx < line.length() && isdigit(line[idx])) {
			pos = pos * 10 + line[idx++] - '0';
		}
		if (pos < 1) { break; }
		if (idx == line.length()) {
			return File_Position { file_name(), pos };
		}
		if (line.substr(idx, 2) == " \"" &&
			idx + 2 < line.size() && line.back() == '"'
		) {
			auto name = line.substr(
				idx + 2, line.size() - idx - 3
			);
			return File_Position { name, pos };
		}
	} while (false);
	return File_Position { };

}

std::string File_Position::line_macro(const File_Position &pos) {
	if (pos == *this) { return std::string { }; }
	std::ostringstream out;

	out << "#line " << pos.line();
	if (pos.file_name() != file_name()) {
		out << " \"" << pos.file_name() << '"';
	}
	*this = pos;
	return out.str();
}

std::ostream &File_Position::change_pos(std::ostream &out, const File_Position &pos) {
	if (pos == *this) { return out; }
	return out << line_macro(pos) << '\n';
}

bool Line_Reader::next(std::string &line) {
	for (;;) {
		if (! std::getline(in_, line)) { return false; }
		File_Position p { pos_.parse_line_macro(line) };
		if (p) { pos_ = p; --pos_; continue; }
		break;
	}
	++pos_;
	return true;
}

