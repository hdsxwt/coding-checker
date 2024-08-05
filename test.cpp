#define DBG_MESSAGE
#include <cassert>
#include "Screem_element_lib.h"
#include <iostream>

using namespace std;

int main() {
	
	Menu button1;
	button1.set_text("hello my friends_1.");
	button1.set_visible(true);
	button1.set_clickable(true);
	button1.set_id(1);
	root.add_son(&button1);
	
	Menu button2;
	button2.set_text("hello my friends_2.");
	button2.set_visible(true);
	button2.set_clickable(true);
	button2.set_id(2);
	button2.set_foldable(true);
	button1.add_son(&button2);
	
	Menu button3;
	button3.set_text("hello my friends_3.");
	button3.set_visible(true);
	button3.set_clickable(true);
	button3.set_id(3);
	button2.add_son(&button3);
	
	Menu button4;
	button4.set_text("hello my friends_4.");
	button4.set_visible(true);
	button4.set_clickable(true);
	button4.set_id(4);
	root.add_son(&button4);
	
	start();
	while (1) {
		Call_back call_back = root.update();
		if (!call_back.empty()) {
			if (call_back.ids[0] == 4) {
				button4.set_text("nb");
			}
		}
	}
	stop();
}
