### ESP-IDF SdCard Wrapper class

A simple wrapper for using a sd card in the esp idf.
This project is not being actively maintained and has not been thoroughly tested.

## Examples

You may refer to the Main.cpp file in the main/ directory.

```cpp
#include "SDCard.hpp"
#include <filesystem>
#include <iostream>

#define PIN_NUM_MISO GPIO_NUM_19
#define PIN_NUM_MOSI GPIO_NUM_23
#define PIN_NUM_CLK GPIO_NUM_18
#define PIN_NUM_CS GPIO_NUM_5

extern "C" void app_main(void) {
  SdCard *sd_card = nullptr;
  SdCardBusConfig sd_card_cfg{};
  sd_card_cfg.spi_bus_cfg = {
      .mosi_io_num = PIN_NUM_MOSI,
      .miso_io_num = PIN_NUM_MISO,
      .sclk_io_num = PIN_NUM_CLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 4000,
  };

  sd_card_cfg.cs_pin = PIN_NUM_CS;
  sd_card = new SdCard(std::move(sd_card_cfg), "/sdcard");
  sd_card->mount();

  if(std::filesystem::exists({"/sdcard"})) {
    std::cout << "Mounted root directory exists" << std::endl;
  } else {
    std::cout << "Mounted root directory does not exist" << std::endl;
  }
  sd_card->unmount();
}
```
