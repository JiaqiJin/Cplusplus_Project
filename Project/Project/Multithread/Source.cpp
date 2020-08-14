#include "Helper.h"

/*
std::async、std::future -> 创建后台任务并返回值。
启动一个异步任务，就是自动创建一个线程并且开始执行对应的线程入口函数。
std::future = 会等待线程执行结束并返回结果。
lunch::deferred：表示线程入口函数调用会被延迟，一直到future的wait或者get函数被调用时。并没有创建新线程，是在主线程中调用的线程入口函数。
std::packaged_task 打包任务，把各种调用对象包围起来。
*/
class A
{
public:
	A() = default;
	int mythread(int num)
	{
		std::cout << " enter number is " << num << endl;
		cout << "mythread start " << "thread id = " << this_thread::get_id() << endl;
		std::chrono::microseconds timer(500000);
		std::this_thread::sleep_for(timer);
		cout << "mythread end " << "thread id = " << this_thread::get_id() << endl;

		return 5;
	}
};


int mythreadA(int num)
{
	std::cout << " enter number is " << num << endl;
	cout << "mythread start " << "thread id = " << this_thread::get_id() << endl;
	std::chrono::microseconds timer(500000);
	std::this_thread::sleep_for(timer);
	cout << "mythread end " << "thread id = " << this_thread::get_id() << endl;

	return 5;
}

int main()
{
	A a;
	int temper = 10;
	cout << "Kawaii desune " << " thread id =  "<< this_thread::get_id() << endl;
	//std::future<int> result = std::async(std::launch::async, &A::mythread, &a, temper);//
	//std::future<int> result = std::async(std::launch::deferred, &A::mythread, &a, temper);//主线程调用函数
	//std::future<int> result = std::async(&A::mythread,&a, temper);
	cout << "continue..." << endl;
	//cout << result.get() << endl; //卡在这里等待线程执行完毕(mythread function)。
	cout << "Kawaii desune " << endl;

	//std::packaged_task<int(int)> mypack(mythreadA);

	std::packaged_task<int(int)> mypack([](int num)
	{
		std::cout << " enter number is " << num << endl;
		cout << "mythread start " << "thread id = " << this_thread::get_id() << endl;
		std::chrono::microseconds timer(500000);
		std::this_thread::sleep_for(timer);
		cout << "mythread end " << "thread id = " << this_thread::get_id() << endl;
		return 5;
	});

	std::thread t1(std::ref(mypack), 1);
	t1.join();
	std::future<int> result = mypack.get_future();
	cout << result.get() << endl;

	return 0;
}