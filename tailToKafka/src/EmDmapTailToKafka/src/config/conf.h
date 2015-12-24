/*
 * Conf.h
 *
 *  Created on: 2012-2-1
 *      Author: root
 */

#ifndef CONF_H_
#define CONF_H_

#define MID_SEPARATOR			'.'
#define MID_SEPARATOR_STRING	"."

#define STRING_SEPARATOR	','
#define STRING_SEPARATOR_STRING	","

#define LINE_SEPARATOR	'\n'
#define LINE_SEPARATOR_STRING "\n"

#define ITEM_SEPARATOR	';'
#define ITEM_SEPARATOR_STRING ";"

#define FOLDER_SEPARATOR 		'/'
#define FOLDER_SEPARATOR_STRING "/"

#define WORD_SEPARATOR				0x01

#define FIELD_COUNT_PV 				22
#define FIELD_COUNT_EVENT 			20
#define FIELD_COUNT_ORDER 			21
#define FIELD_COUNT_ORDER_DETAIL 	18
#define FIELD_COUNT_BS				40

#define TYPE_PV						101
#define TYPE_EVENT					102
#define TYPE_ORDER					103
#define TYPE_ORDER_DETAIL			104
#define TYPE_BS						201

#define SESSION_KEY_VALUE 			"1"
#define SESSION_KEY_EXPIRATION 		86400

// HTK COLLECTOR LIST
#define HTK_COLLECTOR_PV			"101"
#define HTK_COLLECTOR_EVENT			"102"
#define HTK_COLLECTOR_ORDER			"103"
#define HTK_COLLECTOR_ORDER_DETAIL	"104"
#define HTK_COLLECTOR_BS			"201"

#define MONITOR_DUARATION			3600
// string tokens operation part
typedef struct _Tokens{
	int size;
	char **tokens;
}Tokens;
Tokens *init_tokens(char *str, char deli);
Tokens *clean_tokens(Tokens *tokens);
void free_tokens(Tokens *tokens);

char *m_cpy(const char *src);
bool match(char *dest,const char *src);

#include <stdint.h>
#include <deque>
#include <set>
#include <string>
#include <regex.h>

using std::set;
using std::deque;
using std::string;

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
             "abcdefghijklmnopqrstuvwxyz" \
             "0123456789+/";

namespace Conf {
	void flush_key_value(const char *file_path, char *key, char *value);
	char *m_get_key(char *line);
	char *m_get_value(char *line);
	char *key_and_value_to_line(char *key, char *value);
	char *m_get_value_by_key(const char *file_path,const char *target_key);
	unsigned long get_ulong_value_by_key(const char *file_path,const char *target_key);
	long get_long_value_by_key(const char *file_path, const char *target_key);
	long long int get_long_long_value_by_key(const char *file_path, const char *target_key);
	int32_t Str2Gmt(const string &date, const string &time, time_t &ti);
	int32_t Str2Gmt(const string &dateTime, time_t &ti);
	int32_t Str2Gmt_T(const string &dateTime, time_t &ti);
	int32_t ParseMessageData(const char *value, deque<string> &data);
	int32_t ParseLineData(char *data, const char* separator, set<string> &result);

	void get_standard_date(char *date);
	string GetStringDigit(const string &str);
	bool IsStringDigit(const string &str);

	int32_t m_get_seperator_str(const string &str, const string &begin, const string &end, string &matched);
	uint64_t conf_get_time_by_seconds();

	int32_t ParseDecimData(char *data, const char* separator, deque<string> &result);
	int32_t SafeDecimData(const string &data, const char &separator, deque<string> &result);

	std::string base64_encode(unsigned char const* , unsigned int len);
	std::string base64_decode(std::string const& s);
	bool is_file_valid(const string &fileName, const string &pattern);
};
//------------------------------------------------------------------------------------------------------------------
#endif /* CONF_H_ */
