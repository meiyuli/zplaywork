/*
 * mutex_lock.h
 *
 *  Created on: 2013-11-27
 *      Author: root
 */

#ifndef MUTEX_LOCK_H_
#define MUTEX_LOCK_H_

#include <pthread.h>

class MutexLock {
public:
	explicit MutexLock(pthread_mutex_t *lock) :lock_(lock){
		pthread_mutex_lock(lock_);
	}
	~MutexLock() {
		pthread_mutex_unlock(lock_);
	}
private:
	MutexLock(const MutexLock&);
	MutexLock& operator=(const MutexLock&);
private:
	pthread_mutex_t 	*lock_;
};
#endif /* MUTEX_LOCK_H_ */
