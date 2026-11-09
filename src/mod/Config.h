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
namespace pl::config {

template <>
struct Schema<clange_me::ModConfig> {
    static constexpr std::string_view title = "水晶與重生錨助手";
    static constexpr std::string_view description = "PvP 操作設定";

    static constexpr FieldSchema field(std::string_view name) {
        if (name == "version")
            return {"版本", "", {}, {}, true};

        if (name == "enabled")
            return {"啟用模組", "", {}, {}, false};

        if (name == "crystalEnabled")
            return {"啟用水晶連招", "", {}, {}, false};

        if (name == "crystalAutoAttack")
            return {"按住攻擊自動連打", "", {}, {}, false};

        if (name == "crystalCps")
            return {"水晶攻擊 CPS", "", 1, 20, false};

        if (name == "crystalDelayMs")
            return {"水晶操作延遲", "", 0, 250, false};

        if (name == "anchorEnabled")
            return {"啟用重生錨連招", "", {}, {}, false};

        if (name == "anchorAutoGlowstone")
            return {"自動切換螢石充能", "", {}, {}, false};

        if (name == "anchorSwitchToTotem")
            return {"充能後切換圖騰", "", {}, {}, false};

        if (name == "anchorAutoDetonate")
            return {"自動攻擊引爆", "", {}, {}, false};

        if (name == "anchorDelayMs")
            return {"重生錨操作延遲", "", 0, 250, false};

        if (name == "showButtons")
            return {"顯示操作按鈕", "", {}, {}, false};

        if (name == "buttonScale")
            return {"按鈕大小", "", 0.5, 2.0, false};

        return {};
    }
};

