import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch, uart
from esphome.const import CONF_SEND_EVERY

DEPENDENCIES = ["uart"]

ns = cg.esphome_ns.namespace('epson_projector')
EpsonProjector = ns.class_('EpsonProjector', switch.Switch, uart.UARTDevice, cg.PollingComponent)

CONFIG_SCHEMA = (
    switch.switch_schema(EpsonProjector, block_inverted=True)
        .extend(uart.UART_DEVICE_SCHEMA)
        .extend(cv.polling_component_schema("10s"))
)


async def to_code(config):
    var = await switch.new_switch(config)

    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    
