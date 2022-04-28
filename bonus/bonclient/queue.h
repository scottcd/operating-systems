#ifndef Q
#define Q

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>


typedef struct _data {
		void* data;
		struct _data* next;
	}data;

	typedef struct _queue {
		size_t size;
		size_t allocationSize;
		data* head;
		data* tail;
	}queue;

	static pthread_mutex_t qmutex;

	void initMutex();
	queue* createQueue(size_t allocSize);
	void enqueue(queue* q, void* data);
	void dequeue(queue* q, void* toRet);
	void front(queue* q, void* toRet);
	bool isEmpty(queue* q);

	#endif