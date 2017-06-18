//producer and consumer project

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <pthread.h>



pthread_mutex_t mutexVar = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t isNotEmpty=PTHREAD_COND_INITIALIZER;
pthread_cond_t isNotFull=PTHREAD_COND_INITIALIZER;

struct bufferArgs
{
	int *buffer;
	int counter, counterOut, counterLimit,bufferSize, in, out;
	bufferArgs(){counter=0, counterOut=0, in=0, out=0;}
};

void *consumer( void *arg)
{
	bufferArgs* myBuffer=(bufferArgs*) arg;
	while(myBuffer->counterOut<=myBuffer->counterLimit)
	{
		while(myBuffer->out <myBuffer->bufferSize)
		{
			pthread_mutex_lock(&mutexVar);
std::cout << "in:" << myBuffer->in << " out:" <<myBuffer->out << "\n";
			while(myBuffer->out==myBuffer->in)
			{
std::cout <<"waiting\n";
				pthread_cond_wait(&isNotEmpty, &mutexVar);
std::cout<<"done\n";
			//myBuffer->out=0;
			}
			myBuffer->counterOut++;
			myBuffer->out++;
			std::cout << "Consumer: " << myBuffer->buffer[myBuffer->out] << std::endl;//prints number in buffer

			pthread_mutex_unlock(&mutexVar);
			pthread_cond_signal(&isNotFull);
		}
		myBuffer->out=0;

	}
	pthread_exit(NULL);
}
void *producer(void *arg)
{
	pthread_mutex_unlock(&mutexVar);
	pthread_cond_signal(&isNotFull);

	bufferArgs* myBuffer=(bufferArgs*) arg;
	while(myBuffer->counter<myBuffer->counterLimit)// while counter is lsee than counter limit
	{
		while(myBuffer->i<myBuffer->bufferSize)
		{
			pthread_mutex_lock(&mutexVar); //lock the mutex
			while(myBuffer->in == myBuffer->bufferSize ) //if in equals buffer size, wait for the isNotFull signal
			{
				pthread_cond_wait(&isNotFull, &mutexVar);
				//myBuffer->in=0;
			}
			myBuffer->buffer[myBuffer->in]=rand(); //insert random number into buffer
			std::cout <<"Producer: "<< myBuffer->buffer[myBuffer->in] << std::endl;//printout same random number
			myBuffer->counter++; //increment counter
			myBuffer->in++;
			pthread_mutex_unlock(&mutexVar); //unlock mutex
			pthread_cond_signal(&isNotEmpty); //signal buffer is not empty(should i be doing that here?)

		}
		myBuffer->in=0;
	}
	pthread_exit(NULL);
};

//problem here is that both are looping endlessly. need to come up with a condition to check that they are the ned of the beffer without reseting the loop from inside.

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

	return 0;
}
