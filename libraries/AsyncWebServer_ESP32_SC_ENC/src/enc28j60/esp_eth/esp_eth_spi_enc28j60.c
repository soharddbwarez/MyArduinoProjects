/****************************************************************************************************************************
  esp_eth_spi_enc28j60.c

  For LwIP ENC28J60 Ethernet in ESP32_SC_ENC (ESP32_S2/S3/C3 + LwIP ENC28J60)

  AsyncWebServer_ESP32_SC_ENC is a library for the LwIP Ethernet ENC28J60 in ESP32_S2/S3/C3 to run AsyncWebServer

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_ENC
  Licensed under GPLv3 license

  Version: 1.8.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.6.3   K Hoang      15/12/2022 Initial porting for ENC28J60 + ESP32_S3. Sync with AsyncWebServer_ESP32_ENC v1.6.3
  1.7.0   K Hoang      19/12/2022 Add support to ESP32_S2_ENC (ESP32_S2 + LwIP ENC28J60)
  1.8.0   K Hoang      20/12/2022 Add support to ESP32_C3_ENC (ESP32_C3 + LwIP ENC28J60)
 *****************************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "esp_eth_enc28j60.h"
#include "driver/spi_master.h"

////////////////////////////////////////

esp_eth_mac_t* enc28j60_new_mac( spi_device_handle_t *spi_handle, int INT_GPIO )
{
  eth_enc28j60_config_t enc28j60_config = ETH_ENC28J60_DEFAULT_CONFIG( *spi_handle );
  enc28j60_config.int_gpio_num = INT_GPIO;

  eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();

  mac_config.smi_mdc_gpio_num  = -1; // ENC28J60 doesn't have SMI interface
  mac_config.smi_mdio_gpio_num = -1;
  // mac_config.rx_task_prio      = 1;

  return esp_eth_mac_new_enc28j60( &enc28j60_config, &mac_config );
}

////////////////////////////////////////

esp_eth_mac_t* enc28j60_begin(int MISO_GPIO, int MOSI_GPIO, int SCLK_GPIO, int CS_GPIO, int INT_GPIO, int SPI_CLOCK_MHZ,
                              int SPI_HOST)
{
  if (ESP_OK != gpio_install_isr_service(0))
    return NULL;

  /* ENC28J60 ethernet driver is based on spi driver */
  spi_bus_config_t buscfg =
  {
    .miso_io_num   = MISO_GPIO,
    .mosi_io_num   = MOSI_GPIO,
    .sclk_io_num   = SCLK_GPIO,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
  };

  if ( ESP_OK != spi_bus_initialize( SPI_HOST, &buscfg, SPI_DMA_CH_AUTO ))
    return NULL;

  spi_device_interface_config_t devcfg =
  {
    .command_bits     = 3,
    .address_bits     = 5,
    .mode             = 0,
    .clock_speed_hz   = SPI_CLOCK_MHZ * 1000 * 1000,
    .spics_io_num     = CS_GPIO,
    .queue_size       = 1,
    .cs_ena_posttrans = enc28j60_cal_spi_cs_hold_time(SPI_CLOCK_MHZ),
  };

  spi_device_handle_t spi_handle = NULL;

  if (ESP_OK != spi_bus_add_device( SPI_HOST, &devcfg, &spi_handle ))
    return NULL;

  return enc28j60_new_mac( &spi_handle, INT_GPIO );
}

////////////////////////////////////////

