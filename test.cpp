#include<bits/stdc++.h>

using namespace std;

class A {
public:
	int a;
	A() {}
	virtual A* get_class_name() { return this; }
};

class B: public A {
public:
	int b;
	B(): A() {}
	void hello() { cout << "hello" << " " << this -> a << endl; }
	virtual B* get_class_name() { return this; }
};

int main() {
	A* p = new B();
	p -> a = 100;
	B* p_b = dynamic_cast<B*>(p);
	p_b -> hello();
	cout << (typeid(*(p -> get_class_name())) == typeid(B)) << endl;
	
}
