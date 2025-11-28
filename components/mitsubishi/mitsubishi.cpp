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

namespace esphome
{
    namespace panasonic
    {
        // copied from ir_Mitsubishi.cpp

        /*
        const uint16_t kPanasonicHdrMark = 3456;
        const uint16_t kPanasonicHdrSpace = 1728;
        const uint16_t kPanasonicBitMark = 432;
        const uint16_t kPanasonicOneSpace = 1296;
        const uint16_t kPanasonicZeroSpace = 432;
        const uint16_t kPanasonicAcSectionGap = 10000;
        const uint16_t kPanasonicAcSection1Length = 8;
        const uint32_t kPanasonicAcMessageGap = kDefaultMessageGap;        
        */

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

        static const char *const TAG = "mitsubishi.climate";

        void MitsubishiClimate::setup()
        {
            climate_ir::ClimateIR::setup();
            this->apply_state();
        }

        void MitsubishiClimate::transmit_state()
        {
            this->apply_state();

            uint8_t *message = this->ac_.getRaw();
            uint16_t repeat = 1;

            sendGeneric(
                this->transmitter_,
                kMitsubishi112HdrMark, kMitsubishi112HdrSpace,
                kMitsubishi112BitMark, kMitsubishi112OneSpace,
                kMitsubishi112BitMark, kMitsubishi112ZeroSpace,
                kMitsubishi112BitMark, kMitsubishi112Gap,
                message, kMitsubishi112StateLength, 38, false, repeat, 50);
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
                // case climate::CLIMATE_MODE_FAN_ONLY:
                //     this->ac_.setMode(kPanasonicAcFan);
                //     break;
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
/*
const uint8_t kMitsubishi112SwingVLowest =               0b101;
const uint8_t kMitsubishi112SwingVLow =                  0b100;
const uint8_t kMitsubishi112SwingVMiddle =               0b011;
const uint8_t kMitsubishi112SwingVHigh =                 0b010;
const uint8_t kMitsubishi112SwingVHighest =              0b001;
const uint8_t kMitsubishi112SwingVAuto =                 0b111;

const uint8_t kMitsubishi112SwingHLeftMax =              0b0001;
const uint8_t kMitsubishi112SwingHLeft =                 0b0010;
const uint8_t kMitsubishi112SwingHMiddle =               0b0011;
const uint8_t kMitsubishi112SwingHRight =                0b0100;
const uint8_t kMitsubishi112SwingHRightMax =             0b0101;
const uint8_t kMitsubishi112SwingHWide =                 0b1000;
const uint8_t kMitsubishi112SwingHAuto =                 0b1100;
*/
                switch (this->swing_mode)
                {
                case climate::CLIMATE_SWING_OFF:
                    this->ac_.setSwingVertical(kMitsubishi112SwingVMiddle);
                    this->ac_.setSwingHorizontal(kMitsubishi112SwingHMiddle);
                    break;
                case climate::CLIMATE_SWING_VERTICAL:
                    this->ac_.setSwingVertical(kMitsubishi112SwingVAuto);
                    this->ac_.setSwingHorizontal(kMitsubishi112SwingHMiddle);
                    break;
                case climate::CLIMATE_SWING_HORIZONTAL:
                    this->ac_.setSwingVertical(kMitsubishi112SwingVMiddle);
                    this->ac_.setSwingHorizontal(kMitsubishi112SwingHAuto);
                    break;
                case climate::CLIMATE_SWING_BOTH:
                    this->ac_.setSwingVertical(kMitsubishi112SwingVAuto);
                    this->ac_.setSwingHorizontal(kMitsubishi112SwingHAuto);
                    break;
                }

                this->ac_.on();
            }

            ESP_LOGI(TAG, "%s", this->ac_.toString().c_str());
        }

    } // namespace mitsubishi_general
} // namespace esphome
