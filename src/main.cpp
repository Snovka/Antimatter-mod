#include <Geode/Geode.hpp>
#include <Geode/ui/MatUI.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;

static Mod* mod = nullptr;
static float lastTapTime = 0.f;
static bool menuOpen = false;

class $modify(PlayLayer) {
    bool onTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
        float now = CCDirector::sharedDirector()->getRunningScene()->getTotalTime();
        if (now - lastTapTime < 0.3f && !menuOpen) {
            showCheatMenu();
        }
        lastTapTime = now;
        return PlayLayer::onTouchBegan(touch, event);
    }
};

class $modify(PlayerObject) {
    void playerDestroyed(bool force) {
        if (mod->getSettingValue<bool>("noclip") && !force) return;
        PlayerObject::playerDestroyed(force);
    }

    void pushButton(bool down) {
        if (mod->getSettingValue<bool>("infinite-jump")) {
            this->m_isOnGround = true;
        }
        PlayerObject::pushButton(down);
    }
};

class $modify(GJBaseGameLayer) {
    void update(float dt) {
        float speed = mod->getSettingValue<float>("speedhack");
        auto director = CCDirector::sharedDirector();
        director->getScheduler()->setTimeScale(speed);
        GJBaseGameLayer::update(dt / speed);
    }
};

void showCheatMenu() {
    menuOpen = true;
    auto scene = SceneManager::sharedState()->getRunningScene();
    if (auto playLayer = typeinfo_cast<PlayLayer*>(scene)) {
        auto table = matui::TableView::create({0.5f, 0.6f}, {0.4f, 0.4f}, matui::TableType::Cells);
        table->setBackColor({0, 0, 0, 200});
        table->setTitle("Antimatter");

        table->addToggle("Noclip", "noclip", [](bool val) {
            mod->setSettingValue("noclip", val);
        });
        table->addSlider("Speed", "speedhack", 0.1f, 5.0f, [](float val) {
            mod->setSettingValue("speedhack", val);
        });
        table->addToggle("Infinite Jump", "infinite-jump", [](bool val) {
            mod->setSettingValue("infinite-jump", val);
        });
        table->addButton("Close", []() {
            menuOpen = false;
        });

        playLayer->addChild(table);
    }
}

$execute {
    mod = Mod::get();
    log::info("Antimatter loaded! Double-tap to open menu.");
};
