#pragma once
#include "audio_stream_conf.hh"
#include "calibration_storage.hh"
#include "controls.hh"
#include "elements.hh"
#include "flags.hh"
#include "settings.hh"
#include "util/colors.hh"
#include "util/countzip.hh"
#include "util/math.hh"

namespace Mp1BrainDemo
{

// Params holds all the modes, settings and parameters for the sampler
// Params are set by controls (knobs, jacks, buttons, etc)
struct Params {
	Controls &controls;
	Flags &flags;
	CalibrationStorage &system_calibrations;
	UserSettings &settings;

	float pitch;
	float wave;
	float lfo_amt;
	float lfo_rate;

	Params(Controls &controls, Flags &flags, CalibrationStorage &system_calibrations, UserSettings &settings)
		: controls{controls}
		, flags{flags}
		, system_calibrations{system_calibrations}
		, settings{settings} {
		controls.start();
	}

	void update() {
		controls.update();

		update_pot_states();
		update_cv_states();

		update_pitch();
		update_wave();
		update_lfo_amt();
		update_lfo_rate();

		update_leds();
		update_button_modes();
	}

private:
	void update_pitch() {
		auto &pot = pot_state[PotA];
		float potval;
		if (pot.moved_while_rev_down) {
			potval = pot.latched_val;
		} else
			potval = pot.cur_val;

		pitch = (potval + cv_state[CVA].cur_val) / 4096.f;
	}

	void update_wave() {
		auto &pot = pot_state[PotB];
		float potval;

		if (pot.moved_while_bank_down) {
			potval = pot.latched_val;
		} else
			potval = pot.cur_val;

		wave = (potval + cv_state[CVB].cur_val) / 4096.f;
	}

	void update_lfo_amt() {
		auto &pot = pot_state[PotC];
		float potval;

		if (pot.moved_while_bank_down) {
			potval = pot.latched_val;
		} else
			potval = pot.cur_val;

		lfo_amt = (potval + cv_state[CVC].cur_val) / 4096.f;
	}

	void update_lfo_rate() {
		auto &pot = pot_state[PotD];
		float potval;

		if (pot.moved_while_bank_down) {
			potval = pot.latched_val;
		} else
			potval = pot.cur_val;

		lfo_rate = (potval + cv_state[CVD].cur_val) / 4096.f;
	}

	void update_pot_states() {
		for (auto [i, pot] : enumerate(pot_state)) {
			pot.cur_val = (int16_t)controls.read_pot(static_cast<PotAdcElement>(i));

			int16_t diff = std::abs(pot.cur_val - pot.prev_val);
			if (diff > Brain::MinPotChange)
				pot.track_moving_ctr = 250; // 250 for 6kHz = 42ms

			if (pot.track_moving_ctr) {
				pot.track_moving_ctr--;
				pot.prev_val = pot.cur_val;
				pot.delta = diff;
				// pot.moved = true;

				if (controls.action_button.is_pressed()) {
					if (!pot.moved_while_rev_down)
						pot.latched_val = pot.cur_val;
					pot.moved_while_rev_down = true;
					ignore_button_release = true;
				}
			}
		}
	}

	void update_cv_states() {
		for (auto [i, cv] : enumerate(cv_state)) {
			cv.cur_val = (int16_t)controls.read_cv(static_cast<CVAdcElement>(i));
			cv.cur_val += system_calibrations.cv_calibration_offset[i];

			int16_t diff = std::abs(cv.cur_val - cv.prev_val);
			if (diff > Brain::MinCVChange) {
				cv.delta = diff;
				cv.prev_val = cv.cur_val;
			}
		}
	}

	void update_trig_jacks() {
		if (controls.syncA_jack.is_just_pressed()) {
			// handle trig
		}

		if (controls.syncB_jack.is_just_pressed()) {
			// handle trig
		}
	}

	void update_button_modes() {
		if (controls.action_button.is_just_pressed()) {
			// handle play pressed
		}

		if (controls.action_button.is_just_released()) {
			if (!ignore_button_release) {
				// handle offset release
			}
			ignore_button_release = false;
		}

		if (controls.action_button.is_pressed()) {
			if (controls.action_button.how_long_held() == (Brain::ParamUpdateHz / 2)) // 0.5 sec
			{
				// handle long hold
				ignore_button_release = true;
			}
		}
	}

	void update_leds() {
		Color button_color;
		button_color = Colors::off;

		// Output to the LEDs
		if (last_button_color != button_color) {
			last_button_color = button_color;
			controls.offset_led.set_base_color(button_color);
		}
	}

private:
	struct PotState {
		int16_t cur_val = 0;
		int16_t prev_val = 0;		  // old_i_smoothed_potadc
		int16_t track_moving_ctr = 0; // track_moving_pot
		int16_t delta = 0;			  // pot_delta
		int16_t latched_val = 0;
		bool is_catching_up = false;
		bool moved_while_bank_down = false; // flag_pot_changed_infdown
		bool moved_while_rev_down = false;	// flag_pot_changed_revdown
											// bool moved = false;					// flag_pot_changed
	};
	std::array<PotState, NumPots> pot_state;

	struct CVState {
		int16_t cur_val = 0;
		int16_t prev_val = 0;
		int16_t delta = 0;
	};
	std::array<CVState, NumPots> cv_state;

	bool ignore_button_release = false;
	Color last_button_color = Colors::off;
};

} // namespace Mp1BrainDemo
