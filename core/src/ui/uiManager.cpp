#include <ui/uiManager.hpp>
#include <utils/logging.hpp>

#include <SDL_ttf.h>

UIManager::UIManager() = default;
UIManager::~UIManager() = default;

UIManager &UIManager::getInstance() {
    /** Meyers Singleton (another option is std::call_once) */
    static UIManager instance = UIManager();
    return instance;
}

bool UIManager::init()
{
    L_TAG("UIManager::UIManager");

    if(TTF_Init() != 0){
        L_ERROR("SDL2_ttf failed to initialize");
        return false;
    }

}

void UIManager::preUpdate() {

}

void UIManager::fixedUpdate(const time_ms &delta) {

}

void UIManager::update(const time_ms &delta) {

}

void UIManager::postUpdate() {

}

void UIManager::refresh() {

}

core::assets::Font UIManager::loadFont(const AssetName &name)
{
    L_TAG("UIManager::loadFont");

    core::assets::Font font;


    return font;
}
