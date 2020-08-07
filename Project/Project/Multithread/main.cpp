#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include<thread>

using namespace std;

void myprint()
{
	cout << "my thread start " << endl;
	/*
	code
	*/
	cout << "my thread end" << endl;
}

int main()
{
	thread myjob(myprint);
	myjob.join();

	cout << "Hello kawaii" << endl;

	return 0;
}