#include "Screem_element_lib.h"
#include <iostream>

using namespace std;

int main() {
	Button button;
	button.text = "hello my friends.";
	button.position = COORD {20, 10};
	root.add_son(button);
	start();
	while (1) {
		Call_back call_back = root.update();
//		cout << call_back.size() << endl;
	}
	stop();
	system("pause");
}
