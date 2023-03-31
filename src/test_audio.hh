#pragma once
#include "audio_stream_conf.hh"
#include "params.hh"
#include "util/zip.hh"

// Testing
#include "util/oscs.hh"

namespace Mp1BrainDemo
{
class TestAudio {
	Params &params;
	TriangleOscillator<48000> osc1{1000};
	TriangleOscillator<48000> osc2{15000};

public:
	TestAudio(Params &params)
		: params{params} {}

	void update(const AudioStreamConf::AudioInBlock &in, AudioStreamConf::AudioOutBlock &out) {
		// Just testing pass-through and audio outputs
		if (params.wave > 0.5f) {
			for (auto [o, i] : zip(out, in)) {
				o.chan[0] = i.chan[0];
				o.chan[1] = i.chan[1];
			}
		} else {
			osc1.set_frequency(params.pitch * 1000.f);
			osc2.set_frequency(params.lfo_rate * 100.f);
			for (auto [o, i] : zip(out, in)) {
				o.chan[0] = (osc1.process_float() - 0.5f) * 0x00FFFFFFUL;
				o.chan[1] = (osc2.process_float() - 0.5f) * 0x00FFFFFFUL;
			}
		}
	}
};
} // namespace Mp1BrainDemo
