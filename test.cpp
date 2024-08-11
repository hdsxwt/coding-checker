#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
#include"Screen_element_lib.h"


Button ext;

void stop();

int main() {
	Button button1;
	button1.set_visible(true);
	button1.set_text("button1");
	button1.set_id(1);
	button1.set_clickable(true);
	Button button3;
	button3.set_visible(true);
	button3.set_text("button3");
	button3.set_id(4);
	button3.set_clickable(true);
	Button button2;
	button2.set_visible(true);
	button2.set_text("button2advasdvadsv");
	button2.set_clickable(true);
	button2.set_deletable(true); // TODO
	button2.set_id(2);
	root.add_son(&button1);
	root.add_son(&button2);
	root.add_son(&button3);
	
	ext.set_visible(true);
	ext.set_clickable(true);
	ext.set_id(3);
	ext.set_text("Exit");
	ext.set_normal_color(Color(RED, BLACK));
	ext.set_highlight_color(Color(BRIGHTWHITE, BRIGHTRED));
	ext.set_click_color(Color(RED, BLACK));
	root.add_son(&ext);
	
	screen_element_controller.start();
	while (1) {
		Call_back call_back = root.update();
		if (!call_back.empty()) {
			if (call_back[0].id == 3) {
				stop();
			}
		}
	}
}

void stop() {
	screen_element_controller.set_mouse_position(0,23);
	screen_element_controller.stop();
	exit(0);
}
