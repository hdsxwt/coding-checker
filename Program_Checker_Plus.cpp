//#include<windows.h>
#include "checker_lib.h"
#include "Screen_element_lib.h"
#include "json.hpp"
#include <bits/stdc++.h>

using namespace std;
using nlohmann::json;

int main() {
	checker_controller.start();
	screen_element_controller.start();
}
