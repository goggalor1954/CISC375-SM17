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
		for(myBuffer->out=0; myBuffer->out<=myBuffer->bufferSize; myBuffer->out++)
		{
std::cout << "got here 2" << std::endl;
		pthread_mutex_lock(&mutexVar);
			while(myBuffer->out==myBuffer->bufferSize)
			{
std::cout << "got here 3" << std::endl;

				pthread_cond_wait(&full, &mutexVar);
			}
std::cout << "got here 4" << std::endl;
			std::cout << "Consumer: " << myBuffer->buffer[myBuffer->out] << std::endl;//prints number in buffer
			myBuffer->buffer[myBuffer->out]=0;//removes current value in buffer by seting it to zero.
			myBuffer->counterOut++;

			pthread_cond_signal(&empty);
			pthread_mutex_unlock(&mutexVar);
		}

	}
	pthread_exit(NULL);
}
void *producer(void *arg)
{
	bufferArgs* myBuffer=(bufferArgs*) arg;
	while(myBuffer->counter<myBuffer->counterLimit)// while counter is lsee than counter limit
	{
		for(myBuffer->in=0; myBuffer->in<=myBuffer->bufferSize; myBuffer->in++)//loop through the buffer
		{
			pthread_mutex_lock(&mutexVar); //lock the mutex
			while(myBuffer->in==myBuffer->bufferSize) //if in equals buffer size, wait for the empty signal
			{
				pthread_cond_wait(&empty, &mutexVar);
			}
			myBuffer->buffer[myBuffer->in]=rand(); //insert random number into buffer
			std::cout <<"Producer: "<< myBuffer->buffer[myBuffer->in] << std::endl;//printout same random number
			myBuffer->counter++; //increment counter

			pthread_cond_signal(&full); //signal buffer is full(should i be doing that here?)
			pthread_mutex_unlock(&mutexVar); //unlock mutex
		}


	}
	pthread_exit(NULL);
};

//got ti partially to work. but its not stoping bhen the buffer is Full.



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
