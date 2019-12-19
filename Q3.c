#include <stdio.h>
#include <pthread.h>

#define MAX 5

int buffer[MAX]; //생성한 값을 넣는 버퍼
int fill_ptr=0;
int use_ptr=0;
int count=0;
int loops=1000;

int pthread_create(	pthread_t * thread, //쓰레드 생성

		const pthread_attr_t * attr,

			void * (*start_routine)(void*),

			void * arg);

 

void put(int value){ //생산자가 값을 넣을 때 사용

	buffer[fill_ptr] = value;

	fill_ptr=(fill_ptr+1)%MAX;

	count++;

}

 

int get(){ //소비자가 값을 얻을 때 사용

	int tmp=buffer[use_ptr];

	use_ptr=(use_ptr+1)%MAX;

	count--;

	return tmp;

}

 

pthread_cond_t empty, fill;
pthread_mutex_t mutex; 

void *producer(void *arg){

	int i;

	for(i=0; i<loops; i++){

		pthread_mutex_lock(&mutex);
		while(count ==MAX)

			pthread_cond_wait(&empty,&mutex);

		put(i);
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&mutex);
		printf("producer data : %d\n",i);
	}

}

 

void *consumer(void *arg){

	int i;

	for(i=0;i<loops;i++){

		pthread_mutex_lock(&mutex);

		while(count==0)
			pthread_cond_wait(&fill,&mutex);

		int tmp = get();
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);

		printf("\tconsumer data : %d\n",tmp);

	}

}

 

int main(int argc, char *argv[]){

	pthread_t p1,p2;

	pthread_create(&p1,NULL,producer,"producer"); //생산자 쓰레드 생성

	pthread_create(&p2,NULL,consumer,"consumer"); //소비자 쓰레드 생성

	pthread_join(p1, NULL);
	pthread_join(p2, NULL); 

	return 0;

}
