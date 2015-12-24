/*
 * EmDmapReadFile.h
 *
 *  Created on: Dec 19, 2014
 *      Author: meiyuli
 */

#ifndef SRC_KERNEL_EMDMAPREADFILE_H_
#define SRC_KERNEL_EMDMAPREADFILE_H_
#include <map>
#include <string>
#include "FileManager.h"

using std::map;
using std::string;

class EmDmapReadFile {
public:
	EmDmapReadFile();
	virtual ~EmDmapReadFile();

public:
	int32_t Init(const string &topic, const string &filePath, const string &pattern);

	int32_t UnInit();
	int32_t ReadFile();

protected:
	static void *Process_Files(void *);
private:

	map<string, FileManager*> m_nFileMgr;
	pthread_mutex_t 	  m_nLock;
};

#endif /* SRC_KERNEL_EMDMAPREADFILE_H_ */
