#pragma once

#include <string>
#include <fstream>
#include <utility>

class File_Position {
	std::string file_name_;
	int line_;
public:
	File_Position(): file_name_ { }, line_ { 0 } { }
	File_Position(std::string file_name, int line):
		file_name_ {std::move( file_name )}, line_ { line }
	{ }
	explicit operator bool() const { return line_ > 0; }
	[[nodiscard]] const std::string &file_name() const { return file_name_; };
	[[nodiscard]] int line() const { return line_; }
	File_Position &operator++() { ++line_; return *this; }

	File_Position parse_line_macro(const std::string &line);
	std::string line_macro(const File_Position &pos);
	std::ostream &change_pos(std::ostream &out, const File_Position &pos);
};


inline bool operator==(const File_Position &a, const File_Position &b) {
	return a.line() == b.line() && a.file_name() == b.file_name();
}

class Line_Reader {
	std::istream &in_;
	File_Position pos_;
public:
	Line_Reader(const std::string &file_name, std::istream &in):
		in_ { in }, pos_ { file_name, 0 }
	{ }
	virtual ~Line_Reader() = default;

	[[nodiscard]] const File_Position &pos() const { return pos_; }

	bool next(std::string &line);
};

class File_Line_Reader: public Line_Reader {
	std::ifstream file_;
public:
	explicit File_Line_Reader(const std::string &file_name):
		file_ { file_name.c_str() }, Line_Reader { file_name, file_ }
	{ }
};