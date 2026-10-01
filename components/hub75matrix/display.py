from esphome.components import display
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

hub75_ns = cg.esphome_ns.namespace("hub75matrix")
HUB75MatrixDisplay = hub75_ns.class_("HUB75MatrixDisplay", cg.PollingComponent, display.DisplayBuffer)

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(HUB75MatrixDisplay),
}).extend(cv.polling_component_schema("50ms"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)