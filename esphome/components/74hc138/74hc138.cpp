#include "74hc138.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ic74hc138 {

static const char *const TAG = "74hc138";

void IC74HC138Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up 74HC138...");
  for (auto *pin : this->addr_) {
    pin->setup();
    pin->pin_mode(gpio::FLAG_OUTPUT);
    pin->digital_write(false);
  }
}
void IC74HC138Component::dump_config() { ESP_LOGCONFIG(TAG, "74HC138:"); }
bool IC74HC138Component::digital_read(uint8_t pin) { return false; }
void IC74HC138Component::digital_write(uint8_t pin, bool value) {
  if (value)
    return;
  // ESP_LOGD(TAG, "WRITE %d -> %d %d %d", pin, (pin & 1) > 0, (pin & 2) > 0, (pin & 4) > 0);
  this->addr_[0]->digital_write(pin & 1);
  this->addr_[1]->digital_write(pin & 2);
  this->addr_[2]->digital_write(pin & 4);
}
void IC74HC138Component::pin_mode(uint8_t pin, gpio::Flags flags) {}
bool IC74HC138Component::write_gpio_() { return true; }
float IC74HC138Component::get_setup_priority() const { return setup_priority::IO; }

void IC74HC138GPIOPin::setup() { pin_mode(flags_); }
void IC74HC138GPIOPin::pin_mode(gpio::Flags flags) { this->parent_->pin_mode(this->pin_, flags); }
bool IC74HC138GPIOPin::digital_read() { return this->parent_->digital_read(this->pin_) != this->inverted_; }
void IC74HC138GPIOPin::digital_write(bool value) { this->parent_->digital_write(this->pin_, value != this->inverted_); }
std::string IC74HC138GPIOPin::dump_summary() const {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%u via IC74HC138", pin_);
  return buffer;
}

}  // namespace ic74hc138
}  // namespace esphome
