/*
 * tmutex.h
 *
 *  Created on: 2014-7-5
 *      Author: YL.M
 */

///
/// @brief
///     实现linux的互斥量c++封装
///
#ifndef TMUTEX_H
#define TMUTEX_H
#include <pthread.h>

///
/// @brief
///     线程互斥量
///
struct ThreadMutex
{
        ThreadMutex()
        {
                pthread_mutex_init(&mtx,NULL);
        }
        ~ThreadMutex()
        {
                pthread_mutex_destroy( &mtx );
        }
        inline void lock()
        {
                pthread_mutex_lock( &mtx );
        }
        inline void unlock()
        {
                pthread_mutex_unlock( &mtx );
        }

        pthread_mutex_t mtx;
};

///
/// @brief
///     空互斥量，即调用lock时什么事都不做
///
struct NullMutex
{
        inline void lock()  {}
        inline void unlock()  {}
};

template<class T>
class CAutoGuard
{
public:
        CAutoGuard(T &mtx) : m_mtx(mtx)
        {
                m_mtx.lock();
        }
        ~CAutoGuard()
        {
                m_mtx.unlock();
        }
protected:
        T &m_mtx;
};

#define AUTO_GUARD( guard_tmp_var, MUTEX_TYPE, mtx ) CAutoGuard<MUTEX_TYPE> guard_tmp_var(mtx)

#endif /* TMUTEX_H_ */
