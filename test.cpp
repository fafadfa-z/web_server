#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
#include <thread>

using namespace std;
using namespace std::chrono;

#include "logger.h"
#include "local_message.h"
#include "timmer.h"

// Time::Timmer tim(10);

void frint_time(long time)
{
	cout<<"long: "<<time<<std::endl;

}

int main111(int argc, char *argv[])
{

	// Base::LocalMassage::readConfig("/root/code/web_server/web_server/config.conf");
	// Log::init();

	// std::thread th(timer_work);

	// tim.addOnceTask(frint_time, 1000);

	// th.join();

	return 0;
}