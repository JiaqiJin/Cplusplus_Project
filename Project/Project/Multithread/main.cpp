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
//Tester test(10);
void myprint()
{
	cout << " my thread start " << endl;
	/*
	code
	*/
	//test.sum(100);
	cout << " my thread end " << endl;
}

//void printer(const int& i, char* mybuffer) 
void printer(const int& i,const string& mybuffer)
{
	cout << i << endl;
	cout << mybuffer.c_str() << endl;
}

class A {
public:
	A(int a) : m_i(a) { cout << "[A::A(int a)constructor ]" << this << " thread id = "<< this_thread::get_id() << endl; }
	A(const A &a) : m_i(a.m_i){ cout << "[A(const A &a)constructor]" << this << 
		" thread id = " << this_thread::get_id() << endl; }
	~A() { cout << "[~A()destructor]" << this << " thread id = " << this_thread::get_id() << endl; }
	int m_i;
};


class B {
public:
	B(int a) : m_i(a) { cout << "[B::B(int b)constructor ]" << this << " thread id = " << this_thread::get_id() << endl; }
	B(const B& a) : m_i(a.m_i) {
		cout << "[B(const B &b)constructor]" << this <<
			" thread id = " << this_thread::get_id() << endl;
	}
	~B() { cout << "[~B()destructor]" << this << " thread id = " << this_thread::get_id() << endl; }

	void function_thread(int num)
	{
		cout << "[B::B function_thread ]" << this << " thread id = " << this_thread::get_id() << endl;
	}

	void operator () (int num)
	{
		cout << "[ second thread entry ]" << this << " thread id = " << this_thread::get_id() << endl;
	}

	mutable int m_i;
};


void printer2(const int& i, const A& pmbuffer)
{
	cout << &pmbuffer << endl;
}

void printerB(A& pmbuffer,B& pmbuffer2)
{
	pmbuffer2.m_i = 100;
	cout << &pmbuffer << " printer thread id A " << this_thread::get_id() << endl;
	cout << &pmbuffer2 <<" printer thread id B " <<this_thread::get_id()<<endl;
}

void printerPTR(std::unique_ptr<int>pmbuffer2)
{	
	cout << &pmbuffer2 << " printer thread id B " << this_thread::get_id() << endl;
}


/*
Notes : 多线程都需要独立空间，切换需要消耗本该于程序运行的时间。
单个进程中，创建多个线程， 每一个线程中的所有线程共享地址空间。
主线程保持一直运行才能保持子线程运行。

using const ref variable or std::ref in the job creation when passing argument.

unique_ptr parameter need std::move to passing the argument.

*/
int main()
{
	int myval = 1;
	int& myval2 = myval;
	char buffer[] = "this is a test";
	//			function , parameters
	//thread myjob(printer, myval2, buffer); //main函数都执行完了(detach)，系统才用mybuffer去转string-> error
	//thread myjob(printer, myval2, string(buffer));//static_cast<string>(buffer)

	cout << "main thread id " << std::this_thread::get_id() << endl;

	int myvalA = 12;
	//Use join !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//thread myjob(printer2, myval, A(myvalA)); //先构造A类别函数，在main函数结束之前，程序先构造临时对象A。(detach)
	//myjob.join();
	//myjob.detach();

	/*A myA(10);
	B myB(10);
	// use ref copy to change to ref
	thread myjob(printerB, std::ref(myA),std::ref(myB));
	myjob.join();*/

	/*unique_ptr<int> myp(new int(100));
	thread myjob(printerPTR,std::move(myp));
	myjob.join();*/

	B myBclass(10);
	//std::thread mythread(&B::function_thread, myBclass, 10);
	//std::thread mythread(&B::function_thread, std::ref(myBclass), 10); // &obj == std::Ref
	//void operator () (int num)
	std::thread mythread(std::ref(myBclass),10); //没有拷贝函数 B(const B& b)

	mythread.join();

	cout << "Hello kawaii" << endl;



	return 0;
}

//thread myjob(myprint);
	//thread myjob2(test);
	//myjob.join();
	//myjob2.join();
	//myjob.join(); //主线程阻塞这里等待 子线程 执行完毕， 主线程就继续往下执行。
	//myjob2.detach(); // 主线程和子线程失去关联，子线程驻留在后台运行。
	//lambda 
	/*
	auto mylambthread = [] {
		cout << " my thread start " << endl;

		code

		test.sum(100);
		cout << " my thread end " << endl;
	};

	thread myjob3(mylambthread);
	myjob3.join();
	*/