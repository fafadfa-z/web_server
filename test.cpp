#include <iostream>
#include <unordered_set>

using namespace std;


class mytype
{
public:

	friend class  hash_value;

	bool operator==(const mytype& another) const
	{
		return this->value_+this->another_==another.value_ +another.another_;
	}
	// bool operator<(const mytype& another)
	// {
	// 	return this->value_+this->another_<another.value_ +another.another_;
	// }

	int value_=0;
	int another_=0;
};
class  hash_value
{
public:
	size_t operator()(const mytype& val) const 
	{
		return static_cast<size_t>(val.value_);
	}

};

int main11(int argc, char *argv[])
{
	char a=0;

	char *p=&a;
	char *q=p+1;

	std::cout<<reinterpret_cast<unsigned long>(p)<<std::endl;
	std::cout<<reinterpret_cast<unsigned long>(q)<<std::endl;	


	// std::unordered_set<mytype,hash_value> se;

	// mytype a{1,2};
	// mytype b{2,3};

	

	// se.insert(a);
	// se.insert(b);

	
	return 0;
}