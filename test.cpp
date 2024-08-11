#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
#include"Screen_element_lib.h"


Button ext;

const int maxn = 1000000;

void stop();

int main() {
}

void stop() {
	screen_element_controller.set_mouse_position(0,23);
	screen_element_controller.stop();
	exit(0);
}
