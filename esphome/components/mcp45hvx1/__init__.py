import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_MODEL

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

mcp45hvx1_ns = cg.esphome_ns.namespace("mcp45hvx1")
MCP45HVX1Model = mcp45hvx1_ns.enum("Model")

MCP45HVX1_MODEL_OPTIONS = {
    "MCP45HV31": MCP45HVX1Model.MCP45HV31,
    "MCP45HV51": MCP45HVX1Model.MCP45HV51,
}

MCP45HVX1Component = mcp45hvx1_ns.class_(
    "MCP45HVX1Component", cg.Component, i2c.I2CDevice
)


CONF_MCP45HVX1 = "mcp45hvx1"
CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.declare_id(MCP45HVX1Component),
            cv.Required(CONF_MODEL): cv.enum(MCP45HVX1_MODEL_OPTIONS),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x3C))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_model(config[CONF_MODEL]))
