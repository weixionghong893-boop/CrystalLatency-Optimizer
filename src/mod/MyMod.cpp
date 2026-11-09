#include "mod/MyMod.h"

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <mutex>
#include <string>
#include <string_view>

#include <pl/Mod.hpp>
#include <pl/ModMenu.hpp>

namespace clange_me {

namespace {

constexpr std::string_view MODULE_ID = "clange_me.pvp";
constexpr std::string_view CRYSTAL_BUTTON = "clange_me.crystal";
constexpr std::string_view ANCHOR_BUTTON = "clange_me.anchor";

bool parseBool(std::string_view value, bool fallback) {
    if (value == "true" || value == "1") return true;
    if (value == "false" || value == "0") return false;
    return fallback;
}

int parseInt(std::string_view value, int fallback) {
    std::string s(value);
    char* end = nullptr;
    long result = std::strtol(s.c_str(), &end, 10);

    if (end == s.c_str() || *end != '\0') return fallback;
    return static_cast<int>(result);
}

}

ClangeMeMod &ClangeMeMod::instance() {
    static ClangeMeMod instance;
    return instance;
}

ClangeMeMod::ClangeMeMod()
    : mSelf(*ll::mod::NativeMod::current()) {}

bool ClangeMeMod::load() {
    auto &self = getSelf();

    std::error_code ec;
    std::filesystem::create_directories(self.getDataDir(), ec);
    if (ec) return false;

    std::filesystem::create_directories(self.getConfigDir(), ec);
    if (ec) return false;

    mConfigFile.emplace();

    if (!mConfigFile->load()) {
        self.getLogger().error("Config loading failed");
        mConfigFile.reset();
        return false;
    }

    mConfig = mConfigFile->value();

    self.getLogger().info("PvP config loaded");
    return true;
}

bool ClangeMeMod::enable() {
    auto &self = getSelf();

    auto module = pl::modmenu::ModuleBuilder(
        std::string(MODULE_ID), "水晶與重生錨助手"
    )
    .modId(self.getId())
    .description("水晶 PvP 與重生錨操作設定")
    .defaultEnabled(mConfig.enabled)
    .onToggle([](std::string_view, bool enabled) {
        auto &mod = instance();
        mod.mConfig.enabled = enabled;
        if (mod.mConfigFile) {
            mod.mConfigFile->value() = mod.mConfig;
            mod.mConfigFile->save();
        }
    })
    .config("crystalEnabled", "啟用水晶功能",
        pl::modmenu::ConfigType::Toggle,
        mConfig.crystalEnabled ? "true" : "false")
    .config("crystalAutoAttack", "按住攻擊自動連打",
        pl::modmenu::ConfigType::Toggle,
        mConfig.crystalAutoAttack ? "true" : "false")
    .config("crystalCps", "水晶攻擊 CPS",
        pl::modmenu::ConfigType::SliderInt,
        std::to_string(mConfig.crystalCps), "1", "20")
    .config("crystalDelayMs", "水晶操作延遲",
        pl::modmenu::ConfigType::SliderInt,
        std::to_string(mConfig.crystalDelayMs), "0", "250")
    .config("anchorEnabled", "啟用重生錨功能",
        pl::modmenu::ConfigType::Toggle,
        mConfig.anchorEnabled ? "true" : "false")
    .config("anchorAutoGlowstone", "自動切換螢石",
        pl::modmenu::ConfigType::Toggle,
        mConfig.anchorAutoGlowstone ? "true" : "false")
    .config("anchorSwitchToTotem", "充能後切換圖騰",
        pl::modmenu::ConfigType::Toggle,
        mConfig.anchorSwitchToTotem ? "true" : "false")
    .config("anchorAutoDetonate", "自動攻擊引爆",
        pl::modmenu::ConfigType::Toggle,
        mConfig.anchorAutoDetonate ? "true" : "false")
    .config("anchorDelayMs", "重生錨操作延遲",
        pl::modmenu::ConfigType::SliderInt,
        std::to_string(mConfig.anchorDelayMs), "0", "250")
    .config("showButtons", "顯示操作按鈕",
        pl::modmenu::ConfigType::Toggle,
        mConfig.showButtons ? "true" : "false")
    .config("buttonScale", "按鈕大小",
        pl::modmenu::ConfigType::SliderFloat,
        std::to_string(mConfig.buttonScale), "0.5", "2.0")
    .onConfigChanged([](std::string_view id,
                        std::string_view key,
                        std::string_view value) {
        auto &mod = instance();

        if (id != MODULE_ID) return;

        auto &c = mod.mConfig;

        if (key == "crystalEnabled")
            c.crystalEnabled = parseBool(value, c.crystalEnabled);
        else if (key == "crystalAutoAttack")
            c.crystalAutoAttack = parseBool(value, c.crystalAutoAttack);
        else if (key == "crystalCps")
            c.crystalCps = std::clamp(parseInt(value, c.crystalCps), 1, 20);
        else if (key == "crystalDelayMs")
            c.crystalDelayMs = std::clamp(parseInt(value, c.crystalDelayMs), 0, 250);
        else if (key == "anchorEnabled")
            c.anchorEnabled = parseBool(value, c.anchorEnabled);
        else if (key == "anchorAutoGlowstone")
            c.anchorAutoGlowstone = parseBool(value, c.anchorAutoGlowstone);
        else if (key == "anchorSwitchToTotem")
            c.anchorSwitchToTotem = parseBool(value, c.anchorSwitchToTotem);
        else if (key == "anchorAutoDetonate")
            c.anchorAutoDetonate = parseBool(value, c.anchorAutoDetonate);
        else if (key == "anchorDelayMs")
            c.anchorDelayMs = std::clamp(parseInt(value, c.anchorDelayMs), 0, 250);
        else if (key == "showButtons")
            c.showButtons = parseBool(value, c.showButtons);
        else if (key == "buttonScale") {
            try {
                c.buttonScale = std::clamp(std::stof(std::string(value)), 0.5f, 2.0f);
            } catch (...) {}
        } else {
            return;
        }

        if (mod.mConfigFile) {
            mod.mConfigFile->value() = c;
            mod.mConfigFile->save();
        }
    })
    .registerModule();

    if (!module) {
        self.getLogger().error("Mod Menu registration failed");
        return false;
    }

    self.getLogger().info("PvP Mod Menu registered");

    return true;
}

bool ClangeMeMod::disable() {
    pl::modmenu::unregisterButton(CRYSTAL_BUTTON);
    pl::modmenu::unregisterButton(ANCHOR_BUTTON);
    pl::modmenu::unregisterModule(MODULE_ID);

    getSelf().getLogger().info("PvP Mod Menu disabled");
    return true;
}

bool ClangeMeMod::unload() {
    pl::modmenu::unregisterButton(CRYSTAL_BUTTON);
    pl::modmenu::unregisterButton(ANCHOR_BUTTON);
    pl::modmenu::unregisterModule(MODULE_ID);

    mConfigFile.reset();
    return true;
}

} // namespace clange_me
