#include "checksum.hpp"

crc_t compute_crc32(const std::string serialized_data) {
  crc_t crc = ::crc_init();
  crc = ::crc_update(crc, serialized_data.c_str(), serialized_data.length());
  crc = ::crc_finalize(crc);
  return crc;
};
