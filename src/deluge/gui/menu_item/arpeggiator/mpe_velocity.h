/*
 * Copyright (c) 2014-2023 Synthstrom Audible Limited
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
#pragma once
#include "gui/menu_item/selection.h"
#include "gui/ui/sound_editor.h"
#include "model/song/song.h"

namespace deluge::gui::menu_item::arpeggiator {
class ArpMpeVelocity final : public Selection {
public:
	using Selection::Selection;
	void readCurrentValue() override { this->setValue(soundEditor.currentArpSettings->mpeVelocity); }
	void writeCurrentValue() override {
		soundEditor.currentArpSettings->mpeVelocity = this->getValue<ArpMpeModSource>();
	}
	bool isRelevant(ModControllableAudio* modControllable, int32_t whichThing) override {
		return !soundEditor.editingGateDrumRow();
	}

	deluge::vector<std::string_view> getOptions(OptType optType) override {
		(void)optType;
		using enum l10n::String;
		return {
		    l10n::getView(STRING_FOR_DISABLED),                //<
		    l10n::getView(STRING_FOR_PATCH_SOURCE_AFTERTOUCH), //<
		    l10n::getView(STRING_FOR_PATCH_SOURCE_Y),          //<
		};
	}
};
} // namespace deluge::gui::menu_item::arpeggiator
