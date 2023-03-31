#pragma once
#include "brain_conf.hh"
#include "conf/board_conf.hh"
#include "debug.hh"
#include "drivers/adc_builtin.hh"
#include "drivers/analog_in_ext.hh"
#include "drivers/debounced_switch.hh"
#include "elements.hh"
#include "util/filter.hh"

namespace Mp1BrainDemo
{

class Controls {

	template<typename ConfT>
	using AdcDmaPeriph = mdrivlib::AdcDmaPeriph<ConfT>;

	// ADCs (Pots and CV):
	static inline __attribute__((section(".noncachable"))) std::array<uint16_t, NumCVs> cv_adc_buffer;
	AdcDmaPeriph<Brain::CVAdcConf> cv_adcs{cv_adc_buffer, Board::CVAdcChans};

	static inline __attribute__((section(".noncachable"))) std::array<uint16_t, NumPots> pot_adc_buffer;
	AdcDmaPeriph<Brain::PotAdcConf> pot_adcs{pot_adc_buffer, Board::PotAdcChans};

	static constexpr bool hardware_oversampling = Brain::PotAdcConf::oversample;
	std::array<Oversampler<128, uint16_t>, NumPots> pots;
	std::array<Oversampler<32, uint16_t>, NumCVs> cvs;

public:
	Controls() {} //= default;

	// Buttons/Switches:
	Board::ActionButton action_button;

	// Trig Jacks
	Board::SyncAJack syncA_jack;
	Board::SyncBJack syncB_jack;

	// LEDs:
	Board::ActionButPWM offset_led;

	uint16_t read_pot(PotAdcElement adcnum) {
		if constexpr (hardware_oversampling)
			return pot_adc_buffer[adcnum];
		else
			return pots[adcnum].val();
	}
	uint16_t read_cv(CVAdcElement adcnum) {
		if constexpr (hardware_oversampling)
			return cv_adc_buffer[adcnum];
		else
			return cvs[adcnum].val();
	}

	void start() {
		if constexpr (!hardware_oversampling) {
			pot_adcs.register_callback([this] {
				for (unsigned i = 0; auto &pot : pots)
					pot.add_val(pot_adc_buffer[i++]);
			});
			cv_adcs.register_callback([this] {
				for (unsigned i = 0; auto &cv : cvs)
					cv.add_val(cv_adc_buffer[i++]);
			});
		}
		pot_adcs.start();
		cv_adcs.start();
	}

	void update() {
		// Debug::Pin1::high();
		action_button.update();
		syncA_jack.update();
		syncB_jack.update();

		constexpr uint32_t led_throttle = Brain::ParamUpdateHz / LEDUpdateRateHz;
		static uint32_t led_throttle_ctr = 0;
		// 187Hz
		if (led_throttle_ctr++ > led_throttle) {
			led_throttle_ctr = 0;
			offset_led.update_animation();
		}
		// Debug::Pin1::low();
	}
};
} // namespace Mp1BrainDemo
