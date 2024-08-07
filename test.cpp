#include<bits/stdc++.h>
#include "json.hpp"

using namespace std;

using nlohmann::json;

int main() {
	ifstream in("test.json");
	json con;
	in >> con;
	vector<int> vec;
	vec = con["a"].get<vector<int> >();
	for (auto x: vec) cout << x << endl;
	system("pause");
}
