#include "menu.hpp"
#include <iostream>

using namespace std;

int main() {
	Menu menu;
	Button button;
	button.position = COORD{10, 10};
	button.event = "exit";
	button.text = "hello";
	button.position = COORD{10, 10};
	button.visible = true;
	menu.push(button);
	menu.start();
	for (int i = 1; i; i++) {
		string s;
		runMenu(menu, s);
		if (s == "exit") {
			cout << "exit" << "\n";
		}
		cout << "cnt: " << i << "\r";
	}
	menu.stop();
}
