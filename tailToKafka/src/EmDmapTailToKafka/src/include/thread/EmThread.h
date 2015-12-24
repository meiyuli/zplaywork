#ifndef EMTHREAD_H
#define EMTHREAD_H
#include <stdio.h>
#include <sched.h>
#include <pthread.h>

class EmThread
{
public:
	EmThread(int threadId);
	virtual ~EmThread();
	virtual int run();	
	
	virtual void pthreadStart();

	virtual void pthreadStop();
public:
	pthread_t m_pthreadId;
	int m_threadId;
	pthread_attr_t m_attr;
};

void* threadFunc(void* p);

#endif

