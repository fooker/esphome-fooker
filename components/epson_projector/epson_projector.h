#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace epson_projector {

class EpsonProjector : public switch_::Switch, public uart::UARTDevice, public PollingComponent {
public:
	explicit EpsonProjector();

	void setup() override;
	
	void loop() override;

	void update() override;

	void dump_config() override;

protected:
	virtual void write_state(bool state) override;

	virtual std::string icon();

private:
	void on_power(const bool power);

	void exec(const char* cmd);

	bool target;
	bool state;

	String buffer;
};

}
}
