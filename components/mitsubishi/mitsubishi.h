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
        class MitsubishiClimate : public climate_ir::ClimateIR, public ir_remote_base::IrRemoteBase
        {
        public:
        MitsubishiClimate()
                : ClimateIR(16, 30, 1.0f, true, false,
                            {climate::CLIMATE_FAN_AUTO, climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM, climate::CLIMATE_FAN_HIGH},
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

        protected:
            void transmit_state() override;
            void apply_state();

            IRMitsubishi112 ac_ = IRMitsubishi112(255); // pin is not used
        };

    } // namespace mitsubishi
} // namespace esphome
