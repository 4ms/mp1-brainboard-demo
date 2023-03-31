#pragma once

#include <cstdint>

namespace Mp1BrainDemo
{

enum PotAdcElement : uint32_t {
	PotA,
	PotB,
	PotC,
	PotD,
};

enum CVAdcElement : uint32_t {
	CVA,
	CVB,
	CVC,
	CVD,
	CVE,
};

constexpr static uint32_t NumPots = 4;
constexpr static uint32_t NumCVs = 5;
constexpr static uint32_t NumAdcs = NumPots + NumCVs;

// Settings:
enum { FirmwareMajorVersion = 0 };
enum { FirmwareMinorVersion = 1 };

enum { LEDUpdateRateHz = 187 };

} // namespace Mp1BrainDemo
