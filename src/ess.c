#include <zephyr.h>
#include <stddef.h>

#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include <drivers/sensor.h>

#include <logging/log.h>

LOG_MODULE_REGISTER(ess);

static ssize_t read_temp_ambient(struct bt_conn *conn, const struct bt_gatt_attr *attr,
        void *buf, u16_t len, u16_t offset);
static ssize_t read_temp_die(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, u16_t len, u16_t offset);

/* ESS Declaration */
BT_GATT_SERVICE_DEFINE(ess_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_ESS),
	BT_GATT_CHARACTERISTIC(BT_UUID_TEMPERATURE,
			       BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_temp_ambient, NULL, NULL),
	BT_GATT_CCC(NULL, BT_GATT_PERM_NONE),
	BT_GATT_CHARACTERISTIC(BT_UUID_TEMPERATURE,
			       BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_temp_die, NULL, NULL),
	BT_GATT_CCC(NULL, BT_GATT_PERM_NONE),
);

static ssize_t read_temp_ambient(struct bt_conn *conn, const struct bt_gatt_attr *attr,
        void *buf, u16_t len, u16_t offset)
{
	struct device *dev;
	struct sensor_value ambient_temp;

	dev = device_get_binding(DT_INST_0_MAXIM_MAX31855_LABEL);
	if (dev == NULL) {
		LOG_ERR("Cannot get device %s", DT_INST_0_MAXIM_MAX31855_LABEL);
		return -ENODEV;
	}

	sensor_sample_fetch(dev);
	sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &ambient_temp);

	s16_t temperature = ambient_temp.val1 * 100
			+ ambient_temp.val2 / 10000;

    return bt_gatt_attr_read(conn, attr, buf, len, offset, &temperature,
            sizeof(temperature));
}

static ssize_t read_temp_die(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, u16_t len, u16_t offset)
{
	struct device *dev;
	struct sensor_value die_temp;

	dev = device_get_binding(DT_INST_0_MAXIM_MAX31855_LABEL);
	if (dev == NULL) {
		LOG_ERR("Cannot get device %s", DT_INST_0_MAXIM_MAX31855_LABEL);
		return -ENODEV;
	}

	sensor_sample_fetch(dev);
	sensor_channel_get(dev, SENSOR_CHAN_DIE_TEMP, &die_temp);

	s16_t temperature = die_temp.val1 * 100
				+ die_temp.val2 / 10000;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, &temperature,
	            sizeof(temperature));
}
