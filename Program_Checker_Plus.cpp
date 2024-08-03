#include "Screem_element_lib.h"
#include <iostream>

using namespace std;

int main() {
	Menu button;
	button.text = "hello my friends.";
	button.position = COORD {20, 10};
	button.visible = true;
	button.clickable = true;
	button.id = 5;
	root.add_son(button);
	start();
	while (1) {
		Call_back call_back = root.update();
		if (call_back.size() != 0) {
			cout << call_back.ids[0];
		}
	}
	stop();
	system("pause");
}
