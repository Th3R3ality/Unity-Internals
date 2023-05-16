#pragma once
#include "rust/classes/BasePlayer/BasePlayer.hpp"

#include <string>

const static std::string hk__BasePlayer_Load_sig = "BasePlayer.Load(LoadInfo)";

void hk__BasePlayer_Load(BasePlayer* instance, BaseNetworkable::LoadInfo info);
