#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
#include"Screen_element_lib.h"


Button ext;

void stop();

int main() {
	screen_element_controller.start();
	Button button1;
	button1.set_visible(true);
	button1.set_text("button1");
	button1.set_id(1);
	Button button2;
	button2.set_visible(true);
	button2.set_text("button2");
	button2.set_id(1);
	root.update();
	root.add_son(&button1);
	root.update();
	root.add_son(&button2);
	while (1) {
		root.update();
	}
	screen_element_controller.stop();
}
