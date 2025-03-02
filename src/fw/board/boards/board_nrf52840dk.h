#pragma once

#include "services/imu/units.h"
#include "util/size.h"


#define BOARD_LSE_MODE RCC_LSE_Bypass

static const BoardConfig BOARD_CONFIG = {
  .ambient_light_dark_threshold = 150,
  .ambient_k_delta_threshold = 50,
  .photo_en = { },
  .als_always_on = true,

  // JDI LPM013M126 has EXTCOMIN
  .lcd_com = { NRF5_GPIO_RESOURCE_EXISTS, NRF_GPIO_PIN_MAP(0, 6), true },

  .backlight_on_percent = 25,
  .backlight_max_duty_cycle_percent = 67,

  .power_5v0_options = OptionNotPresent,
  .power_ctl_5v0 = { 0 },

  .dbgserial_int = {
    .peripheral = NRFX_GPIOTE_INSTANCE(0),
    .channel = 0,
    .gpio_pin = NRF_GPIO_PIN_MAP(0, 8), // rx
  },

  .has_mic = false,
};

static const BoardConfigButton BOARD_CONFIG_BUTTON = {
  .buttons = {
    [BUTTON_ID_BACK] =
        { "Back",   { NRFX_GPIOTE_INSTANCE(0), 1, NRF_GPIO_PIN_MAP(0, 24) }, NRF_GPIO_PIN_PULLUP },
    [BUTTON_ID_UP] =
        { "Up",     { NRFX_GPIOTE_INSTANCE(0), 2, NRF_GPIO_PIN_MAP(0, 12) }, NRF_GPIO_PIN_PULLUP },
    [BUTTON_ID_SELECT] =
        { "Select", { NRFX_GPIOTE_INSTANCE(0), 3, NRF_GPIO_PIN_MAP(0, 11) }, NRF_GPIO_PIN_PULLUP },
    [BUTTON_ID_DOWN] =
        { "Down",   { NRFX_GPIOTE_INSTANCE(0), 4, NRF_GPIO_PIN_MAP(0, 25) }, NRF_GPIO_PIN_PULLUP },
  },
  .button_com = { 0 },
  .active_high = false,
  .timer = NRFX_TIMER_INSTANCE(1),
};

static const BoardConfigPower BOARD_CONFIG_POWER = {
  .pmic_int = { },
  .pmic_int_gpio = { .gpio_pin = GPIO_Pin_NULL, }, /* TODO */

  .battery_vmon_scale = { /* TODO */
    // Battery voltage is scaled down by a pair of resistors:
    //  - R13 on the top @ 47k
    //  - R15 on the bottom @ 30.1k
    //   (R13 + R15) / R15 = 77.1 / 30.1
    .numerator = 771,
    .denominator = 301,
  },

  .vusb_stat = { .gpio_pin = GPIO_Pin_NULL, },
  .chg_stat = { },
  .chg_fast = { },
  .chg_en = { },
  .has_vusb_interrupt = false,

  .wake_on_usb_power = false,

  .charging_status_led_voltage_compensation = 0,

#if defined(IS_BIGBOARD) && !defined(BATTERY_DEBUG)
  // We don't use the same batteries on all bigboards, so set a safe cutoff voltage of 4.2V.
  // Please do not change this!
  .charging_cutoff_voltage = 4200,
#else
  .charging_cutoff_voltage = 4300,
#endif

  .low_power_threshold = 5,

  // Based on measurements from v4.0-beta16.
  // Typical Connected Current at VBAT without HRM ~520uA
  // Added draw with HRM on : ~1.5mA ==> Average impact (5% per hour + 1 hour continuous / day)
  //    (.05 * 23/24 + 1.0 * 1/24) * 1.5mA = ~134uA
  // Assume ~150uA or so for notifications & user interaction
  // Total Hours = 125 mA * hr / (.520 + .134 + 150)mA = 155 hours
  .battery_capacity_hours = 155 /* TODO */,
};

extern UARTDevice * const DBG_UART;

extern PwmState BACKLIGHT_PWM_STATE;
static const BoardConfigActuator BOARD_CONFIG_BACKLIGHT = {
  .options = ActuatorOptions_Pwm,
  //.ctl = { NRF5_GPIO_RESOURCE_EXISTS, NRF_GPIO_PIN_MAP(1, 8), true },
  .pwm = {
    .state = &BACKLIGHT_PWM_STATE,
    .output = { NRF5_GPIO_RESOURCE_EXISTS, NRF_GPIO_PIN_MAP(0, 13), true },
    .peripheral = NRFX_PWM_INSTANCE(0)
  },
};

static const BoardConfigSharpDisplay BOARD_CONFIG_DISPLAY = {
  .spi = NRFX_SPIM_INSTANCE(3),

  .clk = { NRF5_GPIO_RESOURCE_EXISTS, NRF_GPIO_PIN_MAP(1, 15), true },
  .mosi = { NRF5_GPIO_RESOURCE_EXISTS, NRF_GPIO_PIN_MAP(1, 13), true },
  .cs = { NRF5_GPIO_RESOURCE_EXISTS, NRF_GPIO_PIN_MAP(1, 12), false },

  .on_ctrl = { NRF5_GPIO_RESOURCE_EXISTS, NRF_GPIO_PIN_MAP(1, 11), true },  // reuse this as the DC pin
};

extern const VoltageMonitorDevice * VOLTAGE_MONITOR_ALS;
extern const VoltageMonitorDevice * VOLTAGE_MONITOR_BATTERY;

extern QSPIPort * const QSPI;
extern QSPIFlash * const QSPI_FLASH;

extern MicDevice * const MIC;
