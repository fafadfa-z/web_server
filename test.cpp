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

#include "logger.h"
#include "local_message.h"


int main111(int argc, char *argv[])
{	

	Base::LocalMassage::readConfig("/home/admin/code/web_server/config.conf");
 	Log::init();

	while(1)
	{
		LOG_FATAL<<"1234567890"<<Log::end;
	}


	return 0;
}