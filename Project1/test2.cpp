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

pthread_mutex_t mutexVar = PTHREAD_MUTEX_INITIALIZER;

struct bufferArgs
{
	int *buffer;
	int counter, counterLimit, in, out ,bufferSize, counterOut;
	//pthread_cond_t *empty, *full;
	bufferArgs(){counter=0, in=0, out=0, counterOut=0;}
};
	pthread_cond_t empty=PTHREAD_COND_INITIALIZER;
	pthread_cond_t full=PTHREAD_COND_INITIALIZER;

void *consumer( void *arg)
{
//testing
std::cout << "got here " << std::endl;
	bufferArgs* myBuffer=(bufferArgs*) arg;
	while(myBuffer->counterOut<myBuffer->counterLimit)
	{
std::cout << "got here 2" << std::endl;
		pthread_mutex_lock(&mutexVar);
			while(myBuffer->out==0)
			{
std::cout << "got here 3" << std::endl;
				pthread_cond_wait(&full, &mutexVar);
			}
std::cout << "got here 4" << std::endl;
			std::cout << "Consumer: " << myBuffer->buffer[myBuffer->out] << std::endl;//prints number in buffer
			myBuffer->buffer[myBuffer->out]=0;//removes current value in buffer by seting it to zero.
			myBuffer->counterOut++;
			myBuffer->out++;
			if(myBuffer->out==myBuffer->bufferSize){myBuffer->out=0;}
			pthread_cond_signal(&empty);
			pthread_mutex_unlock(&mutexVar);

	}
	pthread_exit(NULL);
}
void *producer(void *arg)
{
	bufferArgs* myBuffer=(bufferArgs*) arg;
	while(myBuffer->counter<myBuffer->counterLimit)
	{
		pthread_mutex_lock(&mutexVar);
			while(myBuffer->in==myBuffer->bufferSize)
			{
				pthread_cond_wait(&empty, &mutexVar);
			}
			myBuffer->buffer[myBuffer->in]=rand();
			std::cout <<"Producer: "<< myBuffer->buffer[myBuffer->in] << std::endl;//printout same random number
			myBuffer->counter++;
			myBuffer->in++;
			if(myBuffer->in==myBuffer->bufferSize){myBuffer->in=0;}
			pthread_cond_signal(&full);
			pthread_mutex_unlock(&mutexVar);


	}
	pthread_exit(NULL);
};

//i think the problem im having is thiat im not iteraiting through the array corectly, something about int size. don't knoe. too tired
//new idead. mutexes might not work between processs. maybe i should be using semaphores.



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




	//delete [] Buff1;
	return 0;
}
