#include "queue.h"


pthread_mutex_t qmutex = PTHREAD_MUTEX_INITIALIZER;

queue* safeCreate(size_t allocSize)
{
	pthread_mutex_lock(&qmutex);
	queue* q = createQueue(allocSize);
	pthread_mutex_unlock(&qmutex);
	return q;
}

queue* createQueue(size_t allocSize)
{
	queue* q = (queue*)malloc(sizeof(queue));
		
	q->allocationSize = allocSize;
	q->size = 0;
	q->head = q->tail = NULL;
	
	return q;
}

void enqueue(queue* q, void* _data)
{
	data* toInsert = (data*)malloc(sizeof(data));
	toInsert->data = malloc(q->allocationSize);

	toInsert->next = NULL;
	memcpy(toInsert->data, _data, q->allocationSize);
	if (q->size == 0)
	{ //First insertion
		q->head = q->tail = toInsert;
	}
	else
	{
		q->tail->next = toInsert;
		q->tail = toInsert;
	}

	q->size++;

	return;
}

void dequeue(queue* q, void* toRet)
{
	data* toDel = q->head;
	if (q->size == 1)
	{
		memcpy(toRet, toDel->data, q->allocationSize);
		free(toDel->data);
		free(toDel);
		q->head = q->tail = NULL;
		q->size--;
		return;
	}
	q->head = q->head->next;
	memcpy(toRet, toDel->data, q->allocationSize);
	free(toDel->data);
	free(toDel);
	q->size--;

	return;
}

void front(queue* q, void* toRet)
{
	memcpy(toRet, q->head->data, q->allocationSize);
}

bool isEmpty(queue* q)
{
	return q->size == 0 ? true : false;
}
