#include "model/scale/preset_scales.h"
#include "model/scale/note_set.h"
#include "util/d_string.h"

std::array<char const*, NUM_SCALELIKE> scalelikeNames = {
#define DEF(id, name, notes) name,
    DEF_SCALES()
#undef DEF
    // clang-format off
    "USER",
    "RANDOM",
    "NONE"
    // clang-format on
};

const NoteSet presetScaleNotes[NUM_PRESET_SCALES] = {
#define DEF(id, name, notes) notes,
    DEF_SCALES()
#undef DEF
};

const char* getScaleName(Scale scale) {
	if (scale < NUM_SCALELIKE) {
		return scalelikeNames[scale];
	}
	else {
		return "ERR";
	}
}

const uint8_t getAccidental(int32_t rootNoteCode, Scale scale) {
	if (rootNoteCode >= 0 && scale < NUM_SCALELIKE) {
		int8_t degree = 0;
		switch(scale) {
			case MAJOR_SCALE: degree=1; break;
			case DORIAN_SCALE: degree=2; break;
			case PHRYGIAN_SCALE: degree=3; break;
			case LYDIAN_SCALE: degree=4; break;
			case MIXOLYDIAN_SCALE: degree=5; break;
			case MELODIC_MINOR_SCALE:
			case HARMONIC_MINOR_SCALE:
			case HUNGARIAN_MINOR_SCALE:
			case BLUES_SCALE:
			case PENTATONIC_MINOR_SCALE:
			case HIRAJOSHI_SCALE:
			case MINOR_SCALE: degree=6; break;
			case LOCRIAN_SCALE: degree=7; break;
			default: degree=1; break;
		}
		switch(degree) {
			case 1: break;
			case 2: rootNoteCode -= 2; break;
			case 3: rootNoteCode -= 4; break;
			case 4: rootNoteCode -= 5; break;
			case 5: rootNoteCode -= 7; break;
			case 6: rootNoteCode -= 9; break;
			case 7: rootNoteCode -= 11; break;
			default: break;
		}
		int32_t majorRoot = rootNoteCode % 12;
		return majorAccidental[majorRoot];
	}
	else {
		return '.';
	}
}

void noteCodeToString(int32_t noteCode, char* buffer
	, int32_t* getLengthWithoutDot
	, bool appendOctaveNo // defaults to true
	, int32_t rootNoteCode // defaults to -1
	, Scale scale // defaults to NO_SCALE
) {
	char* thisChar = buffer;
	int32_t octave = (noteCode) / 12 - 2;
	int32_t n = (uint16_t)(noteCode + 120) % (uint8_t)12;

	uint8_t accidental = getAccidental(rootNoteCode, scale);
	if(noteIsAltered[n] ) {  // actually: if code is a black key on the piano?
		if((accidental=='#')) {
			*thisChar++ = noteLetter[n];
			*thisChar = accidental;
		} else {
			*thisChar++ = noteLetter[n+1];
			*thisChar = accidental;
		}
	} else {
		*thisChar = noteLetter[n];
	}
	thisChar++;
	if (appendOctaveNo) {
		intToString(octave, thisChar, 1);
	}

	if (getLengthWithoutDot) {
		*getLengthWithoutDot = strlen(buffer);
		if (noteIsAltered[n]) {
			(*getLengthWithoutDot)--;
		}
	}
}

Scale getScale(NoteSet notes) {
	for (int32_t p = 0; p < NUM_PRESET_SCALES; p++) {
		if (notes == presetScaleNotes[p]) {
			return static_cast<Scale>(p);
		}
	}
	return USER_SCALE;
}

bool isUserScale(NoteSet notes) {
	return USER_SCALE == getScale(notes);
}

void ensureNotAllPresetScalesDisabled(std::bitset<NUM_PRESET_SCALES>& disabled) {
	for (uint8_t i = 0; i < NUM_PRESET_SCALES; i++) {
		if (!disabled[i]) {
			return;
		}
	}
	// All disabled. Enable the major scale.
	disabled[MAJOR_SCALE] = false;
}

// These offsets allow us to introduce new 7, 6 and 5 note scales in between the existing
// ones keeping the decreasing order and without breaking backwards compatibility for
// defaults stored in flash memory.
#define FLASH_CODE_OFFSET_6_NOTE_SCALE 64
#define FLASH_CODE_OFFSET_5_NOTE_SCALE 128
#define FLASH_CODE_USER_SCALE 253
#define FLASH_CODE_RANDOM_SCALE 254
#define FLASH_CODE_NO_SCALE 255

Scale flashStorageCodeToScale(uint8_t code) {
	if (code < FLASH_CODE_OFFSET_6_NOTE_SCALE) {
		return static_cast<Scale>(code);
	}
	if (code < FLASH_CODE_OFFSET_5_NOTE_SCALE) {
		return static_cast<Scale>(FIRST_6_NOTE_SCALE_INDEX + code - FLASH_CODE_OFFSET_6_NOTE_SCALE);
	}
	if (code < FLASH_CODE_USER_SCALE) {
		return static_cast<Scale>(FIRST_5_NOTE_SCALE_INDEX + code - FLASH_CODE_OFFSET_5_NOTE_SCALE);
	}
	if (code == FLASH_CODE_USER_SCALE) {
		return USER_SCALE;
	}
	if (code == FLASH_CODE_RANDOM_SCALE) {
		return RANDOM_SCALE;
	}
	return NO_SCALE;
}

uint8_t scaleToFlashStorageCode(Scale scale) {
	if (scale < FIRST_6_NOTE_SCALE_INDEX) {
		return scale;
	}
	if (scale < FIRST_5_NOTE_SCALE_INDEX) {
		return scale - FIRST_6_NOTE_SCALE_INDEX + FLASH_CODE_OFFSET_6_NOTE_SCALE;
	}
	if (scale < USER_SCALE) {
		return scale - FIRST_5_NOTE_SCALE_INDEX + FLASH_CODE_OFFSET_5_NOTE_SCALE;
	}
	if (scale == USER_SCALE) {
		return FLASH_CODE_USER_SCALE;
	}
	if (scale == RANDOM_SCALE) {
		return FLASH_CODE_RANDOM_SCALE;
	}
	return FLASH_CODE_NO_SCALE;
}
