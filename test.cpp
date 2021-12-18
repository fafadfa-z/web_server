#include <iostream>
#include <utility>
#include <string>
#include <memory>

using namespace std;

class A
{
public:
	virtual void fun()
	{
		cout<<"A fun()  "<<this<<endl;
	}
};
class B : public A
{
public:
	void fun() override
	{
		cout<<"B fun()  "<<this<<endl;		

	}
};

int main1(int argc, char *argv[])
{
	A* a =new B;




	// cout << "CHILD object cb's addr is=" << cb << endl;
	return 0;
}