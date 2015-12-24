/*
 * FileManager.h
 *
 *  Created on: Aug 6, 2014
 *      Author: meiyuli
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <set>
#include <map>
#include <string>
#include <stdint.h>

using std::map;
using std::set;
using std::string;

#define FILE_PATH_LEN 256
#define MAX_DATA_LEN 131072  //-- (128 * 1024)

class FileManager {
public:
	FileManager();
	virtual ~FileManager();

public:
	/**
	 * \brief 设置数据主题
	 */
	int32_t Init(const string &topic, const string &filePath, const string &pattern, pthread_mutex_t *plock);

	/**
	 * \brief 从路径提取文件
	 */
//	int32_t ProcessFilesFromDirectory(const char *dataDir, const char *suffix);
	int32_t ProcessFilesFromDirectory();

	/**
	 * \brief 导入已加载数据文件
	 */
	int32_t LoadHandledFileList(const char *dataDir, map<string,long> &handledFileList);

	/**
	 * \brief 添加已读文件
	 */
	int32_t AddAHandledFileToFileList(const char *dataDir, const char *fileName, long size);

	int32_t AddHandledFilesToFileList(const char *dataDir, map<string,long> &handledFileList);

	/**
	 * \brief 读文件
	 */
	long FilesRead(const char *file, long size);

	/**
	 * \brief 处理文件夹
	 */
	int32_t DirRead(const char *dataDir, map<string, long> &handlingFileList, map<string, long> &handledFileList);

private:
	///> 数据对应主题
	string 			m_nTopic;

	string 			m_nFilePath;

	string			m_nPattern;
	
	pthread_mutex_t		*m_pLock;
};

#endif /* FILEMANAGER_H_ */
