#include "utils.hpp"

crc_t compute_crc32(const std::string serialized_data) {
  crc_t crc = crc_init();
  crc = crc_update(crc, serialized_data.c_str(), serialized_data.length());
  crc = crc_finalize(crc);
  return crc;
};

int millis_since_epoch() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}
