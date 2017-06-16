#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <thread>

class buffer
{
	public:
		void setVal(int x);
		int getVal();

	private:
		int val;
}
void buffer::setVal(int x){val=x;}
int buffer::getVal(){return val;}


#endif