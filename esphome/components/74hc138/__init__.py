from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_ADDRESS,
    CONF_ID,
    CONF_INVERTED,
    CONF_MODE,
    CONF_NUMBER,
    CONF_OUTPUT,
    CONF_PIN,
)

MULTI_CONF = True

ic74hc138_ns = cg.esphome_ns.namespace("ic74hc138")

IC74HC138Component = ic74hc138_ns.class_("IC74HC138Component", cg.Component)
IC74HC138GPIOPin = ic74hc138_ns.class_("IC74HC138GPIOPin", cg.GPIOPin)

CONF_74HC138 = "74hc138"
CONFIG_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ID): cv.declare_id(IC74HC138Component),
        cv.Required(CONF_ADDRESS): cv.All(
            cv.ensure_list({cv.Required(CONF_PIN): pins.gpio_output_pin_schema}),
            cv.Length(min=3, max=3),
        ),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    addr_pins = []
    for addr in config[CONF_ADDRESS]:
        pin = await cg.gpio_pin_expression(addr[CONF_PIN])
        addr_pins.append(pin)
    cg.add(var.set_addr(addr_pins))


def validate_mode(value):
    return {CONF_OUTPUT: True}


IC74HC138_PIN_SCHEMA = pins.gpio_base_schema(
    IC74HC138GPIOPin,
    cv.int_range(min=0, max=8),
    modes=[CONF_OUTPUT],
    mode_validator=validate_mode,
    invertable=True,
).extend(
    {
        cv.Required(CONF_74HC138): cv.use_id(IC74HC138Component),
    }
)


@pins.PIN_SCHEMA_REGISTRY.register(CONF_74HC138, IC74HC138_PIN_SCHEMA)
async def ic74hc138_pin_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    parent = await cg.get_variable(config[CONF_74HC138])

    cg.add(var.set_parent(parent))

    num = config[CONF_NUMBER]
    cg.add(var.set_pin(num))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    cg.add(var.set_flags(pins.gpio_flags_expr(config[CONF_MODE])))
    return var
