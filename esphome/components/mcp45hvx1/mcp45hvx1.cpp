#include "mcp45hvx1.h"
#include "esphome/core/log.h"

// Based off of https://github.com/WifWaf/MCP45HVX1
// TBD: Add license/copyright stuff

namespace esphome {
namespace mcp45hvx1 {

static const char *const TAG = "mcp45hvx1";

void MCP45HVX1Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up MCP45HVX1...");
  // if (!this->read_gpio_()) {
  if (false) {
    ESP_LOGE(TAG, "MCP45HVX1 not available under 0x%02X", this->address_);
    this->mark_failed();
    return;
  }

  // this->write_gpio_();
  // this->read_gpio_();
}

void MCP45HVX1Component::dump_config() {
  ESP_LOGCONFIG(TAG, "MCP45HVX1:");
  LOG_I2C_DEVICE(this)
  // ESP_LOGCONFIG(TAG, "  Is PCF8575: %s", YESNO(this->pcf8575_));
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with MCP45HVX1 failed!");
  }
}

bool MCP45HVX1Component::writeWiper(uint8_t wiperValue) {
  ESP_LOGD(TAG, "writeWiper(%d)", wiperValue);
  uint8_t data[2];
  data[0] = (MEM_WIPER | COM_WRITE);
  data[1] = wiperValue;

  if (this->write(data, 2) != i2c::ERROR_OK) {
    this->status_set_warning();
    return false;
  }

  this->status_clear_warning();
  return true;
}

uint8_t MCP45HVX1Component::readWiper() {
  ESP_LOGD(TAG, "readWiper()");
  uint8_t data[2];
  data[0] = (MEM_WIPER | COM_READ);

  this->write(data, 1, false);
  this->read(data, 2);

  ESP_LOGD(TAG, " -> %d", data[1]);

  return data[1];
}

bool MCP45HVX1Component::incrementWiper(uint8_t increments) {
  uint8_t data[1];
  data[0] = (MEM_WIPER | COM_WIPERINC);
  for (uint8_t x = 0; x < increments; x++) {
    if (this->write(data, 1) != i2c::ERROR_OK) {
      this->status_set_warning();
      return false;
    }
  }

  this->status_clear_warning();
  return true;
}

bool MCP45HVX1Component::decrementWiper(uint8_t decrements) {
  uint8_t data[1];
  data[0] = (MEM_WIPER | COM_WIPERDEC);
  for (uint8_t x = 0; x < decrements; x++) {
    if (this->write(data, 1) != i2c::ERROR_OK) {
      this->status_set_warning();
      return false;
    }
  }

  this->status_clear_warning();
  return true;
}

uint8_t MCP45HVX1Component::readTCON() {
  ESP_LOGD(TAG, "readTCON()");
  uint8_t data[2];

  data[0] = (MEM_TCON | COM_READ);
  this->write(data, 1, false);
  this->read(data, 2);

  ESP_LOGD(TAG, " -> %X", data[1]);

  return data[1];
}

bool MCP45HVX1Component::defaultTCON() {
  ESP_LOGD(TAG, "defaultTCON()");
  this->tcon_reg_.R0HW = true;
  this->tcon_reg_.R0A = true;
  this->tcon_reg_.R0B = true;
  this->tcon_reg_.R0W = true;

  return writeTCON();
}

bool MCP45HVX1Component::writeTCON_R0HW(bool isOn) {
  ESP_LOGD(TAG, "writeTCON_R0HW(%d)", isOn);
  this->tcon_reg_.R0HW = isOn;
  return writeTCON();
}

bool MCP45HVX1Component::writeTCON_R0A(bool isOn) {
  ESP_LOGD(TAG, "writeTCON_R0A(%d)", isOn);
  this->tcon_reg_.R0A = isOn;
  return writeTCON();
}

bool MCP45HVX1Component::writeTCON_R0W(bool isOn) {
  ESP_LOGD(TAG, "writeTCON_R0W(%d)", isOn);
  this->tcon_reg_.R0W = isOn;
  return writeTCON();
}

bool MCP45HVX1Component::writeTCON_R0B(bool isOn) {
  ESP_LOGD(TAG, "writeTCON_R0B(%d)", isOn);
  this->tcon_reg_.R0B = isOn;
  return writeTCON();
}

bool MCP45HVX1Component::writeTCON() {
  ESP_LOGD(TAG, "writeTCON()");
  uint8_t flags = 0xFF;

  this->tcon_reg_.R0HW ? (flags |= TCON_R0HW) : (flags ^= TCON_R0HW);
  this->tcon_reg_.R0A ? (flags |= TCON_R0A) : (flags ^= TCON_R0A);
  this->tcon_reg_.R0B ? (flags |= TCON_R0B) : (flags ^= TCON_R0B);
  this->tcon_reg_.R0W ? (flags |= TCON_R0W) : (flags ^= TCON_R0W);

  ESP_LOGD(TAG, " flags: %X", flags);

  uint8_t data[2];
  data[0] = (MEM_TCON | COM_WRITE);
  data[1] = flags;

  return this->write(data, 2);
}

float MCP45HVX1Component::get_setup_priority() const { return setup_priority::HARDWARE; }

}  // namespace mcp45hvx1
}  // namespace esphome
