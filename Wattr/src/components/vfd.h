#include <math.h>
#include <ASF/thirdparty/CMSIS/Include/arm_math.h>


#ifndef VFD_H_
#define VFD_H_



void vfd_write(uint8_t data);
void vfd_write_string(char data[]);
void vfd_cr(void);
void vfd_home(void);
void vfd_lf(void);
void vfd_init(void);
void vfd_clear(void);

void vfd_cursor_on(void);
void vfd_cursor_off(void);

void vfd_select_base_window(void);

void vfd_gui_splash(char date[], char time[]);
void vfd_gui_vaf_aar(void);
void vfd_gui_trig(void);
void vfd_gui_cost(void);
void vfd_gui_config(void);

void vfd_set_cursor(uint8_t xL, uint8_t xH, uint8_t yL, uint8_t yH);

extern volatile char* global_voltage_string;
extern volatile char* global_current_string;
extern volatile char* global_frequency_string;
extern volatile char* global_active_power_string;
extern volatile char* global_apparent_power_string;
extern volatile char* global_reactive_power_string;
extern volatile char* global_power_factor_string;
extern volatile char* global_phase_angle_string;
extern volatile int32_t  epoch;
extern volatile float32_t global_watt_hours;
extern volatile int32_t global_watt_hours_epoch;

#endif