#ifndef SD_CARD_WRAPPER_HPP
#define SD_CARD_WRAPPER_HPP

#include "esp_vfs_fat.h"
#include <string>
#include <vector>

/**
 * struct BusConfig - used for configuring the spi bus
 */
struct SdCardBusConfig {
  /**
   * @spi_bus_cfg spi bus config
   */
  spi_bus_config_t spi_bus_cfg = {};

  /**
   * @cs_pin the cs pin
   */
  gpio_num_t cs_pin = {};
};

class SdCard {
public:
  /**
   * @brief constructor
   *
   * @param bus_cfg - the spi bus config
   * @param mount_point - the mount point for the sd card
   */
  SdCard(const SdCardBusConfig &cfg, const std::string &mount_point);

  /**
   * @brief destructor, calls unmount()
   */
  ~SdCard();

  /**
   * @brief mount the sd card
   *
   * @return - esp_err_t
   */
  esp_err_t mount();

  /**
   * @brief unmount the sd card
   *
   * @return - esp_err_t
   */
  esp_err_t unmount();

  /**
   * @brief retrieve the sd card handle
   *
   * @return sdmmc_card_t*
   */
  sdmmc_card_t *getHandle() { return handle; }

private:
  /**
   * @handle the underlying handle of the sd card
   */
  sdmmc_card_t *handle = nullptr;

  /**
   * @mount_point the mount point / path of the sd card
   */
  std::string mount_point = {};

  /**
   * @bus_cfg the spi bus config of the sd card
   */
  SdCardBusConfig bus_cfg = {};
};

#endif
