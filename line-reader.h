#pragma once

#include <string>
#include <fstream>
#include <utility>
#include <list>
#include <memory>
#include <iostream>

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

class Line_Reader_Pool {
	std::list<std::unique_ptr<Line_Reader>> pool_;
	File_Position empty_pos_ { "no-file", -1 };
public:
	void populate(int argc, const char *argv[]) {
		bool no_args { false };
		for (int i { 1 }; i < argc; ++i) {
			std::string arg { argv[i] };
			if (arg[0] == '-' && ! no_args) {
				if (arg == "--") { no_args = true; continue; }
				if (arg == "-") { push_back("", std::cin); }
			} else {
				push_back(arg);
			}
		}
		if (pool_.empty()) {
			push_back("", std::cin);
		}
	}
	void push_back(const std::string &file_name) {
		pool_.push_back(std::make_unique<File_Line_Reader>(file_name));
	}
	void push_back(const std::string &file_name, std::istream &in) {
		pool_.push_back(std::make_unique<Line_Reader>(file_name, in));
	}
	void push_front(const std::string &file_name) {
		pool_.push_front(std::make_unique<File_Line_Reader>(file_name));
	}
	void push_front(const std::string &file_name, std::istream &in) {
		pool_.push_front(std::make_unique<Line_Reader>(file_name, in));
	}
	const File_Position &pos() const {
		return pool_.empty() ? empty_pos_ : pool_.front()->pos();
	}
	bool next(std::string &line) {
		while (! pool_.empty()) {
			if (pool_.front()->next(line)) { return true; }
			pool_.pop_front();
		}
		return false;
	}
};
