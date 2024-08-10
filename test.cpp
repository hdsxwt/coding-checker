#include<bits/stdc++.h>
using namespace std;
#include"Screen_element_lib.h"


int main() {
	Button button1;
	button1.set_visible(true);
	button1.set_text("button1");
	button1.set_id(1);
	Button button2;
	button2.set_visible(true);
	button2.set_text("button2");
	button2.set_deletable(true);
	button1.set_id(2);
	root.add_son(&button1);
	root.add_son(&button2);
	
	screen_element_controller.start();
	while (1) {
		Call_back call_back = root.update();
	}
}


