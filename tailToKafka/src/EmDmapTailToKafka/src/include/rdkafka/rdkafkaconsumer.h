/*
 * rdkafkaconsumer.h
 *
 *  Created on: Nov 24, 2014
 *      Author: meiyuli
 */

#ifndef RDKAFKACONSUMER_H_
#define RDKAFKACONSUMER_H_

#include "rdkafkabase.h"

class rdkafka_consumer :public rdkafka_base{
public:
	rdkafka_consumer();
	virtual ~rdkafka_consumer();

public:
	int32_t Init(int32_t partition, const char* broker, const char* topic, const char* group);
	int32_t UnInit();

	int32_t Consume_Message(int64_t offset);

	virtual RDKAFKA_TYPE Get_Kafka_Type() const;
};

#endif /* RDKAFKACONSUMER_H_ */
