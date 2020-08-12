#include "Singleton.h"

//Singleton::ptr Singleton::m_instance = nullptr;
Singelton2* Singelton2::instance = NULL;

/*class A : public MySingleton<A>
{
public:
	typedef std::shared_ptr<A> ptr;
	A() = default;
	~A() = default;

	static A::ptr getSingleton();
	void printer()
	{
		std::cout << "HiHi A" << std::endl;
	}
};

template<> std::shared_ptr<A> MySingleton<A>::m_instance = nullptr;
A::ptr A::getSingleton()
{
	if (m_instance == nullptr)
	{
		m_instance = std::make_shared<A>();
		static GCollerctor g;
	}
	return m_instance;
}
*/
void creatingObj()
{
	std::cout << "Object Creating" << std::endl;
	/*Singleton::ptr p1 = Singleton::getSingleton();
	Singleton::ptr p2 = Singleton::getSingleton();

	p1->test();
	p2->test();
	Singleton::getSingleton()->test();*/

	//A a;
	//a.printer();

	Singelton2* p1 = Singelton2::getInstance();
	Singelton2* p2 = Singelton2::getInstance();
	p1->test();
	p2->test();
	std::cout << "Object Creating Finish" << std::endl;
}

/*
call_once 能够保证函数a()只被调用一次。具备互斥量的能力
其实once_flag是一个结构，call_once()就是通过标记来决定函数是否执行，调用成功后，就把标记设置为一种已调用状态。
*/

int main()
{
	std::thread mythread1(creatingObj);
	std::thread mythread2(creatingObj);
	mythread1.join();
	mythread2.join();
	return 0;
}