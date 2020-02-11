#include <bluetooth/uuid.h>

/**
 * UUID (v1):  77e1XXXX-37af-11ea-a7a1-507b9de20712
 */

#define BT_UUID_BASE	BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x77e10000, 0x37af, 0x11ea, 0xa7a1, 0x507b9de20712))

/** @def BT_UUID_COS
 *  @brief Cooker Service
 */
#define BT_UUID_COS						BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x77e11801, 0x37af, 0x11ea, 0xa7a1, 0x507b9de20712))

/** @def BT_UUID_COS_OUTPUT_POWER
 *  @brief COS Characteristic Output Power State
 */
#define BT_UUID_COS_TEMPERATURE			BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x77e12a05, 0x37af, 0x11ea, 0xa7a1, 0x507b9de20712))
