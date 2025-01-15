/*
 * Copyright © 2017-2023 Synthstrom Audible Limited, 2025 Mark Adams
 *
 * This file is part of The Synthstrom Audible Deluge Firmware.
 *
 * The Synthstrom Audible Deluge Firmware is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef DELUGE_OSCILLATOR_H
#define DELUGE_OSCILLATOR_H

#include "storage/wave_table/wave_table.h"
namespace deluge {
namespace dsp {

class Oscillator {

	static void maybeStorePhase(const OscType& type, uint32_t* startPhase, uint32_t phase, bool doPulseWave);
	static void applyAmplitudeVectorToBuffer(int32_t amplitude, int32_t numSamples, int32_t amplitudeIncrement,
	                                         int32_t* outputBufferPos, int32_t* inputBuferPos);

public:
	static void renderOsc(OscType type, int32_t amplitude, int32_t* bufferStart, int32_t* bufferEnd, int32_t numSamples,
	                      uint32_t phaseIncrement, uint32_t pulseWidth, uint32_t* startPhase, bool applyAmplitude,
	                      int32_t amplitudeIncrement, bool doOscSync, uint32_t resetterPhase,
	                      uint32_t resetterPhaseIncrement, uint32_t retriggerPhase, int32_t waveIndexIncrement,
	                      int sourceWaveIndexLastTime, WaveTable* waveTable);
};

} // namespace dsp
} // namespace deluge

#endif // DELUGE_OSCILLATOR_H
