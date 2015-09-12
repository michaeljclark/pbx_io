//
//  util.cc
//

#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cerrno>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>

#include "log.h"
#include "util.h"


/* utility */

const char* util::HEX_DIGITS = "0123456789ABCDEF";

std::string util::ltrim(std::string s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

std::string util::rtrim(std::string s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

std::string util::trim(std::string s) {
	return ltrim(rtrim(s));
}

std::vector<std::string> util::split(std::string str, std::string separator,
		bool includeEmptyElements, bool includeSeparators)
{
	size_t last_index = 0, index;
	std::vector<std::string> components;
	while ((index = str.find_first_of(separator, last_index)) != std::string::npos) {
		if (includeEmptyElements || index - last_index > 0) {
			components.push_back(str.substr(last_index, index - last_index));
		}
		if (includeSeparators) {
			components.push_back(separator);
		}
		last_index = index + separator.length();
	}
	if (includeEmptyElements || str.size() - last_index > 0) {
		components.push_back(str.substr(last_index, str.size() - last_index));
	}
	return components;
}

std::string util::join(std::vector<std::string> list, std::string separator)
{
	std::stringstream ss;
	for (auto i = list.begin(); i != list.end(); i++) {
		if (i != list.begin()) ss << separator;
		ss << *i;
	}
	return ss.str();
}

std::string util::hex_encode(const unsigned char *buf, size_t len, bool byte_swap)
{
	std::string hex;
	if (byte_swap) {
		for (size_t i = len; i > 0; i--) {
			char b = buf[i-1];
			hex.append(HEX_DIGITS + ((b >> 4) & 0x0F), 1);
			hex.append(HEX_DIGITS + (b & 0x0F), 1);
		}
	} else {
		for (size_t i = 0; i < len; i++) {
			char b = buf[i];
			hex.append(HEX_DIGITS + ((b >> 4) & 0x0F), 1);
			hex.append(HEX_DIGITS + (b & 0x0F), 1);
		}
	}
	return hex;
}

void util::hex_decode(std::string hex, unsigned char *buf, size_t len, bool byte_swap)
{
	if (hex.length() % 2 != 0) return;
	if (byte_swap) {
		for (size_t i = hex.length()/2; i > 0; i--) {
			const char tmp[3] = { hex[(i - 1) << 1], hex[((i - 1) << 1) + 1], 0 };
			*buf++ = (char)strtoul(tmp, NULL, 16);
		}
	} else {
		for (size_t i = 0; i < hex.length()/2 && i < len; i++) {
			const char tmp[3] = { hex[i << 1], hex[(i << 1) + 1], 0 };
			*buf++ = (char)strtoul(tmp, NULL, 16);
		}
	}
}

void util::generate_random(unsigned char *buf, size_t len)
{
	static std::default_random_engine generator;
	static std::uniform_int_distribution<unsigned int> distribution(0, 255);
	for (size_t i = 0; i < len; i++) {
		buf[i] = (unsigned char)distribution(generator);
	}
}

void util::generate_uuid(uuid &u)
{
	generate_random(u.data, 16);
	u.val.data3 = (u.val.data3 & 0x0FFF) | 0x4000; /* random uuid */
}

std::string util::format_uuid(uuid &u)
{
	std::stringstream ss;
	ss << hex_encode(&u.data[0], 4, host_endian.value == endian_little);
	ss << "-";
	ss << hex_encode(&u.data[4], 2, host_endian.value == endian_little);
	ss << "-";
	ss << hex_encode(&u.data[6], 2, host_endian.value == endian_little);
	ss << "-";
	ss << hex_encode(&u.data[8], 2, host_endian.value == endian_little);
	ss << "-";
	ss << hex_encode(&u.data[10], 6, false);
	return ss.str();
}
