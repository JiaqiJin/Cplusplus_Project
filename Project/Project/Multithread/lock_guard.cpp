#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <thread>
#include <mutex>
using namespace std;

/*
Mutex
lock unlock
lock_guard template = lock_guard 构造函数里执行了mutex::lock(); 析造函数里执行了 mutex::unlock lock_guard 不灵活
死锁deadlock = 是俩个锁头也就是俩个互斥量(mutex)才能产生,俩个线程在争夺资源。
保证 2 mutex 上锁顺序一致就不会死锁。
mutex1 lock , mutex2 lock
mutex1 unlock , mutex2 unlock
std::lock_guard<mutex> sbguard(my_mutex,std::adopt_lock); //自动unlock, 
不需要在std::lock_guard<mutex>对象再次lock
*/

void myprinter(int num)
{
	//cout << "my printer thread init, thread number = " << num << endl;
	//// codes
	//cout << "my printer thread finish, thread number = " << num << endl;
	cout << "id is" << std::this_thread::get_id() << "thread vector number is " << endl;
	return;
}

class A
{
public:
	 A() = default;
	~A() = default;
	//收到的消息入到一个列队的线程。
	void inMsgReceive()
	{
		for (int i = 0; i < 10000; i++)
		{
			cout << "inMsgReceive, inserting element" << i << endl;
			{
				std::lock(my_mutex, my_mutex2);

				std::lock_guard<mutex> sbguard(my_mutex,std::adopt_lock); //自动unlock
				std::lock_guard<mutex> sbguard2(my_mutex2, std::adopt_lock);

				//my_mutex.lock();
				myList.push_back(i);
				//my_mutex.unlock();
				//my_mutex2.unlock();
			}
			//my_mutex.unlock();
			//.... 执行其他代码
		}	
		return;
	}

	bool outMsgLULProc(int& command)
	{
		std::lock_guard<mutex> sbguard(my_mutex);
		std::lock_guard<mutex> sbguard2(my_mutex2);
		//my_mutex.lock();
		if (!myList.empty())
		{
			command = myList.front();
			myList.pop_front();
			//myvector.push_back(command);
			//my_mutex.unlock();
			return true;
		}
		//my_mutex.unlock();

		return false;
	}

	//把数据从列队中取出的线程
	void outMsgReceive()
	{
		int command = 0;
		for (int i = 0; i < 10000; i++)
		{
			bool result = outMsgLULProc(command);
			if(result)
			{
				cout << "outMsgReceive, deleting element" << command << endl;
			}
			else
			{
				cout << "outMsgReceive, empty element" << i << endl;
			}
		}
	}

private:
	//you can use vector both xd
	std::list<int> myList;
	std::mutex my_mutex;
	std::mutex my_mutex2;
	std::vector<int> myvector;
};


/*
Notes :
List : elements can be inserted and removed just by modifying a few pointers, so that can be quite fast.(contiguo memory)
Vector use for random element insertion, but deleting random element in vector mid is slow(randow memory)
*/
int main()
{
	/*vector<thread> myhtreads;
	for (int i = 0; i < 10; i++)
	{
		myhtreads.push_back(thread(myprinter,i));
	}

	for (auto iter = myhtreads.begin(); iter != myhtreads.end() ; iter++)
		iter->join();*/

	A myobj;
	std::thread myOutThread(&A::outMsgReceive, std::ref(myobj));
	std::thread myInThread(&A::inMsgReceive, std::ref(myobj));
	myInThread.join();
	myOutThread.join();

	//mutex 保护共享数据，把共享数据锁住，其他想操控共享数据的线程必须等待解锁。
	// 先 lock 操作共享数据,unlock -> 每调用一次lock， 必然调用一次unlock。你忘记unlock lock_guard 替你unlock


	cout << "hello kawaii" << endl;

	

	return 0;
}