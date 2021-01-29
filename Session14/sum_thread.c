#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void *work_thread(void * arg); 

int num_threads;
int max_num; 

int main(int argc, char *argv[])
{
	pthread_t *thread;
	int * index; 
	int sum = 0, i;

	printf("Number of threads: ");
	scanf("%d", &num_threads);
	printf("Maximum integer (> Number of threads):");
	scanf("%d", &max_num);

	// TODO: pthread & index array
	thread = (pthread_t*) malloc(sizeof(pthread_t) * num_threads);
	index = (int*) malloc(sizeof(int) * 3);

	// TODO: pthread create	
	for (i = 0; i < num_threads; i++)
	{
		index[0] = i * (max_num / num_threads);
		index[1] = (i + 1) * (max_num / num_threads);
		index[2] = 0;

		if(index[1] > max_num) index[1] = max_num;
		if(i == num_threads-1) index[2] = 1;
		//printf("idx[0] : %d, idx[1] : %d\n", index[0], index[1]);
		
		if(pthread_create(&thread[i], NULL, work_thread,(void*)index) != 0)
		{
			puts("pthread_create() error");
			return -1;
		}
		sleep(3);
	}

	// TODO: pthread_join
	for (i = 0; i < num_threads; i++)
	{
		void * thr_ret;
		if(pthread_join(thread[i], &thr_ret) != 0)
		{
			puts("pthread_join() error");
			return -1;
		}
		
		sum += *((int*)thr_ret);
		free(thr_ret);
	}
	
	
	printf("result: %d \n", sum);
	return 0;
}

void* work_thread(void *arg) 
{
	/// 
	int start = *((int*)arg);
	int end = *((int*)arg + 1);
	int * s = (int *)malloc(sizeof(int));
	
	int last_flag = *((int*)arg+2);

	printf("In function, start = %d  end = %d\n", start, end);
	
	s[0] = 0;
	while(start < end)
	{
		*s += start;
		start++;
	}
	//if(start == max_num) *s += start;
	
	if(last_flag) {
		while(start <= max_num){
			*s += start;
			start++;
		}
	}
	return (void *)s;
}
