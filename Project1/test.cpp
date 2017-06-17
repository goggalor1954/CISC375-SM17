#include <iostream>
#include <cmath>
#include <cstdlib>
#include <pthread.h>
using namespace std;


struct threadArgs
{
	int x;
};

void *threadTest(void *arg)
{
	threadArgs* myData=(threadArgs*) arg;

	for( int i=0; i<myData->x; i++)
	{
		std::cout << i << std::endl;
	}
	pthread_exit(NULL);
}


int main()
{
	srand(time(0));
	threadArgs *ta1 = new threadArgs;
	std::cin >> ta1->x;

	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, threadTest, (void *) ta1 ); //creates the thread
	pthread_create(&thread2, NULL, threadTest, (void *) ta1 );

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);



	return 0;
}


/* what i need to do
create an array of ints buffer
pass the buffer array, and counterstarting at 0 to the struct.
pass that struct to the thread
do whatever the function in the thread says to do eith the array
exit the thread.
*/
