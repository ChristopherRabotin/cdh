#ifndef CHECKSUM_HPP_INCLUDED
#define CHECKSUM_HPP_INCLUDED

#include "crc32.h"
#include <chrono>
#include <string>

crc_t compute_crc32(const std::string serialized_data);
int millis_since_epoch();

#endif
