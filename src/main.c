/*
 *
 */

#include <zephyr.h>
#include <zephyr/types.h>
#include <misc/byteorder.h>
#include <logging/log.h>
#include <settings/settings.h>
#include <device.h>
#include <gpio.h>
#include <soc.h>

#include <stddef.h>
#include <string.h>
#include <errno.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>

#include "config.h"

#include "battery.h"

#ifndef IBEACON_RSSI
#define IBEACON_RSSI 0xc8
#endif

#define SENSE_INTERVAL 5
#define MEASUREMENTS_SIZE  10

/* in microseconds */
#define MIN_PERIOD	(USEC_PER_SEC / 64U)

/* in microseconds */
#define MAX_PERIOD	USEC_PER_SEC

// BT_GAP_ADV_SLOW_INT_MIN = 0x0640 ^= 1s
// BT_GAP_ADV_SLOW_INT_MAX = 0x0780 ^= 1.2s
#define BT_LE_ADV_CONN_NAME_ID BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONNECTABLE | \
                         BT_LE_ADV_OPT_USE_NAME | BT_LE_ADV_OPT_USE_IDENTITY, \
                         BT_GAP_ADV_SLOW_INT_MIN, \
                         BT_GAP_ADV_SLOW_INT_MAX)

LOG_MODULE_REGISTER(main);

static const struct bt_data ad[] = {
		BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
		BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xaa, 0xfe),
		BT_DATA_BYTES(BT_DATA_SVC_DATA16,
				0xaa, 0xfe,		/* Eddystone UUID */
				0x10,			/* Eddystone-URL frame type */
				0x04,			/* Calibrated Tx power at 0m */
				0x02,			/* URL Scheme Prefix http:// */
				'b','r','e','w','c','e','.',
				'c','h','r','i','v','i','e','h',
				0x03)
};

static void connected(struct bt_conn *conn, u8_t err)
{
	if (err) {
		LOG_ERR("Connection failed (err %u)\n", err);
		return;
	}

	bt_le_adv_stop();
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
	LOG_INF("Disconnected (reason %u)\n", reason);

	int err = bt_le_adv_start(BT_LE_ADV_CONN_NAME_ID, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)", err);
		return;
	}
    LOG_INF("Advertising successfully started");
}

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};

static void bt_ready(int err)
{
	if (err) {
		LOG_ERR("Failed to enable bluetooth (err %d)", err);
		return;
	}

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME_ID, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)", err);
		return;
	}
}

void main(void)
{
	int err = 0;

	err = bt_enable(bt_ready);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return;
	}
    bt_conn_cb_register(&conn_callbacks);

    err = battery_init(NULL);
    if (err != 0) {
    	LOG_ERR("Battery init failed (err %d", err);
    }

	while (1) {
		k_sleep(K_FOREVER);
	}
}
