#pragma once

#include "esphome/core/log.h"
#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/climate_ir/climate_ir.h"

#include "ir_Mitsubishi.h"
#include "esphome/components/ir_remote_base/ir_remote_base.h"

namespace esphome
{
    namespace mitsubishi
    {
/*
        const uint8_t kMitsubishi112SwingVLowest =               0b101; // 0x05
        const uint8_t kMitsubishi112SwingVLow =                  0b100; // 0x04
        const uint8_t kMitsubishi112SwingVMiddle =               0b011; // 0x03
        const uint8_t kMitsubishi112SwingVHigh =                 0b010; // 0x02
        const uint8_t kMitsubishi112SwingVHighest =              0b001; // 0x01
        const uint8_t kMitsubishi112SwingVAuto =                 0b111; // 0x07
        
        const uint8_t kMitsubishi112SwingHLeftMax =              0b0001; // 0x01
        const uint8_t kMitsubishi112SwingHLeft =                 0b0010; // 0x02
        const uint8_t kMitsubishi112SwingHMiddle =               0b0011; // 0x03
        const uint8_t kMitsubishi112SwingHRight =                0b0100; // 0x04
        const uint8_t kMitsubishi112SwingHRightMax =             0b0101; // 0x05
        const uint8_t kMitsubishi112SwingHWide =                 0b1000; // 0x08
        const uint8_t kMitsubishi112SwingHAuto =                 0b1100; // 0x0C
*/
        enum HorizontalDirection {
            HORIZONTAL_DIRECTION_LEFT = kMitsubishi112SwingHLeftMax,
            HORIZONTAL_DIRECTION_MIDDLE_LEFT = kMitsubishi112SwingHLeft,
            HORIZONTAL_DIRECTION_MIDDLE = kMitsubishi112SwingHMiddle,
            HORIZONTAL_DIRECTION_MIDDLE_RIGHT = kMitsubishi112SwingHRight,
            HORIZONTAL_DIRECTION_RIGHT = kMitsubishi112SwingHRightMax,
            HORIZONTAL_DIRECTION_SPLIT = kMitsubishi112SwingHWide,
            HORIZONTAL_DIRECTION_AUTO = kMitsubishi112SwingHAuto
        };
        
        enum VerticalDirection {
            VERTICAL_DIRECTION_AUTO = kMitsubishi112SwingVAuto,
            VERTICAL_DIRECTION_UP = kMitsubishi112SwingVHighest,
            VERTICAL_DIRECTION_MIDDLE_UP = kMitsubishi112SwingVHigh,
            VERTICAL_DIRECTION_MIDDLE = kMitsubishi112SwingVMiddle,
            VERTICAL_DIRECTION_MIDDLE_DOWN = kMitsubishi112SwingVLow,
            VERTICAL_DIRECTION_DOWN = kMitsubishi112SwingVLowest
        };
/*
        enum HorizontalDirection {
            HORIZONTAL_DIRECTION_LEFT = 0x01,
            HORIZONTAL_DIRECTION_MIDDLE_LEFT = 0x02,
            HORIZONTAL_DIRECTION_MIDDLE = 0x03,
            HORIZONTAL_DIRECTION_MIDDLE_RIGHT = 0x04,
            HORIZONTAL_DIRECTION_RIGHT = 0x05,
            HORIZONTAL_DIRECTION_SPLIT = 0x08,
            HORIZONTAL_DIRECTION_AUTO = 0x0C
        };
           
        enum VerticalDirection {
            VERTICAL_DIRECTION_AUTO = 0x07,
            VERTICAL_DIRECTION_UP = 0x01,
            VERTICAL_DIRECTION_MIDDLE_UP = 0x02,
            VERTICAL_DIRECTION_MIDDLE = 0x03,
            VERTICAL_DIRECTION_MIDDLE_DOWN = 0x04,
            VERTICAL_DIRECTION_DOWN = 0x05
        };
*/

        class MitsubishiClimate : public climate_ir::ClimateIR, public ir_remote_base::IrRemoteBase
        {
        public:
        MitsubishiClimate()
                : ClimateIR(16, 30, 1.0f, true, false,
                            {climate::CLIMATE_FAN_AUTO, climate::CLIMATE_FAN_QUIET, climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM, climate::CLIMATE_FAN_HIGH},
                            {climate::CLIMATE_SWING_OFF, climate::CLIMATE_SWING_VERTICAL, climate::CLIMATE_SWING_HORIZONTAL, climate::CLIMATE_SWING_BOTH}) {}
                                
/*
esphome::climate_ir::ClimateIR::ClimateIR	(	
float	minimum_temperature,
float	maximum_temperature,
float	temperature_step = 1.0f,
bool	supports_dry = false,
bool	supports_fan_only = false,
climate::ClimateFanModeMask	fan_modes = climate::ClimateFanModeMask(),
climate::ClimateSwingModeMask	swing_modes = climate::ClimateSwingModeMask(),
climate::ClimatePresetMask	presets = climate::ClimatePresetMask() )
*/
            void setup() override;

            void set_horizontal_default(const HorizontalDirection direction);
            void set_vertical_default(const VerticalDirection direction);

            void set_verticle_swing(uint8_t position);
            void set_horizontal_swing(uint8_t position);

        protected:
            void transmit_state() override;
            void apply_state();

            HorizontalDirection default_h_direction = HORIZONTAL_DIRECTION_MIDDLE;
            VerticalDirection default_v_direction = VERTICAL_DIRECTION_MIDDLE;

            IRMitsubishi112 ac_ = IRMitsubishi112(255); // pin is not used
        };

    } // namespace mitsubishi
} // namespace esphome
