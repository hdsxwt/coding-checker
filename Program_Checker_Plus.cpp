#include "menu.hpp"

using namespace std;

int main() {
	Menu menu;
	Button button;
	button.text = "exit";
	button.event = "fuck";
	button.position = {20, 10};
	button.visible = 1;
	button.clickable = 1;
	
	menu.push(button);
	menu.start();
	while (1) {
		
		string s;
		runMenu(menu, s);
		if (s == "fuck") {
			int x = menu.unfold(100, 100);
		}
	}
	menu.stop();
	return 0;
}