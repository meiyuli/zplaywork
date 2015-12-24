/*
 * EmDmapWriteKafka.h
 *
 *  Created on: Dec 19, 2014
 *      Author: meiyuli
 */

#ifndef SRC_KERNEL_EMDMAPWRITEKAFKA_H_
#define SRC_KERNEL_EMDMAPWRITEKAFKA_H_
#include <map>
#include <string>
#include "../include/rdkafka/rdkafkaproducer.h"

using std::map;
using std::string;
class EmDmapWriteKafka {
public:
	EmDmapWriteKafka();
	virtual ~EmDmapWriteKafka();

public:
	///> 初始化
	int32_t Init(const string &topic, const string &group);

	///> 析构处理
	int32_t UnInit();

	///> 写kafka数据
	int32_t WriteKafka();

private:
	///> kafka 生产者
	map<string, rdkafka_producer*> 	m_nTopicProducers;
};

#endif /* SRC_KERNEL_EMDMAPWRITEKAFKA_H_ */
