#include <bits/stdc++.h>
#include "checker_lib.h"
#include "Screen_element_lib.h"
#include "json.hpp"

using namespace std;
using nlohmann::json;

string operator * (const string str, int k) {
	string ret = "";
	for (int i = 0; i < k; i++) ret = ret + str;
	return ret;
}

const string welcome_text = "Welcome to use Program Checker.\n\n\
Feature:\n\n\
Support storing multiple coding!\n\
Support automatic compilation!\n\n\
Created by hdsxwt.";

Button home_button;
Button menu;
Button ext;

Button seperate_line;

Button welcome;
Button welcome_shown_text;
Button welcome_author;
Button control;
Button control_name;
Button control_start; // TODO
Button control_stop; // TODO
Button control_siz;
Button control_compile;
Button control_compile_data_generator;
Button control_compile_answer_generator;
Button control_compile_tested_program;
Button control_compile_data_generator_company;
Button control_compile_answer_generator_company;
Button control_compile_tested_program_company;
Button control_open_file; // TODO
Button control_information;

Checker* now_checker = nullptr;

Button add_tk;

/*
necessary: {
	text;
	id;
	visible
	root.add_son
*/

void show_welcome() {
	control.set_visible(false);
	welcome.set_visible(true);
}

void show_control() {
	welcome.set_visible(false);
	control.set_visible(true);
}

void add_new_task() {
	screen_element_controller.set_mouse_position(3, 28);
	printf("Please enter the name of the task: |");
	string s;
	getline(cin, s);
	screen_element_controller.set_mouse_position(3, 28);
	for (int i = 1; i <= 100; i++) putchar(' ');
	if (s == "") return;
	checker_controller.add_new_task(s);
	Button* button = new Button();
	button -> set_text(checker_controller.checkers.back() -> get_name());
	button -> set_id(checker_controller.checkers.back() -> get_id());
	button -> set_visible(true);
	button -> set_clickable(true);
	button -> set_deletable(true);
	menu.add_son(button);
}


void add_generator(Button &generator, Button &company, int a, int b, string s) {
	generator.set_id(a);
	generator.set_text(s);
	generator.set_visible(true);
	generator.set_clickable(true);
	control_compile.add_son(&generator);
	
	company.set_id(b);
	company.set_text("compile");
	company.set_visible(true);
	company.set_clickable(true);
	company.set_normal_color(Color(RED, BLACK));
	company.set_highlight_color(Color(BRIGHTRED, BLACK));
	company.set_click_color(Color(BRIGHTWHITE, BLACK));
	company.set_position(20, 0);
	company.set_height(0);
	generator.add_son(&company);
}

void update_vis() {
	auto f = [&] (Button &company, bool p) -> void {
		if (p) {
			company.set_normal_color(Color(GREEN, BLACK));
			company.set_highlight_color(Color(BRIGHTGREEN, BLACK));
			company.set_click_color(Color(BRIGHTWHITE, BLACK));
		} else {
			company.set_normal_color(Color(RED, BLACK));
			company.set_highlight_color(Color(BRIGHTRED, BLACK));
			company.set_click_color(Color(BRIGHTWHITE, BLACK));
		}
	};
	f(control_compile_data_generator_company, now_checker -> get_vis_data());
	f(control_compile_answer_generator_company, now_checker -> get_vis_ans());
	f(control_compile_tested_program_company, now_checker -> get_vis_tested());
}

void compile(string file) {
	bool ret = now_checker -> compile_file(file);
	if (ret) {
		control_information.set_text("successful!");
	} else {
		string s = now_checker -> get_path() + "compile.txt";
		ifstream in(s);
		string file_content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		control_information.set_text(file_content);
	}
	update_vis();
	screen_element_controller.start();
}

void stop();

int main() {
	
	{ // home_button -> root
		
		home_button.set_visible(true);
		home_button.set_clickable(true);
		home_button.set_id(1);
		home_button.set_text("Home");
		root.add_son(&home_button);
	}
	
	{ // menu -> root
		
		menu.set_visible(true);
		menu.set_foldable(true);
		menu.fold();
		menu.set_id(2);
		menu.set_text("Tested Programme Menu");
		root.add_son(&menu);
	}
	
	{ // add_tk -> menu
		
		add_tk.set_text("Add new task +");
		add_tk.set_visible(true);
		add_tk.set_clickable(true);
		add_tk.set_id(10);
		menu.add_son(&add_tk);
	}
	
	{ // ext -> root
		
		ext.set_visible(true);
		ext.set_clickable(true);
		ext.set_id(3);
		ext.set_text("Exit");
		ext.set_normal_color(Color(RED, BLACK));
		ext.set_highlight_color(Color(BRIGHTWHITE, BRIGHTRED));
		ext.set_click_color(Color(RED, BLACK));
		root.add_son(&ext);
	}
	
	{ // seperate_line -> root
		
		string line = "|\n";
		line = line * 28;
		seperate_line.set_visible(true);
		seperate_line.set_text(line);
		seperate_line.set_position(30,0);
		seperate_line.set_id(50);
		root.add_son(&seperate_line);
	}
	
	{ // welcome & control -> root
		
		welcome.set_id(100);
		welcome.set_position(35, 0);
		root.add_son(&welcome);
		control.set_id(200);
		control.set_position(35, 0);
		root.add_son(&control);
	}
	
	{ // welcome_shown_text -> welcome
		
		welcome_shown_text.set_id(101);
		welcome_shown_text.set_visible(true);
		welcome_shown_text.set_text(welcome_text);
		welcome.add_son(&welcome_shown_text);
	}
	
	{ // welcome_author -> welcome
		
		welcome_author.set_id(102);
		welcome_author.set_visible(true);
		welcome_author.set_clickable(true);
		welcome_author.set_text("Github: hdsxwt");
		welcome.add_son(&welcome_author);
	}
	
	{ // control_name -> control
		
		control_name.set_id(201);
		control_name.set_visible(true);
		control_name.set_normal_color(default_highlight_color);
		control.add_son(&control_name);
	}
	
	{ // control_start(202) -> control
		
		control_start.set_id(202);
		control_start.set_text("\n start \n");
		control_start.set_visible(true);
		control_start.set_clickable(true);
		control_start.set_normal_color(Color(WHITE, GREEN));
		control_start.set_highlight_color(Color(BRIGHTWHITE, BRIGHTGREEN));
		control_start.set_click_color(Color(WHITE, GREEN));
		control.add_son(&control_start);
	}
	
	{ // control_stop(212) -> control
		control_stop.set_id(212);
		control_stop.set_text("\n stop \n");
		control_stop.set_visible(true);
		control_stop.set_position(10,0);
		control_stop.set_height(0);
		control_stop.set_normal_color(Color(WHITE, BRIGHTBLACK));
		control_start.add_son(&control_stop);
	}
	
	{ // control_siz(210) -> control
		control_siz.set_id(210);
		control_siz.set_visible(true);
		control.add_son(&control_siz);
	}
	
	{ // control_compile(203) -> control
		control_compile.set_id(203);
		control_compile.set_visible(true);
		control_compile.set_position(35, 2);
		control.add_son(&control_compile);
	}
	
	{ // data_generator(204,205) -> control_compile
		add_generator(control_compile_data_generator, control_compile_data_generator_company, 204, 205, "Edit data generator");
	}
	
	{ // answer_generator(206,207) -> control_compile
		add_generator(control_compile_answer_generator, control_compile_answer_generator_company, 206, 207, "Edit answer generator");
	}
	
	{ // tested_program(208,209) -> control_compile
		add_generator(control_compile_tested_program, control_compile_tested_program_company, 208, 209, "Edit tested program");
	}
	
	{ // control_information(211) -> control
		control_information.set_id(211);
		control_information.set_visible(true);
		control_information.set_position(0, 11);
		control_information.set_max_length(70);
		control_information.set_text("information");
		control.add_son(&control_information);
	}
	
	checker_controller.start();
	screen_element_controller.start();
	for (auto p: checker_controller.checkers) {
		string s = p -> get_name();
		Button* button = new Button();
		button -> set_text(s);
		button -> set_id(hsh(s));
		button -> set_visible(true);
		button -> set_clickable(true);
		button -> set_deletable(true);
		menu.add_son(button);
	}
	show_welcome();
	while (1) {
		Call_back callback = root.update();
		for (Cal_back_content x: callback.ids) {
			if (x.id == 1) {
				show_welcome();
			} else if (x.id == 3) {
				stop();
			} else if (x.id == 102) {
				ShellExecute(NULL, "open", "https://github.com/hdsxwt", NULL, NULL, SW_SHOWNORMAL);
			} else if (x.id == 10) {
				add_new_task();
			} else if (x.typ == CALL_BACK_CONTENG_DELETE) {
				checker_controller.del_task(x.id);
				if (x.id == now_checker -> get_id()) {
					now_checker = nullptr;
					show_welcome();
				}
				screen_element_controller.start();
			} else if (x.id == 204) {
				now_checker -> edit_file(data_generator);
				screen_element_controller.start();
			} else if (x.id == 206) {
				now_checker -> edit_file(answer_generator);
				screen_element_controller.start();
			} else if (x.id == 208) {
				now_checker -> edit_file(tested_program);
				screen_element_controller.start();
			} else if (x.id == 205) {
				compile(data_generator);
			} else if (x.id == 207) {
				compile(answer_generator);
			} else if (x.id == 209) {
				compile(tested_program);
			} else {
				show_control();
				for (Checker* checker: checker_controller.checkers) if (checker -> get_id() == x.id) {
					now_checker = checker;
					break;
				}
				control_name.set_text(now_checker -> get_name());
				control_siz.set_text("checked task: " + to_string(now_checker -> get_siz()));
				update_vis();
			}
		}
	}
	screen_element_controller.stop();
	checker_controller.stop();
}

void stop() {
	screen_element_controller.set_mouse_position(0,23);
	screen_element_controller.stop();
	checker_controller.stop();
	exit(0);
}



// width  <= 110
// height <= 30
