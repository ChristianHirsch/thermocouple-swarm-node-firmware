#include <zephyr.h>
#include <errno.h>

#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include "battery.h"

#define MAX_BAT_LVL 0x0350
#define MIN_BAT_LVL 0x02c5

static ssize_t read_blvl(struct bt_conn *conn, const struct bt_gatt_attr *attr,
		void *buf, u16_t len, u16_t offset);

/* Battery Service Declaration */
BT_GATT_SERVICE_DEFINE(bas_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_BAS),
	BT_GATT_CHARACTERISTIC(BT_UUID_BAS_BATTERY_LEVEL,
			BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_blvl, NULL, NULL),
	BT_GATT_CCC(NULL, BT_GATT_PERM_NONE)
);


static ssize_t read_blvl(struct bt_conn *conn, const struct bt_gatt_attr *attr,
		void *buf, u16_t len, u16_t offset)
{
	u16_t value = battery_read_value();
	if (value > MAX_BAT_LVL) {
		value = MAX_BAT_LVL;
	} else if (value < MIN_BAT_LVL) {
		value = MIN_BAT_LVL;
	}

	value  = (value - MIN_BAT_LVL) * 100;
	value /= (MAX_BAT_LVL - MIN_BAT_LVL);

	u8_t battery_value = (u8_t) value;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, &battery_value,
			sizeof(battery_value));
}
