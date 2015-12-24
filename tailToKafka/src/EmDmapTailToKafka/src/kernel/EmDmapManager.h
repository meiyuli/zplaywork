/*
 * EmDmapManager.h
 *
 *  Created on: Dec 19, 2014
 *      Author: meiyuli
 */

#ifndef SRC_KERNEL_EMDMAPMANAGER_H_
#define SRC_KERNEL_EMDMAPMANAGER_H_
#include <map>
#include <string>
#include "EmDmapWriteKafka.h"
#include "EmDmapReadFile.h"

using std::map;
using std::string;
class EmDmapManager {
public:
	EmDmapManager();
	virtual ~EmDmapManager();

public:
	int32_t Init();
	int32_t Run();
	int32_t UnInit();

protected:
	static void *Process_WriteKafka(void *args);

private:
	EmDmapWriteKafka	m_nKafka;
	EmDmapReadFile		m_nReadFile;
};

#endif /* SRC_KERNEL_EMDMAPMANAGER_H_ */
