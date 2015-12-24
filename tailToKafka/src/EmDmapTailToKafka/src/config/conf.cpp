/*
 * Conf.cpp
 *
 *  Created on: 2012-2-1
 *      Author: root
 */

#include "./conf.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <list>
#include <sys/time.h>

#define LINE_SEPARATOR	'\n'
#define LINE_SEPARATOR_STRING "\n"

#define CONF_SEPARATOR	'='
#define CONF_SEPARATOR_STRING	"="

static
void* mallocs(unsigned int size) {
	size += 1;
	void *ptr = malloc(size);
	if(NULL==ptr) {
		printf("Malloc[%d] memory failed,system error:%s.[%s:%d]",size,strerror(errno),__FILE__,__LINE__);
		return NULL;
	}
	memset(ptr, 0, size);
	return ptr;
}
static
char *trim(char *str, char deli) {
	// remove left side
	while(*str==deli) {
		str++;
	}
	// remove right side
	char *tmp = str;
	while('\0'!=*tmp) tmp++;
	//tmp--;// mode by lixy
	while(*tmp==deli) {
		*tmp = '\0';
		tmp--;
	}
	return str;
}
static// count the occurrence of target char inside the string
int count(char *string, char target) {
	int occurrence = 0;
	while('\0'!=*string) {
		if(target==*string) occurrence++;
		string++;
	}
	return occurrence;
}


Tokens *clean_tokens(Tokens *tokens) {
	int i=0;
	for(i=0; i<tokens->size; ++i) {
		if(NULL==tokens->tokens[i]) continue;
		trim(tokens->tokens[i], ' ');
		if(0==strlen(tokens->tokens[i])){
			tokens->size--;
			free(tokens->tokens[i]);
			if(i < tokens->size - 1){
				tokens->tokens[i] = tokens->tokens[i+1];
			}
		}
	}
	return tokens;
}

// sample str: name, sex, home_address, work_address
// sample deli: , may have some issue on deli
Tokens *init_tokens(char *str, char deli) {
	Tokens *tokens = (Tokens *)malloc(sizeof(Tokens));

	tokens->size = 0;
	if(NULL==str || 0==strlen(str)) {
		return tokens;
	}
	char *tmp = str;
	int i=0, len=strlen(str);
	int max_times = count(tmp, deli) + 2;// +1
	tokens->tokens = (char **)malloc(max_times * sizeof(char *));
	memset(tokens->tokens, 0, max_times * sizeof(char *));
	tokens->tokens[0] = (char *)mallocs(len);

	bool prev_matched = false;
	while('\0'!= *tmp) {
		if(*tmp == deli) {
			if(!prev_matched) {
				++i;
				tokens->tokens[i] = (char *)mallocs(len);
			}
			prev_matched = true;
		} else{
			prev_matched = false;
			strncat(tokens->tokens[i], tmp, 1);
		}
		++tmp;
	}
	tokens->size = i+1;
	//printf("%s:%d\n",tokens->tokens[0], tokens->size);
	return clean_tokens(tokens);
}

void free_tokens(Tokens *tokens) {
	if(NULL==tokens) return;
	int i=0;
	for(i=0; i<tokens->size; ++i){
		free(tokens->tokens[i]);
	}
	if(tokens->tokens) {
		free(tokens->tokens);
		tokens->tokens = NULL;
	}
	free(tokens);
}


static
size_t get_file_size(const char *file_path) {
	FILE *fp=fopen(file_path, "r");
	if(NULL==fp) {
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	fclose(fp);
	return size;
}

static // if returns 0,that means the file not exist
char *m_load_txt_file_to_memory(const char *file_path) {
	int file_size = get_file_size(file_path);
	if(0>=file_size) return NULL;
	char *buffer = (char *)mallocs(file_size);
	FILE *fp = fopen(file_path, "r");
	if(NULL==fp) {
		return NULL;
	}
	fread(buffer, file_size, 1, fp);
	fclose(fp);
	return buffer;
}


static
char *cpy(char *dest,const char *src) {
	int len = strlen(src);
	memset(dest, 0, len+1);
	return strncpy(dest, src, len);
}

char *m_cpy(const char *src) {
	if(NULL==src) return NULL;
	char *s = (char *)mallocs(strlen(src) + 1);
	return cpy(s,src);
}

static
void generate_list_by_token(char *bytes, char token, std::list<char *> &items) {
	Tokens *tokens = init_tokens(bytes, token);
	for(int i=0; i<tokens->size; ++i) {
		if(0==strlen(tokens->tokens[i])) continue;
		char *item = m_cpy(tokens->tokens[i]);
		items.push_back(item);
	}
	free_tokens(tokens);
}

static
void get_lines_from_text_file(const char *file_path, std::list<char *> &lines) {
	char *file_content = m_load_txt_file_to_memory(file_path);
	if(NULL==file_content) {
		return ;
	}
	generate_list_by_token(file_content, LINE_SEPARATOR, lines);
	free(file_content);
	return ;
}

static
int max_t(int a, int b) {
	return (a>b ? a:b);
}
static
bool cmp(char *dest,const char *src, int len) {
	bool result = false;
	if(0==strncasecmp(dest,src,len)) result = true;
	return result;
}

// the safe version of cmp, will used max len between the two strings and ignore case
bool match(char *dest,const char *src){
	int max_len = max_t(strlen(dest), strlen(src));
	// Base on previous experience, strcmp may not fit for this situation
	return cmp(dest, src, max_len);
}

char *Conf::m_get_key(char *line) {
	Tokens *tokens = init_tokens(line, CONF_SEPARATOR);
	char *key = m_cpy(trim(tokens->tokens[0], ' '));
	free_tokens(tokens);
	return key;
}

char *Conf::m_get_value(char *line) {
	char *value = NULL;
	Tokens *tokens = init_tokens(line, CONF_SEPARATOR);
	if(tokens->size > 1){
		value = m_cpy(trim(tokens->tokens[1], ' '));
	}
	free_tokens(tokens);

	return value;
}

// if the target key has not been found, the method will return NULL
char *Conf::m_get_value_by_key(const char *file_path,const char *target_key) {
	char *value = NULL;
	std::list<char *> lines;
	get_lines_from_text_file(file_path,lines);
	char *line = NULL;
	std::list<char *>::iterator iter,beg,end;
	beg = lines.begin();
	end = lines.end();
	for(iter=beg; iter!=end; ++iter) {
		line = *iter;
		if(NULL==line) break;
		if(0<count(line,CONF_SEPARATOR)){
			char *key = m_get_key(line);
			if(match(key,target_key)) {
				value = m_get_value(line);
			}
			free(key);
		}
		free(line);
		line = NULL;
	}

	if(NULL!=value){
		trim(value,' ');
	}
	return value;
}

unsigned long Conf::get_ulong_value_by_key(const char *file_path,const char *target_key) {
	unsigned long ul_value = 0;
	char *value = m_get_value_by_key(file_path, target_key);
	if(NULL!=value) {
		ul_value = strtoull(value,NULL,10);
		free(value);
	}
	return ul_value;
}

long  Conf::get_long_value_by_key(const char *file_path, const char *target_key) {
	long l_value = -1;
	char *value = m_get_value_by_key(file_path, target_key);
	if (NULL != value) {
		l_value = strtol(value,NULL,10);
		free(value);
	}
	return l_value;
}

long long int  Conf::get_long_long_value_by_key(const char *file_path, const char *target_key) {
	long long int l_value = -1;
	char *value = m_get_value_by_key(file_path, target_key);
	if (NULL != value) {
		l_value = strtoll(value,NULL,10);
		free(value);
	}
	return l_value;
}

int32_t Conf::ParseMessageData(const char *value, deque<string> &data) {
	if (!value) return -1;

	uint32_t len = strlen(value);
	const char *token,*curr = value;
	token = strchr(curr, LINE_SEPARATOR);
	while(1) {
		if(token==NULL) {
			if(curr != value + len) {
				data.push_back(string(curr));
			}
			break;
		}else {
			data.push_back(string(curr,token-curr));
			curr = token + 1;
			token = strchr(curr, LINE_SEPARATOR);
		}
	}
	return 0;
}

int32_t Conf::ParseLineData(char *data, const char* separator, set<string> &result) {
	if (!data) return -1;

	char *pch;
	char *last = NULL;
	pch = strtok_r(data,separator,&last);
	while (pch != NULL) {
		result.insert(pch);
		pch = strtok_r(NULL,separator,&last);
	}

	return 0;
}

int32_t Conf::Str2Gmt(const string &date, const string &time, time_t &ti) {

	return Str2Gmt(date + string(" ") + time, ti);
}

int32_t Conf::Str2Gmt(const string &dateTime, time_t &ti) {
	struct tm tmS;
	strptime(dateTime.c_str(),"%Y-%m-%d %H:%M:%S", &tmS);
/*	tmS.tm_year -= 1900;
	tmS.tm_mon --;

	tmS.tm_isdst=-1;
*/
	ti = mktime(&tmS);
	return 0;
}

int32_t Conf::Str2Gmt_T(const string &dateTime, time_t &ti) {
	struct tm tmS;
	strptime(dateTime.c_str(),"%Y%m%d%H%M%S", &tmS);
/*	tmS.tm_year -= 1900;
	tmS.tm_mon --;

	tmS.tm_isdst=-1;
*/
	ti = mktime(&tmS);
	return 0;
}

string Conf::GetStringDigit(const string &str) {
	int pos = 0;
	while(str[pos] != '\0') {
		if (str[pos] >= '0' && str[pos] <= '9') break;
		pos ++;
	}
	return str.substr(pos);
}

bool Conf::IsStringDigit(const string &str) {
	size_t pos = 0;
	while (str[pos] != '\0') {
		if (str[pos] < '0' || str[pos] > '9') break;
		pos ++;
	}

	return pos == str.length();
}

int32_t Conf::ParseDecimData(char *data, const char* separator, deque<string> &result) {
	if (!data) return -1;

	char *pch;
	char *last = NULL;
	pch = strtok_r(data,separator,&last);
	while (pch != NULL) {
		result.push_back(pch);
		pch = strtok_r(NULL,separator,&last);
	}

	return 0;
}

int32_t Conf::SafeDecimData(const string &data, const char &separator, deque<string> &result) {
	size_t posB,posE;
	posB = posE = 0;
	while (true) {
		posE = data.find(separator,posB);
		if (posE == string::npos) {
		   break;
		}
		if (posE == posB) {
			result.push_back(string(""));
		}else {
			result.push_back(data.substr(posB,posE - posB));
		}
		posB = posE + 1;
	}

	if (posB != data.size()) {
		result.push_back(data.substr(posB));
	}
	return 0;
}

uint64_t Conf::conf_get_time_by_seconds()
{
	struct timeval tval;
	gettimeofday(&tval, NULL);
	return tval.tv_sec;
}

int32_t Conf::m_get_seperator_str(const string &str, const string &begin, const string &end, string &matched) {

	size_t nPosBegin = str.rfind(begin);
	if (nPosBegin == string::npos) {
		return -1;
	}
	size_t nPosEnd = str.find(end,nPosBegin);
	if (nPosEnd == string::npos) {
		return -2;
	}

	matched = str.substr(nPosBegin + begin.size(), nPosEnd - nPosBegin - begin.size());
	return 0;
}

/*
   base64.cpp and base64.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string Conf::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string Conf::base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

void Conf::get_standard_date(char *date) {
	time_t t=time(NULL);
	struct tm *_tm=localtime(&t);

	strftime(date,12,"%Y%m%d", _tm); //将tm时间转换为字符串
}

bool Conf::is_file_valid(const std::string &fileName, const std::string &strPattern) {
	
//	const std::regex pattern(strParttern.c_str());
//	return std::regex_match(fileName, pattern);
	
	int nErrCode = 0;
	int cflags = REG_EXTENDED;
	regex_t oRegex;
	if ((nErrCode = regcomp(&oRegex, strPattern.c_str(), cflags)) == 0) {
		
		if ((nErrCode = regexec(&oRegex, fileName.c_str(), 0, NULL, 0)) == 0) {
		
			regfree(&oRegex);
			return true;
		}
	}
	regfree(&oRegex);
	return false;
}
