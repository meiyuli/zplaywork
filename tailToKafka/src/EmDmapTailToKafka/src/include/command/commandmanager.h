/*
 * commandmanager.h
 *
 *  Created on: 2014-7-5
 *      Author: YL.M
 */

#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include "command.h"
//#include "tmutex.h"
#include <semaphore.h>
#include <list>
const int MAX_COMMAND_COUNT = 1024;                     ///< 最大存储的命令个数

//typedef ThreadMutex MUTEX_TYPE; ///< 使用线程互斥量的互斥量类型
//typedef NullMutex MUTEX_TYPE; ///< 不使用线程互斥量的互斥量类型

using namespace std;
///
/// @brief
///     命令管理器类
///
class CCommandMgr {
public:
        CCommandMgr(void);
        CCommandMgr(const CCommandMgr &rhs);
        CCommandMgr& operator=(const CCommandMgr &rhs);
public:
        virtual ~CCommandMgr(void);

        ///
        /// @brief
        ///     获取命令管理对象指针
        ///
        static CCommandMgr *GetInstance();
        ///
        /// @brief
        ///     添加操作命令对象
        ///
        virtual int AddOperator(Command *cmd);
        ///
        /// @brief
        ///     获取操作命令对象
        ///
        virtual Command* GetOperatorToExcute();
        ///
        /// @brief
        ///    删除所有命令对象
        ///
        virtual void RemoveAllCommand();
        ///
        /// @brief
        ///    清理资源
        ///
        void Release();
private:
        pthread_mutex_t     m_hListMutex;           ///< 队列锁，每次只能有一个线程访问队列
        sem_t               m_hPutSemaphore;        ///< 生产者信号量
        sem_t               m_hGetSemaphore;        ///< 消费者信号量
        std::list<Command*> m_lstCmd;               ///< 命令队列

        static CCommandMgr *m_pCmdMgr;              ///< 命令对象指针
};

#endif /* COMMANDMANAGER_H_ */
