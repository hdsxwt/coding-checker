#define DBG_MESSAGE
#include <cassert>
#include "Screem_element_lib.h"
#include <iostream>

using namespace std;

int main() {
	
	Menu button1;
	button1.text = "hello my friends_1.";
	button1.visible = true;
	button1.clickable = true;
	button1.id = 1;
	root.add_son(&button1);
	
	Menu button2;
	button2.text = "hello my friends_2.";
	button2.visible = true;
	button2.clickable = true;
	button2.id = 2;
	button2.foldable = true;
	button1.add_son(&button2);
	
	Menu button3;
	button3.text = "hello my friends_3.";
	button3.visible = true;
	button3.clickable = true;
	button3.id = 3;
	button2.add_son(&button3);
	
	Menu button4;
	button4.text = "hello my friends_4.";
	button4.visible = true;
	button4.clickable = true;
	button4.id = 4;
	root.add_son(&button4);
	
	start();
	while (1) {
		Call_back call_back = root.update();
		if (!call_back.empty()) {
			if (call_back.ids[0] == 2) {
				button2.fold();
			} if (call_back.ids[0] == 1) {
				button2.unfold();
			} if (call_back.ids[0] == 4) {
				button4.text = "nb";
			}
		}
	}
	stop();
	system("pause");
}
