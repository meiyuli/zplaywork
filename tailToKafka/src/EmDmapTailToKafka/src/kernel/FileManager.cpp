/*
 * FileManager.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: meiyuli
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>

#include "FileManager.h"
#include "../config/config.h"
#include "../config/conf.h"
#include "../include/log/log.h"
#include "../include/lock/mutex_lock.h"
#include "../include/command/commandmanager.h"
#include "../include/WaitQuitSignal/WaitQuitSignal.h"

#define FILE_HANDLE_TIME_INTERVAL 600
#define HANDLED_FILE_NAME ".handled.list"
#define HANDLED_FILE_NAME_TEMP ".handled.list.temp"

bool g_bTaskFlag = true;
FileManager::FileManager() {
	// TODO Auto-generated constructor stub
	m_pLock = NULL;
}

FileManager::~FileManager() {
	// TODO Auto-generated destructor stub
}

int32_t FileManager::Init(const string &topic, const string &filePath, const string &pattern, pthread_mutex_t *pLock) {
	m_nTopic = topic;
	m_nFilePath = filePath;
	m_nPattern = pattern;
	m_pLock = pLock;
	return 0;
}

int32_t FileManager::LoadHandledFileList(const char *dataDir, map<string,long> &handledFileList) {
	if (!dataDir) return -1;

	char listFile[FILE_PATH_LEN] = {0};
	snprintf(listFile, sizeof(listFile),"%s/%s", dataDir, HANDLED_FILE_NAME);

	FILE *fp = fopen(listFile, "a+");
	if (!fp) {
		logg(ERROR, "cant open file for loading handled file list, file: %s", listFile);
		return -2;
	}

	while(!feof(fp)) {
		char handledfile[FILE_PATH_LEN] = {0};
		char fileName[FILE_PATH_LEN] = {0};		///> 文件名
		long lPosition = 0L;							///> 当前文件读取位置


		if (fgets(handledfile, FILE_PATH_LEN, fp)) {
			handledfile[strlen(handledfile) - 1] = 0;
			char *pch = strrchr(handledfile,':');
			if (NULL == pch) {
				logg(ERROR, "handledfile:%s" , handledfile);
				continue;
			}
			strncpy(fileName, handledfile, pch - handledfile);
			lPosition = atol(pch+1);
			map<string,long>::iterator it = handledFileList.find(fileName);
			if (it != handledFileList.end() && (it->second < lPosition)) {
				it->second = lPosition;
			}else {
				handledFileList.insert(pair<string,long>(fileName,lPosition));
			}
		}
	}

	fclose(fp);
	return 0;
}

int32_t FileManager::AddAHandledFileToFileList(const char *dataDir, const char *fileName, long size) {
	if (!dataDir || !fileName) return -1;

	char listFile[FILE_PATH_LEN] = {0};
	sprintf(listFile, "%s/%s", dataDir, HANDLED_FILE_NAME);

	FILE *fp = fopen(listFile, "a+");
	if (!fp) {
		logg(ERROR, "cant open file for add a handled file to filelist, file: %s", listFile);
		return -2;
	}

	fprintf(fp, "%s:%ld\n", fileName, size);
	fflush(fp);
	fclose(fp);
	return 0;
}

int32_t FileManager::AddHandledFilesToFileList(const char *dataDir, map<string, long> &handledFileList) {
 	MutexLock lock(m_pLock);
	
	if (!dataDir) return -1;

	char srcFile[FILE_PATH_LEN] = {0};
	sprintf(srcFile, "%s/%s", dataDir, HANDLED_FILE_NAME);

	char listFile[FILE_PATH_LEN] = {0};
	sprintf(listFile, "%s/%s", dataDir, HANDLED_FILE_NAME_TEMP);

	FILE *fp = fopen(listFile, "w");
	if (!fp) {
		logg(ERROR, "cant open file for add a handled file to filelist, file: %s", listFile);
		return -2;
	}

	map<string, long>::iterator it = handledFileList.begin();
	for (;it != handledFileList.end(); it++) {
		fprintf(fp, "%s:%ld\n", it->first.c_str(), it->second);
	}
	fflush(fp);
	fclose(fp);

	if (rename(listFile, srcFile) != 0) {
		logg(ERROR, "rename opts is error for:[%s]", strerror(errno));
		return errno;
	}
	return 0;
}

/*
//int32_t FileManager::ProcessFilesFromDirectory(const char *dataDir, const char *suffix) {
int32_t FileManager::ProcessFilesFromDirectory() {
//	if (!dataDir)  return -1;

	map<string, long> handledFileList;
	map<string, long> handlingFileList;

	if (this->LoadHandledFileList(m_nFilePath.c_str(), handledFileList)) return -2;

	///> 取后缀名
	set<string> suffixs;
	char tmp_suffix[256];
	snprintf(tmp_suffix, 256, "%s", m_nSuffix.c_str());
	Conf::ParseLineData(tmp_suffix,"*; ",suffixs);
	while (1) {
		char currDir[256] = {0};
		char date[16] = {0};

//		Conf::get_standard_date(date);
//		sprintf(currDir, "%s/%s", m_nFilePath.c_str(), date);

		snprintf(currDir, sizeof(currDir), "%s", m_nFilePath.c_str());

		DIR *pdir;
		struct dirent *ent;
		struct stat stStatBuf;

		pdir = opendir(currDir);
		if (!pdir) {
			logg(WARN, "cant open dir for handling file, dir: %s", currDir);
			sleep(10);
			continue;
		}

		while((ent = readdir(pdir)) != NULL) {

			char filename[FILE_PATH_LEN] = {0};
			sprintf(filename, "%s/%s", currDir, ent->d_name);
			memset(&stStatBuf, 0x0, sizeof(struct stat));
			if (stat(filename, &stStatBuf) == -1) {
				continue;
			}
			if (stStatBuf.st_mode & S_IFDIR) continue;
			///> 拿到匹配文件
			if (stStatBuf.st_mode & S_IFREG) {
				char *pch = strrchr(ent->d_name,'.');
				if (pch && (suffixs.find(pch) != suffixs.end())) {

					map<string, long>::iterator it = handledFileList.find(filename);
					if (it == handledFileList.end()) {
						handlingFileList.insert(pair<string, uint32_t>(filename, 0));
					}else {

						if (stStatBuf.st_size > it->second) {
							handlingFileList.insert(pair<string, uint32_t>(filename, it->second));
						}
					}
				}
			}
		}
		closedir(pdir);

		for (map<string, long>::iterator it = handlingFileList.begin(); it != handlingFileList.end(); ) {
			const char *fileName = (*it).first.c_str();
			long size = (*it).second;

			size = this->FilesRead(fileName, size);
			this->AddAHandledFileToFileList(m_nFilePath.c_str(), fileName, size);
			if (handledFileList.find(fileName) != handledFileList.end()) {
				handledFileList[fileName] = size;
			}else {
				handledFileList.insert(pair<string,long>(fileName,size));
			}
			handlingFileList.erase(it);
			it = handlingFileList.begin();
		}

		sleep(1);
	}

	return 0;
}
*/

int32_t FileManager::ProcessFilesFromDirectory() {
//	if (!dataDir)  return -1;

	map<string, long> handledFileList;
	map<string, long> handlingFileList;

	if (this->LoadHandledFileList(m_nFilePath.c_str(), handledFileList)) return -2;

	while (1) {

		this->DirRead(m_nFilePath.c_str(), handlingFileList, handledFileList);

		if (false == g_bTaskFlag) {
			break;
		}

		if (handlingFileList.empty()) {
			sleep(1);
			continue;
		}
		for (map<string, long>::iterator it = handlingFileList.begin(); it != handlingFileList.end(); ) {
			const char *fileName = (*it).first.c_str();
			long size = (*it).second;

			if (size < 0) {
				logg(WARN, "[%s:%d] file:%s,pos:%d", __FUNCTION__, __LINE__, fileName, size);
				size = 0;
			}

			long nSize = this->FilesRead(fileName, size);
			if (nSize < size) {
			// 处理异常
				logg(WARN, "[%s:%d] file:%s,pos:%d exception.", __FUNCTION__, __LINE__, fileName, nSize);
				nSize = size;
			}

			// this->AddAHandledFileToFileList(m_nFilePath.c_str(), fileName, size);
			if (handledFileList.find(fileName) != handledFileList.end()) {
				handledFileList[fileName] = nSize;
			}else {
				handledFileList.insert(pair<string,long>(fileName,nSize));
			}
			handlingFileList.erase(it);
			it = handlingFileList.begin();

			if (false == g_bTaskFlag) {
				break;
			}
		}

		this->AddHandledFilesToFileList(m_nFilePath.c_str(), handledFileList);
		// sleep(1);

		if (false == g_bTaskFlag) {
			break;
		}
	}

	return 0;
}

/**
 * \brief 每次读取文件时，根据当前文件开始时刻到大小读取。
 */

long FileManager::FilesRead(const char *file, long size) {
	if (!file) return -1;

	CCommandMgr *pCmdMgr = CCommandMgr::GetInstance();
	assert(pCmdMgr);

	ifstream input(file);
	if (!input) {
		logg(WARN, "cant open file to process, file: %s", file);
		return -2;
	}
	///> 获取文件位置
	input.seekg(0, ios::end);
	long fileLen = input.tellg();

	if (fileLen <= size) {
		///> 文件尾小于或等于所要获取位置，则直接返回
		return fileLen;
	}
	///> 设置当前获取位置
	input.seekg(size);
	input.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	long nSize = size;
//	while (!input.eof()) {

		//char line_data[MAX_DATA_LEN] = {0};
		try {
			std::string line_data;
			std::string line_record;
			while(std::getline(input, line_data)) {
				//input.getline(line_data,MAX_DATA_LEN);

				// line_data[strlen(line_data) - 1] = 0;
				// 判断line_data后两位不为\0x03\n,则记录数据，循环继续
				size_t len = line_data.length();
				if (len < 1) {
					continue;
				}

				if (line_data.substr(len - 1).compare("\003")) {
					line_record += line_data;
					continue;
				}else {
					line_record += line_data.substr(0, len -1);
				}

				DataCommand *pCmd = new DataCommand(m_nTopic,line_record);
				pCmd->m_nCmdID = CMD_DATA_DATA;
				pCmdMgr->AddOperator(pCmd);

				nSize = input.tellg(); 	// 当前读取到文件位置

				if (false == g_bTaskFlag || nSize >= fileLen) {
					break;
				}
				line_data.clear();
				line_record.clear();
			}
		} catch(std::ifstream::failure e) {
			if (!input.eof()) {
				logg(WARN, "[%s:%d] Exception :%s", __FUNCTION__, __LINE__, e.what());
			}
			usleep(1000);
		//	break;
		}
		//if (false == g_bTaskFlag) {
		//	break;
		//}
//	}

	input.close();

	return nSize;
}

/**
 * \brief 创建读文件线程池，把读文件操作加入线程池处理，程序不用停在读文件位置。
 */
/*
long FileManager::FilesRead(const char *file, long size) {
	if (!file) return -1;

	CCommandMgr *pCmdMgr = CCommandMgr::GetInstance();
	assert(pCmdMgr);

	FILE *fp = fopen(file, "r");
	if (!fp) {
		logg(WARN, "cant open file to process, file: %s", file);
		return -2;
	}
	fseek(fp, size, SEEK_SET);
	while (!feof(fp)) {

		char line_data[MAX_DATA_LEN] = {0};

		fgets(line_data, MAX_DATA_LEN, fp);
		line_data[strlen(line_data) - 1] = 0;

		DataCommand *pCmd = new DataCommand(m_nTopic,line_data);
		pCmd->m_nCmdID = CMD_DATA_DATA;
		pCmdMgr->AddOperator(pCmd);

		if (false == g_bTaskFlag) {
			break;
		}
	}
	long nSize = ftell(fp); 	// 当前读取到文件位置

	fclose(fp);

	return nSize;
}
*/
int32_t FileManager::DirRead(const char *dataDir, map<string, long> &handlingFileList, map<string, long> &handledFileList) {

/*
  	char date[16] = {0};
	Conf::get_standard_date(date);
	sprintf(currDir, "%s/%s", m_nFilePath.c_str(), date);
*/

	DIR *pdir;
	struct dirent *ent;
	struct stat stStatBuf;

	pdir = opendir(dataDir);
	if (!pdir) {
		logg(WARN, "cant open dir for handling file, dir: %s", dataDir);
		sleep(10);
		return 0;
//		continue;
	}

	while((ent = readdir(pdir)) != NULL) {

		char filename[FILE_PATH_LEN] = {0};
		sprintf(filename, "%s/%s", dataDir, ent->d_name);
		memset(&stStatBuf, 0x0, sizeof(struct stat));
		if (stat(filename, &stStatBuf) == -1) {
			continue;
		}

		if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0) {
			continue;
		}

		if (stStatBuf.st_mode & S_IFDIR) {
			DirRead(filename, handlingFileList, handledFileList);
			continue;
		}
		///> 拿到匹配文件
		if (stStatBuf.st_mode & S_IFREG) {
			// printf("%s match %s\n", ent->d_name, m_nPattern.c_str());
			if (Conf::is_file_valid(ent->d_name, m_nPattern)) {

			//	printf("%s match %s\n", ent->d_name, m_nPattern.c_str());
				map<string, long>::iterator it = handledFileList.find(filename);
				if (it == handledFileList.end()) {
					handlingFileList.insert(pair<string, long>(filename, 0L));
				}else {

					if (stStatBuf.st_size > it->second) {
						handlingFileList.insert(pair<string, long>(filename, it->second));
					}
				}
			}
		}
	}
	closedir(pdir);
	return 0;
}
