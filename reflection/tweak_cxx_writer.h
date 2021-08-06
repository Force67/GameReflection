// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

namespace retk {
class TweaksDatabase;

// it shoudld have to rely on tweaks db, but rather directly on information of
// parser pass.
bool WriteTweaksCXXRegistry(const TweaksDatabase&, const std::string &out_path);
}
