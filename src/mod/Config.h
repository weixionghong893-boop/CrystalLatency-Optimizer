#pragma once

#include <string>
#include <string_view>

#include <pl/Config.hpp>

namespace clange_me {

struct ModConfig {
    int version = 1;
    bool enabled = true;

    // 水晶連招
    bool crystalEnabled = true;
    bool crystalAutoAttack = true;
    int crystalCps = 8;
    int crystalDelayMs = 0;

    // 重生錨連招
    bool anchorEnabled = true;
    bool anchorAutoGlowstone = true;
    bool anchorSwitchToTotem = true;
    bool anchorAutoDetonate = true;
    int anchorDelayMs = 0;

    // 操作介面
    bool showButtons = true;
    float buttonScale = 1.0f;
};

nlohmann::json makeDefaultConfigJson();
nlohmann::json makeConfigSchemaJson();

} // namespace clange_me

template <> struct pl::config::Schema<clange_me::ModConfig> {
    static constexpr std::string_view title = "水晶與重生錨助手";
    static constexpr std::string_view description = "PvP 操作設定";

    static constexpr FieldSchema field(std::string_view name) {
        if (name == "version")
            return {.title = "版本", .readOnly = true};

        if (name == "enabled")
            return {.title = "啟用模組"};

        if (name == "crystalEnabled")
            return {.title = "啟用水晶連招"};

        if (name == "crystalAutoAttack")
            return {.title = "按住攻擊自動連打"};

        if (name == "crystalCps")
            return {.title = "水晶攻擊 CPS"};

        if (name == "crystalDelayMs")
            return {.title = "水晶操作延遲"};

        if (name == "anchorEnabled")
            return {.title = "啟用重生錨連招"};

        if (name == "anchorAutoGlowstone")
            return {.title = "自動切換螢石充能"};

        if (name == "anchorSwitchToTotem")
            return {.title = "充能後切換圖騰"};

        if (name == "anchorAutoDetonate")
            return {.title = "自動攻擊引爆"};

        if (name == "anchorDelayMs")
            return {.title = "重生錨操作延遲"};

        if (name == "showButtons")
            return {.title = "顯示操作按鈕"};

        if (name == "buttonScale")
            return {.title = "按鈕大小"};

        return {};
    }
};
