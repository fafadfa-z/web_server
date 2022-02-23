#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>

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

int main1(int argc, char *argv[])
{
	vector<string>vec={"1,2,3,4,5"};

	vec.reserve(10);

	string& str=vec[0];

	for(int i=0;i<1;i++)
	{
		vec.push_back(std::to_string(i));
	}

	cout<<str<<endl;

	
	return 0;
}