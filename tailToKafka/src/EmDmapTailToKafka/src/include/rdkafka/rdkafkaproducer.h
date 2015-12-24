/*
 * rdkafkaproducer.h
 *
 *  Created on: Nov 24, 2014
 *      Author: meiyuli
 */

#ifndef RDKAFKAPRODUCER_H_
#define RDKAFKAPRODUCER_H_

#include <string>
#include "rdkafkabase.h"

#define MAX_WAIT_KAFKA_QUEUE_SIZE		10000

class rdkafka_producer : public rdkafka_base{
public:
	rdkafka_producer();
	virtual ~rdkafka_producer();

public:
	int32_t Init(int32_t partition, const char* broker, const char* topic, const char* group);
	int32_t UnInit();

	int32_t Produce_Message(const std::string &msg);

	virtual RDKAFKA_TYPE Get_Kafka_Type() const;
};

#endif /* RDKAFKAPRODUCER_H_ */
