#include "audio_stream.hh"
#include "calibration_storage.hh"
#include "conf/board_conf.hh"
#include "controls.hh"
#include "debug.hh"
#include "flags.hh"
#include "hardware_tests/hardware_tests.hh"
#include "params.hh"
#include "settings.hh"
#include "system.hh"
#include "test_audio.hh"

namespace
{
// Initialize the system before main()
Mp1BrainDemo::System _init;
} // namespace

void main() {
	using namespace Mp1BrainDemo;
	using namespace mdrivlib;
	using AudioInBlock = AudioStreamConf::AudioInBlock;
	using AudioOutBlock = AudioStreamConf::AudioOutBlock;

	Controls controls;
	if (Board::ActionButton::PinT::read()) {
		// TODO:
		// HWTests::run(controls);
	}

	CalibrationStorage system_calibrations;
	auto fw_version = system_calibrations.load_flash_params();

	UserSettings settings;

	Flags flags;
	Params params{controls, flags, system_calibrations, settings};

	TestAudio audio{params};

	AudioStream audio_stream([&audio, &params](const AudioInBlock &in, AudioOutBlock &out) {
		Debug::Pin0::high();
		params.update();
		audio.update(in, out);
		Debug::Pin0::low();
	});

	audio_stream.start();

	while (true) {
		__NOP();
	}
}

void recover_from_task_fault(void) { __BKPT(); }
