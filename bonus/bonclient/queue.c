#include "queue.h"


void initMutex()
{
	int i = pthread_mutex_init(&qmutex, NULL);
}

queue* safeCreate(size_t allocSize)
{
	int ret = pthread_mutex_trylock(&qmutex);
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
	int ret = pthread_mutex_trylock(&qmutex);
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
	pthread_mutex_unlock(&qmutex);
	return;
}

void dequeue(queue* q, void* toRet)
{
	int ret = pthread_mutex_trylock(&qmutex);
	data* toDel = q->head;
	if (q->size == 1)
	{
		memcpy(toRet, toDel->data, q->allocationSize);
		free(toDel->data);
		free(toDel);
		q->head = q->tail = NULL;
		q->size--;
		pthread_mutex_unlock(&qmutex);
		return;
	}
	q->head = q->head->next;
	memcpy(toRet, toDel->data, q->allocationSize);
	free(toDel->data);
	free(toDel);
	q->size--;
	pthread_mutex_unlock(&qmutex);
	return;
}

void front(queue* q, void* toRet)
{
	int ret = pthread_mutex_trylock(&qmutex);
	memcpy(toRet, q->head->data, q->allocationSize);
	pthread_mutex_unlock(&qmutex);
}

bool isEmpty(queue* q)
{
	int ret = pthread_mutex_trylock(&qmutex);
	if(q->size == 0)
	{
		pthread_mutex_unlock(&qmutex);
		return true;
	}
	pthread_mutex_unlock(&qmutex);
	return false;
	
}
