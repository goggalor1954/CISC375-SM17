//producer and consumer project

#include <iostream>
#include <cmath>
#include <pthread.h>
//#include <studio.h>


class buffer //
{
	public:
		void setVal(int x);
		int getVal();

	private:
		int val;
};
void buffer::setVal(int x){val=x;}
int buffer::getVal(){return val;}

/*
void *producer(buffer currentBuffer[], int &in, int &out, int &countLim, int bufferSize)//do i pass these by refrence? i think yes
{
	while(true)//figure out what to put here
	{
		while (((in+1)%bufferSize)==out){};//do nothing, wait for buffer space
		currentBuffer[in].setVal(rand());
		std::cout <<"Producer: "<< currentBuffer[in].getVal() << std::endl;
		in=(in+1)%bufferSize;
		countLim++;
	}

}
void *consumer( buffer currentBuffer[], int &in, int &out, int &countLim, int bufferSize)
{
	while(true)
	{
		while(in==out){};
		std::cout << "consumer: " << currentBuffer[out].getVal() << std::endl;
		currentBuffer[out].setVal(0);
		out=(out+1)%bufferSize;
		countLim++;
	}
}*/
void *producer(void *ptr)
{
	while(true)//figure out what to put here
	{
		while (((in+1)%buffSize)==out){};//do nothing, wait for buffer space
		Buff1[in].setVal(rand());
		std::cout <<"Producer: "<< Buff1[in].getVal() << std::endl;
		in=(in+1)%bufferSize;
		countLim++;
	}

};








int main(int argc, char, *argv[])
{
	int buffSize, countLim, in=0, out=0;
	srand(time(0));

	std::cin >> buffSize;
	std::cin >> countLim;
	try //if the enter a number <= 0, catch the error , send an cerr, and end the program.
	{
		if(buffSize<1 || countLim<1){throw 1;}
	}
	catch(int err_code)
	{
		std::cerr << "You have entered a non-positive value.";
		return 0;
	}
	buffer *Buff1 = new buffer[buffSize];

	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, producer,(void) )


	delete [] Buff1;
	return 0;
}