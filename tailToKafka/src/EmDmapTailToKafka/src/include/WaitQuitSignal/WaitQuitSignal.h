/*
 * WaitQuitSignal.h
 *
 *  Created on: Apr 14, 2015
 *      Author: meiyuli
 */

#ifndef SRC_INCLUDE_WAITQUITSIGNAL_WAITQUITSIGNAL_H_
#define SRC_INCLUDE_WAITQUITSIGNAL_WAITQUITSIGNAL_H_

#include <signal.h>
#include <time.h>
class WaitQuitSignal {
public:
	WaitQuitSignal();
	virtual ~WaitQuitSignal();
public:
    static void init();
    static bool wait(bool&flag);
private:
    static sigset_t m_wait_mask;
    static struct timespec m_time;
};

#endif /* SRC_INCLUDE_WAITQUITSIGNAL_WAITQUITSIGNAL_H_ */
