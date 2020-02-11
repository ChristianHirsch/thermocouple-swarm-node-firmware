#include <device.h>
#include <zephyr/types.h>

s8_t battery_init(struct device *);
u16_t battery_read_value(void);
