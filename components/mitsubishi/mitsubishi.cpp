#define _IR_ENABLE_DEFAULT_ false

#define SEND_MITSUBISHI112 true
#define DECODE_MITSUBISHI112 true

#define SEND_MITSUBISHI_AC true
#define DECODE_MITSUBISHI_AC true

#define SEND_MITSUBISHI136 true
#define DECODE_MITSUBISHI136 true

#include "esphome.h"
#include "ir_Mitsubishi.h"
#include "mitsubishi.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"

namespace esphome
{
    namespace mitsubishi
    {
        // copied from ir_Mitsubishi.cpp
        // Mitsubishi 112 bit A/C
        const uint16_t kMitsubishi112HdrMark = 3450;
        const uint16_t kMitsubishi112HdrSpace = 1696;
        const uint16_t kMitsubishi112BitMark = 450;
        const uint16_t kMitsubishi112OneSpace = 1250;
        const uint16_t kMitsubishi112ZeroSpace = 385;
        const uint32_t kMitsubishi112Gap = kDefaultMessageGap;
        // Total tolerance percentage to use for matching the header mark.
        //const uint8_t  kMitsubishi112HdrMarkTolerance = 5;
        const uint16_t kMitsubishi112StateLength = 14;
        //const uint32_t kMitsubishi112Freq = 38;
        const uint32_t kMitsubishi112Freq = 38000;

        static const char *const TAG = "mitsubishi.climate";

        void MitsubishiClimate::setup()
        {
            climate_ir::ClimateIR::setup();

            this->ac_.on();
            this->ac_.setFan(1);
            this->ac_.setMode(kMitsubishiAcCool);
            this->ac_.setTemp(26);
            //this->ac_.setVane(kMitsubishiAcVaneAuto);
            this->ac_.setSwingV(kMitsubishi112SwingVHighest);
            this->ac_.setSwingH(kMitsubishi112SwingHRight);            
            this->apply_state();
        }

        void MitsubishiClimate::transmit_state()
        {
            this->apply_state();

            uint8_t *message = this->ac_.getRaw();

            sendGeneric(
                this->transmitter_,
                kMitsubishi112HdrMark, kMitsubishi112HdrSpace,
                kMitsubishi112BitMark, kMitsubishi112OneSpace,
                kMitsubishi112BitMark, kMitsubishi112ZeroSpace,
                kMitsubishi112BitMark, kMitsubishi112Gap,
                message, kMitsubishi112StateLength, 
                kMitsubishi112Freq);
        }
        void MitsubishiClimate::set_horizontal_default(const HorizontalDirection direction)
        {
            this->default_h_direction = direction;
        }

        void MitsubishiClimate::set_vertical_default(const VerticalDirection direction)
        {
            this->default_v_direction = direction;
        }

        void MitsubishiClimate::set_verticle_swing(uint8_t position)
        {
            this->ac_.setSwingV(position);
        }

        void MitsubishiClimate::set_horizontal_swing(uint8_t position)
        {
            this->ac_.setSwingH(position);
        }

        void MitsubishiClimate::apply_state()
        {
            if (this->mode == climate::CLIMATE_MODE_OFF)
            {
                this->ac_.off();
            }
            else
            {
                this->ac_.setTemp(this->target_temperature);

                switch (this->mode)
                {
                case climate::CLIMATE_MODE_HEAT_COOL:
                    this->ac_.setMode(kMitsubishi112Auto);
                    break;
                case climate::CLIMATE_MODE_HEAT:
                    this->ac_.setMode(kMitsubishi112Heat);
                    break;
                case climate::CLIMATE_MODE_COOL:
                    this->ac_.setMode(kMitsubishi112Cool);
                    break;
                case climate::CLIMATE_MODE_DRY:
                    this->ac_.setMode(kMitsubishi112Dry);
                    break;
                }

                if (this->fan_mode.has_value())
                {
                    switch (this->fan_mode.value())
                    {
                    case climate::CLIMATE_FAN_AUTO:
                        this->ac_.setFan(kMitsubishiAcFanAuto);
                        break;
                    case climate::CLIMATE_FAN_QUIET:
                        this->ac_.setFan(kMitsubishi112FanMin);
                        break;
                    case climate::CLIMATE_FAN_LOW:
                        this->ac_.setFan(kMitsubishi112FanLow);
                        break;
                    case climate::CLIMATE_FAN_MEDIUM:
                        this->ac_.setFan(kMitsubishi112FanMed);
                        break;
                    case climate::CLIMATE_FAN_HIGH:
                        this->ac_.setFan(kMitsubishi112FanMax);
                        break;
                    }
                }
                switch (this->swing_mode)
                {
                case climate::CLIMATE_SWING_OFF:
                    this->ac_.setSwingV(this->default_v_direction);
                    this->ac_.setSwingH(this->default_h_direction);
                    //this->ac_.setSwingV(kMitsubishi112SwingVMiddle);
                    //this->ac_.setSwingH(kMitsubishi112SwingHMiddle);
                    break;
                case climate::CLIMATE_SWING_VERTICAL:
                    this->ac_.setSwingV(kMitsubishi112SwingVAuto);
                    this->ac_.setSwingH(kMitsubishi112SwingHMiddle);
                    break;
                case climate::CLIMATE_SWING_HORIZONTAL:
                    this->ac_.setSwingV(kMitsubishi112SwingVMiddle);
                    this->ac_.setSwingH(kMitsubishi112SwingHAuto);
                    break;
                case climate::CLIMATE_SWING_BOTH:
                    this->ac_.setSwingV(kMitsubishi112SwingVAuto);
                    this->ac_.setSwingH(kMitsubishi112SwingHAuto);
                    break;
                }
                this->ac_.on();
            }

            ESP_LOGI(TAG, "%s", this->ac_.toString().c_str());
        }

    } // namespace mitsubishi_general
} // namespace esphome
