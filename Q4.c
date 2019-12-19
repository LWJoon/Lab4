#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
char msg[] = "test message";
int msgstat=0;
pthread_cond_t msgcond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t msgmutex=PTHREAD_MUTEX_INITIALIZER;

void *receiveMsg(void *name){
  while(1){ 
  pthread_cond_wait(&msgcond,&msgmutex);
  printf("message[%s] : %s\n",(char*)name,msg);
  }
  exit(0);
}


int main()
{
  pthread_t p_thread[2];
  int thr_id;
  int status;
  char *p1="[1]";
  char *p2="[2]";
  thr_id=pthread_create(&p_thread[0],NULL,receiveMsg,p1);
  if(thr_id < 0){
	perror("create thread err");
	exit(0);
  }
  thr_id=pthread_create(&p_thread[1],NULL,receiveMsg,p2);
  if(thr_id < 0){
	perror("create thread err");
	exit(0);
  }
   sleep(1);
  while(1){
   pthread_mutex_lock(&msgmutex); 
   pthread_mutex_unlock(&msgmutex);
   pthread_cond_broadcast(&msgcond);
   sleep(1);
  }
  pthread_join(p_thread[0],(void**)&status );
  pthread_join(p_thread[1],(void**)&status );
  printf("쓰레드 [메인] 종료");
  return 0;
}
