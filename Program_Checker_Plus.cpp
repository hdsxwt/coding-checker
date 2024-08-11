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

void stop();


int main() {
	
	home_button.set_visible(true);
	home_button.set_clickable(true);
	home_button.set_id(1);
	home_button.set_text("Home");
	root.add_son(&home_button);
	
	menu.set_visible(true);
	menu.set_foldable(true);
	menu.fold();
	menu.set_id(2);
	menu.set_text("Tested Programme Menu");
	root.add_son(&menu);
		
	add_tk.set_text("Add new task +");
	add_tk.set_visible(true);
	add_tk.set_clickable(true);
	add_tk.set_id(10);
	menu.add_son(&add_tk);
	
	ext.set_visible(true);
	ext.set_clickable(true);
	ext.set_id(3);
	ext.set_text("Exit");
	ext.set_normal_color(Color(RED, BLACK));
	ext.set_highlight_color(Color(BRIGHTWHITE, BRIGHTRED));
	ext.set_click_color(Color(RED, BLACK));
	root.add_son(&ext);
	
	string line = "|\n";
	line = line * 28;
	seperate_line.set_visible(true);
	seperate_line.set_text(line);
	seperate_line.set_position(30,0);
	seperate_line.set_id(50);
	root.add_son(&seperate_line);
	
	welcome.set_id(100);
	welcome.set_position(35, 0);
	root.add_son(&welcome);
	
	welcome_shown_text.set_id(101);
	welcome_shown_text.set_visible(true);
	welcome_shown_text.set_text(welcome_text);
	welcome.add_son(&welcome_shown_text);
	
	welcome_author.set_id(102);
	welcome_author.set_visible(true);
	welcome_author.set_clickable(true);
	welcome_author.set_text("Github: hdsxwt");
	welcome.add_son(&welcome_author);
	
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
				screen_element_controller.set_mouse_position(3, 28);
				printf("Please enter the name of the task: |");
				string s;
				cin >> s;
				screen_element_controller.set_mouse_position(3, 28);
				for (int i = 1; i <= 100; i++) putchar(' ');
				checker_controller.add_new_task(s);
				
				Button* button = new Button();
				button -> set_text(s);
				button -> set_id(checker_controller.checkers.back() -> get_id());
				button -> set_visible(true);
				button -> set_clickable(true);
				button -> set_deletable(true);
				menu.add_son(button);
			} else if (x.typ == CALL_BACK_CONTENG_DELETE) {
				checker_controller.del_task(x.id);
				screen_element_controller.start();
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
