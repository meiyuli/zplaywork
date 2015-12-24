/*
 * md5.h
 *
 *  Created on: 2013-11-27
 *      Author: root
 */

#ifndef MD5_H_
#define MD5_H_
#include <stdint.h>

uint64_t md5_sum(unsigned char *key, uint32_t length);

#endif /* MD5_H_ */
