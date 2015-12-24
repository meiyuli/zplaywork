/*
 * md5.cpp
 *
 *  Created on: 2013-11-27
 *      Author: root
 */

#include "md5.h"
#include <openssl/md5.h>

uint64_t md5_sum(unsigned char *key, uint32_t length) {
	unsigned char md[16]={0};
	MD5(key,length,md);
	return *((uint64_t *)md);
}
