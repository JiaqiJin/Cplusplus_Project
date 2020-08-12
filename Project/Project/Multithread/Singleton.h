#pragma once
#include<iostream>
#include <memory>
#include <mutex>

//static std::mutex my_mutex;
//class Singleton
//{
//public:
//	typedef std::shared_ptr<Singleton> ptr;
//	Singleton() = default;
//	static Singleton::ptr getSingleton()
//	{
//		if (m_instance == NULL)
//		{
//			//std::unique_lock<std::mutex> mutexhelper(my_mutex);
//			if (m_instance == NULL)
//			{
//				m_instance = std::make_shared<Singleton>();
//				static Garbage g;
//			}
//		}
//		return m_instance;
//	}
//
//	void test()
//	{
//		std::cout << "Hi " << std::endl;
//	}
//
//	class Garbage
//	{
//	public:
//		~Garbage()
//		{
//			if (Singleton::m_instance)
//			{
//				delete &Singleton::m_instance;
//				Singleton::m_instance = NULL;
//			}
//		}
//	};
//
//private:
//	static std::shared_ptr<Singleton> m_instance;
//};
//
//template<typename T>
//class MySingleton
//{
//private:
//	MySingleton(const MySingleton<T>&) {}
//	MySingleton &operator=(const MySingleton<T>&){}
//protected:
//	static std::shared_ptr<T> m_instance;
//
//public:
//	MySingleton() = default;
//	~MySingleton() = default;
//
//	class GCollerctor
//	{
//	public:
//		~GCollerctor()
//		{
//			if (MySingleton::m_instance)
//			{
//				MySingleton::m_instance = nullptr;
//			}
//		}
//	};
//
//	static std::shared_ptr<T> getSingleton()
//	{
//		if (m_instance == nullptr)
//		{
//			m_instance = std::make_shared<T>();
//			static GCollerctor g;
//		}
//			
//		return m_instance;
//	}
//
//};

static std::mutex my_mutex;
std::once_flag g_glag;

class Singelton2
{
	static void CreateInstance() //只被调用一次。
	{
		std::chrono::milliseconds dura(20000);
		std::this_thread::sleep_for(dura);
		instance = new Singelton2();
		static CGarhuishou g;
	}

public:

	static Singelton2* getInstance() {
		/*if (instance == NULL)
		{
			std::unique_lock<std::mutex> mymutex(my_mutex);
			if (instance == NULL) {
				static CGarhuishou huishou;
				instance = new Singelton2;
			}
		}*/
		std::call_once(g_glag, CreateInstance);//俩个线程同时进行到这里，另一个线程要等CreateInstance执行完毕。
		std::cout << "call once() finish" << std::endl;
		return instance;
	}
	void test()
	{
		std::cout << "HI" << std::endl;
	}

	class CGarhuishou {
	public:
		~CGarhuishou()
		{
			if (Singelton2::instance)
			{
				delete Singelton2::instance;
				Singelton2::instance = NULL;
			}
		}
	};
private:
	Singelton2() {}
	static Singelton2* instance;
};

