#ifndef CHECKERLIB
#define CHECKERLIB

#include "json.hpp"
#include <Shldisp.h>

#include<fstream>


using namespace std;

using std::string;
using std::istringstream;
using nlohmann::json;

const string tested_program = "tested_program";
const string data_generator = "data_generator";
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

class Checker;
class Checker_controller;

const int hshb = 128;
const int mod = 998244353;


int hsh(string str) {
	int ret = 0;
	for (size_t i = 0; i < str.size(); i++) ret = ((long long)ret * hshb % mod + str[i]) % mod;
	return ret + 1000;
}


class Checker { // tested
private:
	int id;
	short siz;
	short siz_all;
	string path;
	string name;
	short mode;
	bool vis_tested;
	bool vis_ans;
	bool vis_data;
#define CHECKERMODE_WHOLEMATCH 0
#define CHECKERMODE_IGNORESPACE 1
#define CHECKERMODE_FLOAT3 2
#define CHECKERMODE_FLOAT5 3
#define CHECKERMODE_FLOAT8 4
public:
	void set_id (string name) { set_id(hsh(name)); }
	void set_id   (int id) {this -> id = id; }
	void set_siz  (short siz) {this -> siz = siz;}
	void set_siz_all (short siz_all) {this -> siz_all = siz_all; }
	void set_path (string path) { this -> path = path; }
	void set_mode (short mode) {
		if (mode < 0 || mode > 4) throw ("Not a Cheker mode.");
		this -> mode = mode;
		write();
	}
	void set_name (string name) { this -> name = name; }
	int get_id        () { return id; }
	short get_mode    () { return mode; }
	short get_siz     () { return siz; }
	short get_siz_all () { return siz_all; }
	string get_path   () { return path; }
	string get_name   () { return name; }
	bool get_vis_tested () { return vis_tested; }
	bool get_vis_ans    () { return vis_ans; }
	bool get_vis_data   () { return vis_data; }
private:
	void cpy() {
		if (!directory_exists(path + "task\\")) {
			create_directory(path + "task\\");
		}
		string path1 = path + "in.txt";
		string path2 = path + "out.txt";
		string path3 = path + "ans.txt";
		string path_1 = path + "task\\in" + to_string(siz) + ".txt";
		string path_2 = path + "task\\out" + to_string(siz) + ".txt";
		string path_3 = path + "task\\ans" + to_string(siz) + ".txt";
		CopyFile(path1.data(), path_1.data(), true);
		CopyFile(path2.data(), path_2.data(), true);
		CopyFile(path3.data(), path_3.data(), true);
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
		string command = "\"" + path + file + ".exe\" > " + path + "in.txt";
		system(command.data());
	}
	void generate_answer(string file) {
		string command = "\"" + path + file + ".exe\" < " + path + "in.txt > " + path + "ans.txt";
		system(command.data());
	}
	void running(string file) {
		string command = "\"" + path + file + ".exe\" < " + path + "in.txt > " + path + "out.txt";
		system(command.data());
	}
public:
	void write() {
		json config;
		config["siz"] = siz;
		config["siz_all"] = siz_all;
		config["mode"] = mode;
		config["vis_data"] = vis_data;
		config["vis_ans"] = vis_ans;
		config["vis_tested"] = vis_tested;
		ofstream out(path + "config.json");
		out << setw(4) << config << endl;
		out.close();
	}
	bool start() { // read
		if (!directory_exists(path)) {
			if (!create_directory(path)) return false;
		}
		if (!directory_exists(path + "task\\")) {
			if (!create_directory(path + "task\\")) return false;
		}
		ifstream in(path + "config.json");
		if (in.is_open()) {
			json config;
			try {
				in >> config;
			} catch (json::parse_error& e) {
				in.close();
				return false;
			}
			in.close();
			this -> siz = config["siz"];
			this -> siz_all = config["siz_all"];
			this -> mode = config["mode"];
			this -> vis_data = config["vis_data"];
			this -> vis_ans =  config["vis_ans"];
			this -> vis_tested = config["vis_tested"];
		}
		in.close();
		write();
		return true;
	}
	Checker () {
		id = 0;
		siz = 0;
		path = "";
		name = "";
		mode = CHECKERMODE_IGNORESPACE;
		vis_ans = vis_data = vis_tested = false;
	}
	bool compile_file(string file) {
		string command = "g++ \"" + path + file + ".cpp\" -o \"" + path + file + ".exe\" -std=c++14 -Wall -O2 " +
				"> \"" + path + "compile.txt\" 2>&1";
		bool ret = (system(command.data()) == 0);
		if (file == tested_program) vis_tested = ret;
		if (file == data_generator) vis_data = ret;
		if (file == answer_generator) vis_ans = ret;
		write();
		return ret;
	}
	void edit_file(string file) {
		string command = "notepad \"" + path + file + ".cpp \" ";
		system(command.data());
	}
	bool check() {
		generate_data(data_generator);
		generate_answer(answer_generator);
		running(tested_program);
		bool ret;
		if (mode == CHECKERMODE_WHOLEMATCH) ret = whole_match();
		else if (mode == CHECKERMODE_IGNORESPACE) ret = ignore_space();
		else if (mode == CHECKERMODE_FLOAT3) ret = float3();
		else if (mode == CHECKERMODE_FLOAT5) ret = float5();
		else if (mode == CHECKERMODE_FLOAT8) ret = float8();
		else ret = ignore_space();
		if (!ret) {
			siz++;
			cpy();
		}
		siz_all++;
		write();
		return ret;
	}
	void del_files(bool typ = false) {
		string del_path = "" + path + "task\\";
		string command = "del " + del_path + " /Q";
		system(command.data());
		if (typ) RemoveDirectory(del_path.data());
		siz = siz_all = 0;
		write();
	}
	
	void remove() {
		del_files(true);
		string command = "del \"" + path + "\" /Q";
		system(command.data());
		RemoveDirectory(path.data());
	}
	
	void open_files() {
		string tasks = path + "task\\";
		ShellExecute(NULL, NULL, tasks.data(), NULL, NULL, SW_MAXIMIZE);
	}
};

class Checker_controller {
private:
	bool illegal (char c) {
		return c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '\"' || c == '!' ||
		c == '<' || c == '>' || c == '|' || c == ' ';
	}
public:
	size_t siz;
	vector<Checker*> checkers;
	void write() {
		json config;
		config["siz"] = siz;
		vector<string> names(0);
		for (auto p:checkers) {
			names.push_back(p -> get_name());
		}
		config["names"] = names;
		ofstream out(HOME + "config.json");
		out << setw(4) << config << endl;
		out.close();
	}
	bool start() {
		if (!directory_exists(HOME)) {
			if (!create_directory(HOME)) return false;
		}
		ifstream in(HOME + "config.json");
		if (in.is_open()) {
			json config;
			try {
				in >> config;
			} catch (json::parse_error& e) {
				in.close();
				return false;
			}
			in.close();
			siz = config["siz"];
			vector<string> names = config["names"].get<vector<string>>();
			
			if (siz != names.size()) return false;
			
			for (size_t i = 0; i < siz; i++) {
				Checker* checker = new Checker();
				checkers.push_back(checker);
				for (size_t j = 0; j < names[i].size(); j++) {
					char &c = names[i][j];
					if (illegal(c)) c = '_';
				}
				checker -> set_name(names[i]);
				checker -> set_path(HOME + names[i] + "\\");
				checker -> set_id(names[i]);
				if (!checker -> start()) return false;
			}
		}
		in.close();
		write();
		return true;
	}
	
	void stop() {
		write();
	}
	bool add_new_task(string name) {
		Checker* checker = new Checker();
		checkers.push_back(checker);
		for (size_t i = 0; i < name.size(); i++) {
			char &c = name[i];
			if (illegal(c)) c = '_';
		}
		checker -> set_name(name);
		checker -> set_path(HOME + name + "\\");
		checker -> set_id(name);
		if (!checker -> start()) return false;
		siz++;
		write();
		return true;
	}
private:
	void del(int p) {
		checkers[p] -> remove();
		delete checkers[p];
		siz--;
		checkers.erase(checkers.begin() + p);
	}
public:
	void del_task (string name) {
		for (size_t i = 0; i < siz; i++) {
			if (checkers[i] -> get_name() == name) {
				del(i);
				break;
			}
		}
		write();
	}
	void del_task (int id) {
		for (size_t i = 0; i < siz; i++) {
			if (checkers[i] -> get_id() == id) {
				del(i);
				break;
			}
		}
		write();
	}
} checker_controller;

#endif

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
