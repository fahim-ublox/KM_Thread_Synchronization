#include "mbed.h"
#include "rtos.h"


DigitalOut led1(LED1);

Semaphore sem(1);
Mutex mutex;

int gA=1;
int gB;
const int C = 10;

static void myHeap_Address()
{
	//pc.printf("address gA: %d\n", &gA);
 	//pc.printf("address gB: %d\n", &gB);
 	//pc.printf("address constant C: %d\n", &C);
}

/* Semaphore is added in this function but released in 2nd thread function to check semaphore working or not.
 * "Th_print_1' Function execution will be halted until 2nd thread function releases semaphore.
 */
void Th_print_1(void) {
	int32_t value=0;

	value = sem.wait();
    printf("\nSemaphore wait: %d\n\r",value);

    printf("\nSemaphore Working\n\r");
    //sem.release();
}

/* Mutex is locked in this function and No other thread can access this function until mutex is unlocked.
 * and Semaphore is also released here.
 */
void Th_print_2(void) {

	mutex.lock();

    printf("\nMutex Working\n\r");

    sem.release();

    mutex.unlock();
}

void test_thread_1(void const *args) {

	while(true){
		Th_print_1();
		Thread::wait(30);
	}
}

void test_thread_2(void const *args) {

	while(true){
		Th_print_2();
		Thread::wait(100);
	}
}


int main (void) {
    Thread t1(osPriorityHigh);
    Thread t2(osPriorityAboveNormal);

    t1.start(callback(test_thread_1, (void *)"Semaphore Thread"));
    t2.start(callback(test_thread_2, (void *)"Mutex Thread"));


    while (true) {
    	myHeap_Address();
    }
}

