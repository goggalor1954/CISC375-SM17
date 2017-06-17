//producer and consumer project

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <pthread.h>
//#include <studio.h>


class buffer //contains the value of the int being passed. i mignt not really need this i could just create an array.
{
	public:
		void setVal(int x);
		int getVal();

	private:
		int val;
};
void buffer::setVal(int x){val=x;}
int buffer::getVal(){return val;}

struct bufferArgs
{
	int *buffer;
	int counter, counterLimit, in, out ,bufferSize, bufferLocation;
	pthread_mutex_t *mutexVar;
	pthread_cond_t *empty, *full;
	bufferArgs(){counter=0;}
};
		
/*
void *consumer( void *arg)
{
	bufferArgs* myBuffer=(bufferArgs*) arg;
	while(myBuffer->counter < myBuffer->counterLimit)
	{
		while(myBuffer->in==myBuffer->out){}; // do nothing
		std::cout << "Consumer: " << myBuffer->buffer[myBuffer->out] << std::endl;//prints number in buffer
		myBuffer->buffer[myBuffer->out]=0;//removes current value in buffer by seting it to zero.
		myBuffer->out=(myBuffer->out+1)%myBuffer->bufferSize; // increment buffer location up to n-1
		myBuffer->counter++; //increment buffer count

	}
}
void *producer(void *arg)
{
	bufferArgs* myBuffer=(bufferArgs*) arg;
	while(myBuffer->counter < myBuffer->counterLimit)
	{
		while (((myBuffer->in+1)%myBuffer->bufferSize)==myBuffer->out){};//do nothing, wait for buffer space
		myBuffer->buffer[myBuffer->in]=rand();//asign random number to buffer
		std::cout <<"Producer: "<< myBuffer->buffer[myBuffer->in] << std::endl;//printout same random number
		myBuffer->in=(myBuffer->in+1)%myBuffer->bufferSize;//increment buffer location up to n-1
		myBuffer->counter++; //increment counter

	}

};
*/


void *consumer( void *arg)
{

	bufferArgs* myBuffer=(bufferArgs*) arg;
	while(myBuffer->counter < myBuffer->counterLimit)
	{
		for(int i=0; i<myBuffer->bufferSize; i++)
		{
			while(i==myBuffer->bufferSize)
			{
				pthread_cond_wait(myBuffer->full, myBuffer->mutexVar);
			}
			pthread_mutex_lock(myBuffer->mutexVar);
			std::cout << "Consumer: " << myBuffer->buffer[i] << std::endl;//prints number in buffer
			myBuffer->buffer[i]=0;//removes current value in buffer by seting it to zero.
			myBuffer->counter++;
			pthread_mutex_unlock(myBuffer->mutexVar);
			pthread_cond_signal(myBuffer->empty);

		}
	}
}
void *producer(void *arg)
{
	bufferArgs* myBuffer=(bufferArgs*) arg;

	while(myBuffer->counter < myBuffer->counterLimit)
	{
		for(int i=0; i < myBuffer->bufferSize; i++){
			while(i==myBuffer->bufferSize)
			{
				pthread_cond_wait(myBuffer->empty, myBuffer->mutexVar);
			}
			pthread_mutex_lock(myBuffer->mutexVar);
			myBuffer->buffer[i]=rand();
			std::cout <<"Producer: "<< myBuffer->buffer[i] << std::endl;//printout same random number
			myBuffer->counter++;
			pthread_mutex_unlock(myBuffer->mutexVar);
			pthread_cond_signal(myBuffer->full);

		}

	}
};

//i think the problem im having is thiat im not iteraiting through the array corectly, something about int size. don't knoe. too tired




int main()
{
	int bufferSize, countLimit;
	srand(time(0));
	pthread_mutex_t threadLock;

	std::cin >> bufferSize;
	std::cin >> countLimit;
	try //if the enter a number <= 0, catch the error , send an cerr, and end the program.
	{
		if(bufferSize<1 || countLimit<1){throw 1;}
	}
	catch(int err_code)
	{
		std::cerr << "You have entered a non-positive value.";
		return 0;
	}
	int *buff1 = new int[bufferSize];
	bufferArgs *sharedBuffer = new bufferArgs;
	sharedBuffer->buffer=buff1;
	sharedBuffer->counterLimit=countLimit;
	sharedBuffer->bufferSize=bufferSize;

	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, producer,(void *) sharedBuffer);
	pthread_create(&thread2, NULL, consumer,(void *) sharedBuffer);

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);

//testing

	//delete [] Buff1;
	return 0;
}
