#ifndef Q
#define Q

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

	queue* createQueue(size_t allocSize);
	queue* enqueue(queue* q, void* data);
	queue* dequeue(queue* q, void* toRet);
	queue* front(queue* q, void* toRet);
	size_t getSize(queue* q);
	bool isEmpty(queue* q);

	#endif