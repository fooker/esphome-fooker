#include "esphome.h"

class EpsonProjectorComponent : public PollingComponent, public UARTDevice, public Switch {
public:
	EpsonProjectorComponent(UARTComponent* parent) :
		PollingComponent(5000),
		UARTDevice(parent),
		Switch("target") {
	}

	void setup() override {
	}

	void loop() override {
		if (this->available()) {
			String line = this->readStringUntil(':');
			line.trim();

			ESP_LOGD("epson", "Got line '%s'", line.c_str());

			if (line == "PWR=00" || line == "PWR=03") {
				ESP_LOGD("epson", "Power off");
				this->on_power(false);
			}

			if (line == "PWR=01" || line == "PWR=02") {
				ESP_LOGD("epson", "Power on");
				this->on_power(true);
			}
		}
	}

	void update() override {
		// Get power state from device
		this->exec("PWR?");
	}

	void dump_config() override {
		ESP_LOGCONFIG("epson", "EPSON Projector:");
		this->check_uart_settings(9600);
	}

protected:
	virtual void write_state(bool state) override {
		this->target = state;
		this->publish_state(this->target);
		
		this->update();
	}

	virtual std::string icon() {
		return "mdi:projector";
	}

private:
	void on_power(const bool power) {
		ESP_LOGD("epson", "Switching to %d having %d targeting %d", power, this->state, this->target);

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

	void exec(const char* cmd) {
		ESP_LOGD("epson", "Sending command: %s", cmd);

		this->write_str(cmd);
		this->write_str("\r");
		this->flush();
	}

	bool target;
	bool state;
};

