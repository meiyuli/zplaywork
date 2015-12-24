/*
 * commandmanager.cpp
 *
 *  Created on: 2014-7-5
 *      Author: YL.M
 */
#include <stdio.h>
#include "commandmanager.h"

CCommandMgr* CCommandMgr::m_pCmdMgr = NULL;

CCommandMgr::CCommandMgr(void)
{
    sem_init( &m_hPutSemaphore, 0, MAX_COMMAND_COUNT );
    sem_init( &m_hGetSemaphore, 0, 0 );
    pthread_mutex_init(&m_hListMutex,NULL);
}

CCommandMgr::~CCommandMgr(void)
{
}

CCommandMgr* CCommandMgr::GetInstance()
{
    if (NULL == m_pCmdMgr)
    {
         m_pCmdMgr = new CCommandMgr();
    }
    return m_pCmdMgr;
}

int CCommandMgr::AddOperator(Command *cmd)
{
    sem_wait( &m_hPutSemaphore );

    pthread_mutex_lock( &m_hListMutex );
	m_lstCmd.push_front(cmd);  ///< 可以对命令进行处理，判断是否有重复命令，如果有，则删除重复命令
	//printf("[command] command list size:%d \n", m_lstCmd.size());
//	fprintf(stdout,"[command] command list size:%d \n", m_lstCmd.size());
	pthread_mutex_unlock( &m_hListMutex );

    sem_post( &m_hGetSemaphore );  ///< 在生产者线程中，消费者数据单元+1
    return 0;
}

Command* CCommandMgr::GetOperatorToExcute()
{
    if (sem_wait( &m_hGetSemaphore )) {
    	return NULL;
    }

    pthread_mutex_lock( &m_hListMutex );

	Command* pCmd = m_lstCmd.back();
	m_lstCmd.pop_back();
	pthread_mutex_unlock( &m_hListMutex );

	sem_post( &m_hPutSemaphore );
    return pCmd;
}

void CCommandMgr::RemoveAllCommand()
{
	pthread_mutex_lock( &m_hListMutex );

    std::list<Command*>::iterator it;
    it = m_lstCmd.begin();
    while (it != m_lstCmd.end())                       ///< 删除所有命令，同时对应的所有生产与消费者也必须同步更新
    {
         sem_wait( &m_hGetSemaphore );
         (*it)->Remove();
         sem_post( &m_hPutSemaphore );

         it ++;
    }

    m_lstCmd.clear();
    pthread_mutex_unlock( &m_hListMutex );
}

void CCommandMgr::Release()
{
    RemoveAllCommand();

    sem_destroy( &m_hPutSemaphore );
    sem_destroy( &m_hGetSemaphore );

    pthread_mutex_destroy( &m_hListMutex );

    delete m_pCmdMgr;
    m_pCmdMgr = NULL;
}
