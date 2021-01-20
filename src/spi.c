/**
 * @file spi.c
 * @author Bernhard Kraemer
 * @brief SPI sample, only tx.
 * @version 0.1
 * @date 2021-01-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/*** Imported Objects ***************************************************************/
#include <zephyr.h>
#include <logging/log.h>

#include <drivers/spi.h>
#include <globals.h>

/*** Constants **********************************************************************/
#define LOG_MODULE_NAME spi

#define START_DELAY     500
#define STACK_SIZE      500
#define PRIORITY        -5

#define SPI_DEV         DT_LABEL(DT_NODELABEL(lpspi3))

/*** Types **************************************************************************/


/*** Variables **********************************************************************/


/*** Function Prototypes ************************************************************/
void spi_main_f();
static int spi_send(const struct device *spi,
			  const struct spi_config *spi_cfg,
			  const uint8_t *data, size_t len);
static int spi_send_str(const struct device *spi,
			      const struct spi_config *spi_cfg,
			      const unsigned char *str);

/*** Macros *************************************************************************/
LOG_MODULE_REGISTER(LOG_MODULE_NAME, LOG_LEVEL_DBG);

K_THREAD_DEFINE(spi_id, STACK_SIZE, spi_main_f, NULL, NULL, NULL,
                PRIORITY, 0, START_DELAY);

/*** Function implementation ********************************************************/

static int spi_send(const struct device *spi,
			  const struct spi_config *spi_cfg,
			  const uint8_t *data, size_t len)
{
	const struct spi_buf_set tx = {
		.buffers = &(const struct spi_buf){
			.buf = (uint8_t *)data,
			.len = len,
		},
		.count = 1,
	};

	return spi_write(spi, spi_cfg, &tx);
}

static int spi_send_str(const struct device *spi,
			      const struct spi_config *spi_cfg,
			      const unsigned char *str)
{
	return spi_send(spi, spi_cfg, str, strlen(str));
}

void spi_main_f()
{
    int ret;
    const struct device *spi;
    struct spi_config cfg;

    spi = device_get_binding(SPI_DEV);
    if (spi == NULL)
    {
        LOG_ERR("Device binding is NULL");
        return;
    }

    cfg.frequency = 10000U;
    cfg.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_OP_MODE_MASTER;
    cfg.cs = NULL;

    LOG_INF("Thread started");
    
    while (1)
    {
        ret = spi_send_str(spi, &cfg, "Lorem ipsum dolor sit amet");
        if (ret != 0)
        {
            LOG_ERR("Failed to write %s, Error: %u", SPI_DEV, ret);
            return;
        }

        k_sleep(K_MSEC(5000));
    }
}
