#include "EmThread.h"

void* threadFunc(void* p)
{
   int ret = ((EmThread*)(p))->run();
   return (void*)ret;
}

EmThread::EmThread(int threadId)
{
	m_threadId = threadId;
}

EmThread::~EmThread()
{
}

int
EmThread::run()
{    
    return 0;
}

void EmThread::pthreadStart()
{
    pthread_attr_init(&m_attr);
    pthread_attr_setscope(&m_attr, PTHREAD_SCOPE_SYSTEM);
    pthread_create(&m_pthreadId, &m_attr, threadFunc, (void*)this);
}

void EmThread::pthreadStop()
{
	pthread_cancel(m_pthreadId);
}

