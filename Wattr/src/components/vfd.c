/*
 * vfd.c
 *
 * Created: 03 Dec 2013 12:20:27 AM
 *  Author: Andrew
 */ 
#include <asf.h>
#include <vfd.h>

void vfd_write(uint8_t data) {
	uint8_t i = 1;
	for (i = 0; i < 8; i++) {
		ioport_set_pin_level(VFD_SCK, true);
		delay_us(100);

		if (data & (1 << i)) {
			ioport_set_pin_level(VFD_MOSI, false);
			} else {
			ioport_set_pin_level(VFD_MOSI, true);
		}
		ioport_set_pin_level(VFD_SCK, false);
		delay_us(100);
	}
	delay_us(100);
}

void vfd_write_string(char data[]) {
	int index = 0;
	for (;;) {
		if (data[index] == '\0') {
			return;
		} else {
			vfd_write(data[index]);
			index++;
		}
	}
}

void vfd_cr(void) {
	vfd_write(0x0D);
}

void vfd_home(void) {
	vfd_write(0x0B);
}

void vfd_lf(void) {
	vfd_write(0x0A);
}

void vfd_init(void) {
	ioport_set_pin_level(VFD_NRST, true);
	delay_ms(100);
	ioport_set_pin_level(VFD_NRST, false);
	
	delay_ms(500);
	
	vfd_write(0x1B);
	delay_ms(100);
	vfd_write('@');
	delay_ms(100);

//	vfd_cursor_on();
	vfd_select_base_window();
}

void vfd_clear(void) {
	vfd_write(0x0C);
}

void vfd_cursor_on(void) {
	vfd_write(0x1F);
	vfd_write(0x43);
	vfd_write(0x01);
}

void vfd_cursor_off(void) {
	vfd_write(0x1F);
	vfd_write(0x43);
	vfd_write(0x00);
}

void vfd_select_base_window(void) {
	vfd_write(0x1F);
	vfd_write(0x28);
	vfd_write(0x77);
	vfd_write(0x10);
	vfd_write(0x01);
}

void vfd_set_cursor(uint8_t xL, uint8_t xH, uint8_t yL, uint8_t yH) {
	vfd_write(0x1F);
	vfd_write(0x24);
	vfd_write(xL);
	vfd_write(xH);
	vfd_write(yL);
	vfd_write(yH);
}

void vfd_gui_splash(char date[], char time[]) {
	vfd_home();
	
	vfd_write_string("  ");
	vfd_write_string("--- WATTR ---");
	
	vfd_lf();
	vfd_cr();
	
	vfd_write_string("                 ");
		
	vfd_lf();
	vfd_cr();

	vfd_write_string("  ");
	vfd_write_string(date);

	vfd_lf();
	vfd_cr();

	vfd_write_string("       ");
	vfd_write_string(time);
}



void vfd_gui_vaf_aar(void) {
	vfd_home();
	
	char* time_string = malloc(32);
	sprintf(time_string, "    %d", epoch);
	vfd_write_string(time_string);
	
	free(time_string);
	vfd_lf();
	vfd_cr();
	
	vfd_write_string(global_voltage_string);
	vfd_write_string("V   ");
	
	vfd_lf();
	vfd_cr();
	
	vfd_write_string(global_current_string);
	vfd_write_string("A  ");
	
	vfd_lf();
	vfd_cr();

	vfd_write_string(global_frequency_string);
	vfd_write_string("Hz   ");
	
	vfd_set_cursor(9*8, 0, 1, 0);
	
	vfd_write_string(global_active_power_string);
	vfd_write_string("W     ");
	
	vfd_set_cursor(9*8, 0, 2, 0);
	
	vfd_write_string(global_apparent_power_string);
	vfd_write_string("VA     ");
	
	vfd_set_cursor(9*8, 0, 3, 0);
	
	vfd_write_string(global_reactive_power_string);
	vfd_write_string("VAR    ");
}

void vfd_gui_trig(void) {
	vfd_home();
	
	char* time_string = malloc(32);
	sprintf(time_string, "    %d", epoch);
	vfd_write_string(time_string);
	
	free(time_string);
	vfd_lf();
	vfd_cr();
	
	vfd_write_string("Phase Angle: ");
	vfd_write_string(global_phase_angle_string);
	vfd_write_string("          ");
	
	vfd_lf();
	vfd_cr();
	
	vfd_write_string("Power Factor:");
	
	
	vfd_lf();
	vfd_cr();
	
	vfd_write_string("     ");
	vfd_write_string(global_power_factor_string);
	vfd_write_string("     ");
}

void vfd_gui_config(void) {
	vfd_home();
	
	vfd_write_string("MAC Address:");
	vfd_lf();
	vfd_cr();
	
	vfd_write_string(" DE:AD:BE:EF:CA:FE  ");	
	vfd_lf();
	vfd_cr();
	
	vfd_write_string("DHCP: N/A             ");
	vfd_lf();
	vfd_cr();
	
	vfd_write_string("SD Card: N/A               ");
}

void vfd_gui_cost(void) {
	char* total_energy_string = malloc(32);
	char* time_delta_string = malloc(32);
	char* total_energy_cost_string = malloc(32);
	
	float32_t global_watt_hours_fixed = global_watt_hours / 1000;
	
	total_energy_string = gcvtf(global_watt_hours_fixed, 5, total_energy_string);
	
	int32_t time_delta = epoch - global_watt_hours_epoch;
	
	sprintf(time_delta_string, "%d", time_delta);
	
	
	float32_t energy_cost = global_watt_hours_fixed;
	energy_cost *= 0.0013f; // 13 cents/kWh in cents/Wh
	total_energy_cost_string = gcvtf(energy_cost, 5, total_energy_cost_string);
	
	vfd_home();
	
	vfd_write_string("Energy for ");
	vfd_write_string(time_delta_string);
	vfd_write_string("s:");
	
	vfd_lf();
	vfd_cr();
	
	vfd_write_string("  ");
	vfd_write_string(total_energy_string);
	vfd_write_string("Wh     ");
	
	vfd_lf();
	vfd_cr();
	
	vfd_write_string("13 cents per kWh:");
		
	vfd_lf();
	vfd_cr();
	
	vfd_write_string("  $");
	vfd_write_string(total_energy_cost_string);
	vfd_write_string("      ");
	
	free(total_energy_string);
	free(time_delta_string);
	free(total_energy_cost_string);
}