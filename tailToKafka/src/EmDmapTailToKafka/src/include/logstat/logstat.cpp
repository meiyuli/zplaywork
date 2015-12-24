/*
 * logstat.cpp
 *
 *  Created on: Jul 23, 2014
 *      Author: meiyuli
 */

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "logstat.h"
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include "../../config/conf.h"
#include "../../config/config.h"

#define FILE_PREFIX		"tokafka"
#define MAX_LOGSTAT_SIZE	1024*1024*10	// 10M

logstat g_logstat;
logstat *g_plogstat = &g_logstat;

logstat g_monitor;
logstat *g_pmonitor = &g_monitor;

logstat::logstat() :fd_(-1){
	// TODO Auto-generated constructor stub
	memset(filePath_,0,sizeof(filePath_));
	memset(fileHour_,0,sizeof(fileHour_));
}

logstat::~logstat() {
	// TODO Auto-generated destructor stub

	UnInit();
}

bool logstat::Init(const char *path) {
	strncpy(filePath_,path,sizeof(filePath_));
	pthread_mutex_init(&lock_,NULL);
	return true;
}

void logstat::UnInit() {
	if (fd_ != -1) {
		::close(fd_);
		fd_ = -1;
	}
	pthread_mutex_destroy(&lock_);
}

void logstat::GetCurrentTime(char *dateTime) {
	time_t _tm = time(NULL);
	struct tm *ptm;

	ptm = localtime(&_tm);
	sprintf(dateTime,"%04d-%02d-%02d %02d:%02d:%02d",
        	ptm->tm_year+1900,
        	ptm->tm_mon+1,
        	ptm->tm_mday,
        	ptm->tm_hour,
        	ptm->tm_min,
        	ptm->tm_sec);
}

void logstat::GetCurrentDate_Hour(char *date, char *hour) {
	time_t _tm = time(NULL);
	struct tm *ptm;

	ptm = localtime(&_tm);
	sprintf(date,"%04d%02d%02d",
        	ptm->tm_year+1900,
        	ptm->tm_mon+1,
        	ptm->tm_mday);

    sprintf(hour,"%02d", ptm->tm_hour);
}

void logstat::GetFileName(const char *path, char *fullPath) {
	time_t _tm = time(NULL);
	struct tm *ptm ;

	ptm = localtime(&_tm);
	string type("tail");
/*
	switch(G_DATA_ANALYSIS) {
	case TYPE_PV:
		type = string("pv");
		break;
	case TYPE_EVENT:
		type = string("event");
		break;
	case TYPE_ORDER:
		type = string("order");
		break;
	case TYPE_ORDER_DETAIL:
		type = string("detail");
		break;
	case TYPE_BS:
		type = string("bs");
		break;
	default :
		type = string("unknown");
		break;
	}
*/
	sprintf(fullPath,"%s/%s_%s_%04d%02d%02d_%02d.dat", path, FILE_PREFIX,type.c_str(),
        	ptm->tm_year+1900,
        	ptm->tm_mon+1,
        	ptm->tm_mday,
        	ptm->tm_hour);
}

int logstat::MakeDirection(const char *dirPath) {

	if (!dirPath) return -1;

	DIR *dir = opendir(dirPath);
	if (!dir) {
		mkdir(dirPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	} else {
		closedir(dir);
	}

	return 0;
}

long logstat::GetFileSize(int fd) {
	if (fd == -1) {
		return 0;
	}
	struct stat stat;
	int ret = fstat(fd, &stat);
	if (ret < 0) {
		return 0;
	}
	return stat.st_size;
}

bool logstat::OpenLogFile() {
	if (fd_ != -1) {
		return true;
	}
	char fullPath[MAX_FILE_PATH] = {0};
	char temp[MAX_FILE_PATH] = {0};
	time_t now = time(NULL);
	char date[64];
	strftime(date,sizeof(date),"%Y%m%d",localtime(&now));

	sprintf(temp, "%s/%s", filePath_, date);
	MakeDirection(temp);

	GetFileName(temp, fullPath);
	fd_ = ::open(fullPath, O_RDWR|O_CREAT, S_IWUSR|S_IRUSR);
	if (fd_ == -1) {
		return false;
	}
	strftime(fileHour_,sizeof(fileHour_),"%Y%m%d_%H",localtime(&now));

	return true;
}

void logstat::LogPrint(const char *fmt, ...) {

	if(fd_ == -1) {
		pthread_mutex_lock(&lock_);
		if(!OpenLogFile()) {
			printf("[WARNING!] Log object open file failed,error:%s.[%s:%d]\n",strerror(errno),__FILE__,__LINE__);
			pthread_mutex_unlock(&lock_);
			return ;
		}
		pthread_mutex_unlock(&lock_);
	}

	time_t now = time(NULL);
	char buf[64] = {0};
	char dateHour[64] = {0};

	//strftime(buf, 64, "%d %b %H:%M:%S",localtime(&now));
	strftime(buf, 64, "%Y/%m/%d %H:%M:%S",localtime(&now));
	strftime(dateHour,sizeof(dateHour), "%Y%m%d_%H", localtime(&now));

	pthread_mutex_lock(&lock_);
	//dprintf(fd_, "[%d] %s", getpid(), buf);
	dprintf(fd_, "[%s]\1", buf);
	va_list ap;
	va_start(ap, fmt);
	vdprintf (fd_, fmt, ap);
	va_end(ap);
	dprintf(fd_, "\n");
//	fsync(fd_);

	if(strcmp(fileHour_,dateHour)) {
		::close(fd_);
		fd_ = -1;
	}
	pthread_mutex_unlock(&lock_);
	return ;
}

void logstat::LogFlush() {
	pthread_mutex_lock(&lock_);
	fsync(fd_);
	pthread_mutex_unlock(&lock_);
}
