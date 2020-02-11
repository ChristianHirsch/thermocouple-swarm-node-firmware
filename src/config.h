#ifdef	DT_ALIAS_GPIO_0_LABEL
#define GPIO_PORT		DT_ALIAS_GPIO_0_LABEL
#define BATTERY_EN_PIN	17	// P0.17
#else
#error "Choose supported GPIO 0 driver"
#endif

#ifdef DT_ADC_0_NAME
#define ADC_DEVICE_NAME         DT_ADC_0_NAME
#define ADC_RESOLUTION          10
#define ADC_GAIN                ADC_GAIN_1_3
#define ADC_REFERENCE           ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME    ADC_ACQ_TIME_DEFAULT
#define BATTERY_SENSE_PIN       NRF_ADC_CONFIG_INPUT_4  // P0.03/GPIO3/ANALOG4
#else
#error "you need to define ADC 0"
#endif

/*
#ifdef CONFIG_SOC_NRF52840_QIAA
#ifdef DT_ALIAS_ADC_0_LABEL
#include <hal/nrf_saadc.h>
#define ADC_DRIVER DT_ALIAS_ADC_0_LABEL
#define BATTERY_SENSE_PIN NRF_SAADC_INPUT_AIN2  // P0.04/AIN2
#else
#error "you need to define ADC 0"
#endif
#endif
*/
