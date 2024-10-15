#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"

// Based off of https://github.com/WifWaf/MCP45HVX1
// TBD: Add license/copyright stuff

namespace esphome {
namespace mcp45hvx1 {

enum Model { MCP45HV31, MCP45HV51 };

const uint8_t TCON_R0HW = 0x08;  // Shutdown Resistor Force
const uint8_t TCON_R0A = 0x04;   // Terminal A Connection
const uint8_t TCON_R0W = 0x02;   // Wiper Connection
const uint8_t TCON_R0B = 0x01;   // Terminal B Connection

const uint8_t GCALL_TCON = 0x60;
const uint8_t GCALL_WIPER = 0x40;
const uint8_t GCALL_WIPERUP = 0x42;
const uint8_t GCALL_WIPERDWN = 0x44;
const uint8_t GCALL_COM_WRITE = 0x02;
const uint8_t GCALL_COM_RWRITE = 0x03;
const uint8_t GCALL_COM_WIPERINC = 0x42;
const uint8_t GCALL_COM_WIPERDEC = 0x44;

const uint8_t MEM_WIPER = 0x00;
const uint8_t MEM_TCON = 0x40;

const uint8_t COM_WRITE = 0x00;
const uint8_t COM_READ = 0x0C;
const uint8_t COM_WIPERINC = 0x04;
const uint8_t COM_WIPERDEC = 0x08;

typedef struct {
  bool R0HW = true;
  bool R0A = true;
  bool R0B = true;
  bool R0W = true;
} TCON_Register;

class MCP45HVX1Component : public Component, public i2c::I2CDevice {
 public:
  MCP45HVX1Component() = default;

  /// Check i2c availability and setup masks
  void setup() override;
  void set_model(Model model) { this->model_ = model; }

  float get_setup_priority() const override;

  void dump_config() override;

  uint8_t readWiper();
  bool writeWiper(uint8_t wiperValue);
  bool incrementWiper(uint8_t incriments = 1);
  bool decrementWiper(uint8_t decriments = 1);

  uint8_t readTCON();
  bool writeTCON();
  bool defaultTCON();

  bool writeTCON_R0HW(bool isOn);
  bool writeTCON_R0A(bool isOn);
  bool writeTCON_R0W(bool isOn);
  bool writeTCON_R0B(bool isOn);

 protected:
  Model model_;
  TCON_Register tcon_reg_;
};

}  // namespace mcp45hvx1
}  // namespace esphome
