#ifndef CHECKERLIB
#define CHECKERLIB

#include "json.hpp"
#include<windows.h>
#include<bits/stdc++.h>

using namespace std;

using std::string;
using std::istringstream;
using nlohmann::json;

const string tested_program = "tested_program";
const string data_generater = "data_generater";
const string answer_generator = "answer_generator";
const string HOME = ".\\coding_checker\\";

bool directory_exists(const string& dir) {
	DWORD ftyp = GetFileAttributesA(dir.data());
	if (ftyp == INVALID_FILE_ATTRIBUTES) return false;
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;
	return false;
}

bool create_directory(const string& dirName) {
	return CreateDirectoryA(dirName.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}



class Checker {
private:
	short siz;
	string path;
	string name;
	short mode;
#define CHECKERMODE_WHOLEMATCH 0
#define CHECKERMODE_IGNORESPACE 1
#define CHECKERMODE_FLOAT3 2
#define CHECKERMODE_FLOAT5 3
#define CHECKERMODE_FLOAT8 4
public:
	void set_siz  (short siz) {this -> siz = siz;}
	void set_path (string path) { this -> path = path; }
	void set_mode (short mode) {
		if (mode < 0 || mode > 3) throw ("Not a Cheker mode.");
		this -> mode = mode;
	}
	void set_name (string name) { this -> name = name; }
	short get_mode  () { return mode; }
	short get_siz   () { return siz; }
	string get_path () { return path; }
	string get_name () { return name; }
private:
	void cpy() {
		// TODO copy
	}
	
	void get_file (string &file_out, string &file_ans) {
		file_out = path + "out.txt";
		file_ans = path + "ans.txt";
	}
	bool whole_match() {
		string file_out, file_ans;
		get_file(file_out, file_ans);
		string command = "fc \"" + file_out + "\" \"" + file_ans + "\" > NUL";
		if (system(command.data())) {
			return false;
		}
		return true;
	}
	bool ignore_space () {
		string file_out, file_ans;
		get_file(file_out, file_ans);
		ifstream out(file_out);
		ifstream ans(file_ans);
		string s_out, s_ans;
		while (out >> s_out && ans >> s_ans) {
			if (s_out != s_ans) return false;
		}
		if (out >> s_out || ans >> s_ans) return false;
		return true;
	}
	bool float3 () {
		string file_out, file_ans;
		get_file(file_out, file_ans);
		ifstream out(file_out);
		ifstream ans(file_ans);
		long double s_out, s_ans;
		while (out >> s_out && ans >> s_ans) {
			if (abs(s_out - s_ans) <= 1e-3) return false;
		}
		if (out >> s_out || ans >> s_ans) return false;
		return true;
	}
	bool float5 () {
		string file_out, file_ans;
		get_file(file_out, file_ans);
		ifstream out(file_out);
		ifstream ans(file_ans);
		long double s_out, s_ans;
		while (out >> s_out && ans >> s_ans) {
			if (abs(s_out - s_ans) <= 1e-5) return false;
		}
		if (out >> s_out || ans >> s_ans) return false;
		return true;
	}
	bool float8 () {
		string file_out, file_ans;
		get_file(file_out, file_ans);
		ifstream out(file_out);
		ifstream ans(file_ans);
		long double s_out, s_ans;
		while (out >> s_out && ans >> s_ans) {
			if (abs(s_out - s_ans) <= 1e-8) return false;
		}
		if (out >> s_out || ans >> s_ans) return false;
		return true;
	}
	void generate_data(string file) {
		string command = "\"" + path + file + "\" > in.txt";
		system(command.data());
	}
	void generate_answer(string file) {
		string command = "\"" + path + file + "\" < in.txt > ans.txt";
		system(command.data());
	}
	void running(string file) {
		string command = "\"" + path + file + "\" < in.txt > out.txt";
		system(command.data());
	}
public:
	bool start() { // siz, mode
		// TODO Checker_start
		if (!directory_exists(path)) {
			if (!create_directory(path)) return false;
		}
		ifstream in(path + name);
		if (in.is_open()) {
			json config;
			try {
				in >> config;
			} catch (json::parse_error& e) {
				return false;
			}
			this -> siz = config["siz"];
			this -> mode = config["mode"];
		}
	}
	Checker () {
		siz = 0;
		path = "";
		name = "";
		mode = CHECKERMODE_IGNORESPACE;
	}
	bool compile_file(string file) {
		string command = "g++ \"" + path + file + "\" -std=c++14 -Wall -O2";
		return system(command.data());
	}
	void edit_file(string file) {
		string command = "notepad \"" + path + file + "\" ";
		system(command.data());
	}
	bool check() {
		generate_data(data_generater);
		generate_answer(answer_generator);
		running(tested_program);
		bool ret;
		if (mode == CHECKERMODE_WHOLEMATCH) ret = whole_match();
		if (mode == CHECKERMODE_IGNORESPACE) ret = ignore_space();
		if (mode == CHECKERMODE_FLOAT3) ret = float3();
		if (mode == CHECKERMODE_FLOAT5) ret = float5();
		if (mode == CHECKERMODE_FLOAT8) ret = float8();
		return ret;
	}
	void del_all() {
		string command = "del \"" + path + "\" /Q";
		system(command.data());
	}
	void open_files() {
		ShellExecute(NULL, NULL, path.data(), NULL, NULL, SW_MAXIMIZE);
	}
};

class Checker_controller {
public:
	int siz;
	vector<Checker*> checkers;
	bool start() {
		// TODO controller start
		if (!directory_exists(HOME)) {
			if (!create_directory(HOME)) return false;
		}
		ifstream in(HOME + "config.json");
		if (in.is_open()) {
			json config;
			try {
				in >> config;
			} catch (json::parse_error& e) {
				return false;
			}
			siz = config["siz"];
			vector<string> names = config["names"].get<vector<string>>();
			for (int i = 0; i < siz; i++) {
				Checker* checker = new Checker();
				checkers.push_back(checker);
				for (size_t j = 0; j < names[i].size(); i++) {
					char &c = names[i][j];
					if (c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '\"' || 
							c == '<' || c == '>' || c == '|')
						c = '_';
				}
				checker -> set_name(names[i]);
				checker -> set_path(HOME + names[i] + "\\");
				if (!checker -> start()) return false;
			}
		}
		in.close();
		return true;
	}
	
	void del(int p) {
		checkers.erase(checkers.begin() + p, checkers.begin() + p);
	}
	
	void stop() {
	}
} checker_controller;

#endif

// TODO untested

/*
using process:
1. add new menu of a project (use different object of Checker) (add new class)
2. edit file
3. compile file
4. run()
5. clear task list
6. open working space
*/

// first json: siz, mode;
// second json: cnt;
