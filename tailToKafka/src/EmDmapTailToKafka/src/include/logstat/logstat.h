/*
 * logstat.h
 *
 *  Created on: Jul 23, 2014
 *      Author: meiyuli
 */

#ifndef LOGSTAT_H_
#define LOGSTAT_H_
#include <pthread.h>
#define MAX_FILE_PATH		256

class logstat {
public:
	logstat();
	virtual ~logstat();

public:
	bool Init(const char *path);

	void LogPrint(const char *fmt, ...);

	void LogFlush();
protected:
	void UnInit();

	bool OpenLogFile();

	void GetCurrentTime(char *dateTime);

	void GetCurrentDate_Hour(char *date, char *hour);

	void GetFileName(const char *path, char *fullPath);

	long GetFileSize(int fd);

	int  MakeDirection(const char *dirPath);
private:
	pthread_mutex_t		lock_;

	int					fd_;

	char				filePath_[MAX_FILE_PATH];

	/**
	 * \brief 文件创建某天小时
	 */
	char				fileHour_[64];
};

#define log_flush		 g_plogstat->LogFlush
#define log_printf		 g_plogstat->LogPrint
#define log_init		 g_plogstat->Init

#define log_monit_flush	 g_pmonitor->LogFlush
#define log_monit_printf g_pmonitor->LogPrint
#define log_monit_init	 g_pmonitor->Init

extern logstat *g_plogstat;
extern logstat *g_pmonitor;
#endif /* LOGSTAT_H_ */
