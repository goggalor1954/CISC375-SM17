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
pthread_cond_t isNotEmpty=PTHREAD_COND_INITIALIZER;
pthread_cond_t isNotFull=PTHREAD_COND_INITIALIZER;

void *consumer( void *arg)
{
	bufferArgs* myBuffer=(bufferArgs*) arg;
	while(myBuffer->counterOut<myBuffer->counterLimit)
	{
		for(int out=0; out<=myBuffer->bufferSize; out++)
		{
		pthread_mutex_lock(&mutexVar);
			while(out==myBuffer->bufferSize )
			{
std::cout << "waiting in consumer" << std::endl;
				pthread_cond_wait(&isNotEmpty, &mutexVar);
				out=0;
std::cout << "done waiting in consumer "<< std::endl;
			}


			std::cout << "Consumer: " << myBuffer->buffer[out] << std::endl;//prints number in buffer
			pthread_mutex_unlock(&mutexVar);
			pthread_cond_signal(&isNotFull);
		}

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
		for(int in=0; in<=myBuffer->bufferSize; in++)//loop through the buffer
		{
			pthread_mutex_lock(&mutexVar); //lock the mutex
			while(in==myBuffer->bufferSize ) //if in equals buffer size, wait for the isNotFull signal // i think i need to add more here. its getting stuck because it needs to be dependant on something more than size
			{
std::cout <<"waiting in prodeucer\n";
				pthread_cond_wait(&isNotFull, &mutexVar);
				in=0;

std::cout <<"done waiting in prodeucer\n";
			}
std::cout<<in << "\n";
			myBuffer->buffer[in]=rand(); //insert random number into buffer
			std::cout <<"Producer: "<< myBuffer->buffer[in] << std::endl;//printout same random number
			myBuffer->counter++; //increment counter
			pthread_mutex_unlock(&mutexVar); //unlock mutex
			pthread_cond_signal(&isNotEmpty); //signal buffer is not empty(should i be doing that here?)

		}


	}
	pthread_exit(NULL);
};

//got ti partially to work. but its not stoping bhen the buffer is Full.
//maybe i shouldnt be unlocking the mutex at all? just the signal flags?


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
