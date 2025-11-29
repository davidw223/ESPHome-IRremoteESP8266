import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate_ir
from esphome.components import ir_remote_base
from esphome.const import CONF_MODEL

AUTO_LOAD = ["climate_ir", "ir_remote_base"]

mitsubishi_ns = cg.esphome_ns.namespace("mitsubishi")
MitsubishiClimate = mitsubishi_ns.class_("MitsubishiClimate", climate_ir.ClimateIR)

HorizontalDirection = mitsubishi_ns.enum("HorizontalDirection")
HorizontalDirections = {
    "LEFT": HorizontalDirection.HORIZONTAL_DIRECTION_LEFT,
    "MIDDLE_LEFT": HorizontalDirection.HORIZONTAL_DIRECTION_MIDDLE_LEFT,
    "MIDDLE": HorizontalDirection.HORIZONTAL_DIRECTION_MIDDLE,
    "MIDDLE_RIGHT": HorizontalDirection.HORIZONTAL_DIRECTION_MIDDLE_RIGHT,
    "RIGHT": HorizontalDirection.HORIZONTAL_DIRECTION_RIGHT,
    "SPLIT": HorizontalDirection.HORIZONTAL_DIRECTION_SPLIT,
    #"AUTO": HorizontalDirection.HORIZONTAL_DIRECTION_AUTO,
}

VerticalDirection = mitsubishi_ns.enum("VerticalDirection")
VerticalDirections = {
    #"AUTO": VerticalDirection.VERTICAL_DIRECTION_AUTO,
    "UP": VerticalDirection.VERTICAL_DIRECTION_UP,
    "MIDDLE_UP": VerticalDirection.VERTICAL_DIRECTION_MIDDLE_UP,
    "MIDDLE": VerticalDirection.VERTICAL_DIRECTION_MIDDLE,
    "MIDDLE_DOWN": VerticalDirection.VERTICAL_DIRECTION_MIDDLE_DOWN,
    "DOWN": VerticalDirection.VERTICAL_DIRECTION_DOWN,
}

CONFIG_SCHEMA = climate_ir.climate_ir_with_receiver_schema(MitsubishiClimate).extend(
    {
        cv.Optional("horizontal_default", default="MIDDLE"): cv.enum(HorizontalDirections),
        cv.Optional("vertical_default", default="MIDDLE"): cv.enum(VerticalDirections),
    }
)

async def to_code(config):
    ir_remote_base.load_ir_remote()

    var = await climate_ir.new_climate_ir(config)
    cg.add(var.set_horizontal_default(config["horizontal_default"]))
    cg.add(var.set_vertical_default(config["vertical_default"]))
