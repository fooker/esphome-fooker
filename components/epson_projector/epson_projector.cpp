#include "epson_projector.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace epson_projector {
	
EpsonProjector::EpsonProjector() :
	switch_::Switch(),
	uart::UARTDevice(),
	PollingComponent() {
}

void EpsonProjector::setup() {
}

void EpsonProjector::loop() {
	uint8_t c;
	while (this->available() > 0) {
		this->read_byte(&c);
		if (c == ':') {
			goto got_line;
		}

		this->buffer += (char) c;
	}

	return;

got_line:
	this->buffer.trim();

	if (this->buffer == "PWR=00" || this->buffer == "PWR=03") {
	       	this->on_power(false);
       	}

       	if (this->buffer == "PWR=01" || this->buffer == "PWR=02") {
	       	this->on_power(true);
       	}

	this->buffer.clear();
}

void EpsonProjector::update() {
	// Get power state from device
	this->exec("PWR?");
}

void EpsonProjector::dump_config() {
	ESP_LOGCONFIG("epson", "EPSON Projector:");
	this->check_uart_settings(9600);
}

void EpsonProjector::write_state(bool state) {
	this->target = state;
	this->publish_state(this->target);
	
	this->update();
}

std::string EpsonProjector::icon() {
	return "mdi:projector";
}

void EpsonProjector::on_power(const bool power) {
	if (power != this->state) {
		// Power state has changed
		ESP_LOGD("epson", "Power state has changed %d", power);
		
		// Handle sudden power changes (i.e. button usage or power loss)
		if (this->target == this->state) {
			// The target has been reacht and state changed afterwards - using the device state as new target
			this->target = power;
			ESP_LOGD("epson", "Following button: %d", this->target);
		}

		// Remember changed state
		this->state = power;
	}

	// Instruct device to change power state
	if (this->state != this->target) {
		this->exec(this->target ? "PWR ON" : "PWR OFF");
	}

	this->publish_state(this->target);
}

void EpsonProjector::exec(const char* cmd) {
	ESP_LOGD("epson", "Sending command: %s", cmd);

	this->write_str(cmd);
	this->write_str("\r");
	this->flush();
}


}
}

