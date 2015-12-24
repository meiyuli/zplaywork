/*
 * Log.h
 *
 *  Created on: 2012-2-1
 *      Author: root
 */

#ifndef LOG_H_
#define LOG_H_

/** Log Level Part **/
#define DISABLE 4 //Disable - system is unusable, and will aborted. For example: Memory Allocation failed.
#define ERROR 	3 //Emergencies - system is unusable, and will aborted. For example: Memory Allocation failed.
#define WARN 	2 //Some bad thing has occurs, but the situation is recoverable. For example: the default region port has been taken.
#define INFO 	1 //Some important event happens, need to record it. For example: the memstore is flushing.
#define DEBUG 	0 //Store all performance and command info. For example: the time of command execution

/* tty, i.e console */
#define LOG_TTY						0x80000000
/* to file or not */
#define LOG_TOFILE                  0x04000000

class Log {
public:
	Log();
	~Log();
public:
	bool SetupLogging(int log_level, char *log_file_path, char *log_idx_file_path);
	void Logg(int level, const char *fmt, ...);

private:
	bool InitIndex();// 设置当前日志的序列号
	bool LogIndex(int idx);
	long GetFlSize(int fd);
	bool OpenLogFile();
private:
	int 	m_log_level;
	int		m_log_idx;
	char 	*m_log_file_path;
	char 	*m_log_idx_file_path;
	int		m_curr_pid;
	int		m_log_fd;			/* current log file descriptor */
};

#define logg 					g_pLog->Logg
#define setup_logging			g_pLog->SetupLogging
extern	Log *g_pLog;

#endif /* LOG_H_ */
