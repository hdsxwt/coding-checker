#include "Screem_element_lib.h"
#include <iostream>

using namespace std;

int main() {
	Menu button;
	button.text = "hello my friends.";
	button.position = COORD {20, 10};
	button.visible = true;
	root.add_son(button);
	start();
	int cnt = 0;
	while (1) {
		Call_back call_back = root.update();
		if (call_back.size() != 0) {
			cnt++;
			cout << cnt << endl; //
		}
	}
	stop();
	system("pause");
}
