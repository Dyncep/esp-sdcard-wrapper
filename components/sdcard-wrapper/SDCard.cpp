#include "SDCard.hpp"
#include "esp_log.h"
#include "sdmmc_cmd.h"

constexpr const char *TAG = "SdCardWrapper";

SdCard::SdCard(const SdCardBusConfig &cfg, const std::string &mount_point)
    : mount_point(mount_point), bus_cfg(cfg) {}


SdCard::~SdCard() {
  unmount();
}

esp_err_t SdCard::mount() {
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();

  auto ret = spi_bus_initialize(static_cast<spi_host_device_t>(host.slot),
                                &bus_cfg.spi_bus_cfg, SDSPI_DEFAULT_DMA);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize SPI Bus and mount SdCard");
    return ret;
  }

  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  slot_config.gpio_cs = bus_cfg.cs_pin;
  slot_config.host_id = static_cast<spi_host_device_t>(host.slot);

  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = false,
      .max_files = 5,
      .allocation_unit_size = 16 * 1024};
  ret = esp_vfs_fat_sdspi_mount(mount_point.c_str(), &host, &slot_config,
                                &mount_config, &handle);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to mount SdCard. Make sure SD Card lines have "
                  "pull-up-resistors in place.");
    return ret;
  }

  ESP_LOGI(TAG, "Filesystem mounted");
  sdmmc_card_print_info(stdout, handle);
  return ret;
}

esp_err_t SdCard::unmount() {
  if (handle == nullptr) {
    return ESP_OK;
  }

  auto ret = esp_vfs_fat_sdcard_unmount(mount_point.c_str(), handle);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to unmount SdCard");
    return ret;
  }
  handle = nullptr;
  ESP_LOGI(TAG, "Filesystem unmounted");

  ret = spi_bus_free(static_cast<spi_host_device_t>(SDSPI_DEFAULT_HOST));
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to free SPI bus");
  }
  ESP_LOGI(TAG, "SPI Bus freed");
  return ret;
}
