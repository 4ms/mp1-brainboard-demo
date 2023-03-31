#pragma once
#include "brain_conf.hh"
#include "brain_pinout.hh"
#include "drivers/adc_builtin_conf.hh"
#include "drivers/debounced_switch.hh"
#include "drivers/pin.hh"
#include "drivers/rgbled.hh"
#include "drivers/tim_pwm.hh"
#include "elements.hh"
#include <array>

namespace Mp1BrainDemo::Board
{

using enum mdrivlib::PinPolarity;
using enum mdrivlib::PinMode;

using ActionButton = mdrivlib::DebouncedButton<Brain::Pin::D12, Inverted>;

using SyncAJack = mdrivlib::DebouncedPin<Brain::Pin::D13, Normal>;
using SyncBJack = mdrivlib::DebouncedPin<Brain::Pin::D2, Normal>;

using ActionButR = mdrivlib::FPin<Brain::Pin::D8.gpio, Brain::Pin::D8.pin, Output, Inverted>;
using ActionButG = mdrivlib::FPin<Brain::Pin::D11.gpio, Brain::Pin::D11.pin, Output, Inverted>;
using ActionButB = mdrivlib::FPin<Brain::Pin::D14.gpio, Brain::Pin::D14.pin, Output, Inverted>;
using ActionButLED = mdrivlib::MixedRgbLed<ActionButR, ActionButG, ActionButB>;
using ActionButGPwm = mdrivlib::TimPwmChan<Brain::Pin::D11PwmConf>;
using ActionButBPwm = mdrivlib::TimPwmChan<Brain::Pin::D14PwmConf>;
using ActionButPWM = mdrivlib::MixedRgbLed<ActionButR, ActionButGPwm, ActionButBPwm, LEDUpdateRateHz>;

constexpr std::array<mdrivlib::AdcChannelConf, NumPots> PotAdcChans = {{
	{Brain::Pin::A1, Brain::Pin::A1AdcChan, PotA, Brain::AdcSampTime},
	{Brain::Pin::A3, Brain::Pin::A3AdcChan, PotB, Brain::AdcSampTime},
	{Brain::Pin::A7, Brain::Pin::A7AdcChan, PotC, Brain::AdcSampTime},
	{Brain::Pin::A9, Brain::Pin::A9AdcChan, PotD, Brain::AdcSampTime},
}};

constexpr std::array<mdrivlib::AdcChannelConf, NumCVs> CVAdcChans = {{
	{Brain::Pin::A2, Brain::Pin::A2AdcChan, CVA, Brain::AdcSampTime},
	{Brain::Pin::A0, Brain::Pin::A0AdcChan, CVB, Brain::AdcSampTime},
	{Brain::Pin::A5, Brain::Pin::A5AdcChan, CVC, Brain::AdcSampTime},
	{Brain::Pin::A8, Brain::Pin::A8AdcChan, CVD, Brain::AdcSampTime},
	{Brain::Pin::A4, Brain::Pin::A4AdcChan, CVE, Brain::AdcSampTime},
}};

} // namespace Mp1BrainDemo::Board
