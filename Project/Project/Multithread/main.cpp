#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include<thread>

using namespace std;

class Tester {
public:
	int number;
	void operator ()() { 
		cout << "Mythread number " << number << endl; 
		cout << "Mythread number 1 " << number << endl;
		cout << "Mythread number 2 " << number << endl;
		cout << "Mythread number 3 " << number << endl;
	};

	Tester(int num) : number(num) { cout << "My tester creation "  << endl; }
	Tester(const Tester & tester) : number(tester.number) { cout << "My COPY tester creation !" << endl; }

	~Tester(){ cout << "My  tester creation Destruction ! " << endl; }

	void sum(int numb) { this->number += numb; }
	int getNumber() { return number;}
};

Tester test(10);

void myprint()
{
	cout << " my thread start " << endl;
	/*
	code
	*/
	test.sum(100);
	cout << " my thread end " << endl;
	
}
/*
Notes : 多线程都需要独立空间，切换需要消耗本该于程序运行的时间。
单个进程中，创建多个线程， 每一个线程中的所有线程共享地址空间。
主线程保持一直运行才能保持子线程运行。
*/
int main()
{

	//thread myjob(myprint);

	//thread myjob2(test);

	//myjob.join();
	//myjob2.join();
	
	
	//myjob.join(); //主线程阻塞这里等待 子线程 执行完毕， 主线程就继续往下执行。
	//myjob2.detach(); // 主线程和子线程失去关联，子线程驻留在后台运行。

	//lambda 
	auto mylambthread = [] {
		cout << " my thread start " << endl;
		/*
		code
		*/
		test.sum(100);
		cout << " my thread end " << endl;
	};

	cout << "Hello kawaii" << endl;
	thread myjob3(mylambthread);
	myjob3.join();

	//cout << test.getNumber() << endl;

	return 0;
}