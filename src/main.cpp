#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <alphalaneous.editortab_api/include/EditorTabs.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {
    static void onModify(auto& self) {
        // if (Loader::get()->isModLoaded("hjfod.betteredit")) {
        //     if (!self.setHookPriorityAfterPost("EditorUI::init", "hjfod.betteredit")) {
        //         geode::log::warn("Failed to set hook priority.");
        //     } else {
        //         geode::log::debug("Hooked after BetterEdit!");
        //     }
        // } else {
		//     if (!self.setHookPriorityPost("EditorUI::init", Priority::Last)) {
		//     	geode::log::warn("Failed to set hook priority.");
		//     } else {
        //         geode::log::debug("Hooked as last!");
        //     }
        // }
        if (!self.setHookPriorityBeforePost("EditorUI::init", "hjfod.betteredit")) {
            geode::log::warn("Failed to set hook priority.");
        }
	};

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        EditButtonBar* customEditBar = nullptr;
        
        EditorTabs::addTab(this, TabType::EDIT, "mods_editor_edit_tab"_spr, [&customEditBar](EditorUI* ui, CCMenuItemToggler* toggler) -> CCNode* { //create the tab
            auto arr = CCArray::create();

            CCLabelBMFont* textLabelOn = CCLabelBMFont::create("Mods", "bigFont.fnt");
            textLabelOn->setScale(0.25f);
            CCLabelBMFont* textLabelOff = CCLabelBMFont::create("Mods", "bigFont.fnt");
            textLabelOff->setScale(0.25f);

            EditorTabUtils::setTabIcons(toggler, textLabelOn, textLabelOff);

            customEditBar = EditorTabUtils::createEditButtonBar(arr, ui);
            return customEditBar;
            
        }, [](EditorUI*, bool state, CCNode*) {});

        // Thanks to kuel27 for helping
        if (!customEditBar || !customEditBar->m_buttonArray) {
            log::error("Failed to create or access the custom edit bar");
            return true;
        }

        auto buttonsArray = CCArray::create();

        int lastRobtopButton = m_editButtonBar->m_buttonArray->indexOfObject(this->getChildByIDRecursive("warp-button"));
        
        for (int i = lastRobtopButton + 1; i < m_editButtonBar->m_buttonArray->count(); i++) {
            auto* object = static_cast<CCMenuItemSpriteExtra*>(m_editButtonBar->m_buttonArray->objectAtIndex(i));
            if (object->getID().find("move") == std::string::npos || object->getID() == "") {
                // log::debug("Adding button {} to custom bar", i);
                customEditBar->m_buttonArray->addObject(object);
                buttonsArray->addObject(object);
            };
        }

        m_editButtonBar->m_buttonArray->removeObjectsInArray(buttonsArray);

        auto rows = GameManager::sharedState()->getIntGameVariable("0049");
		auto cols = GameManager::sharedState()->getIntGameVariable("0050");
        customEditBar->loadFromItems(customEditBar->m_buttonArray, rows, cols, false);
        m_editButtonBar->loadFromItems(m_editButtonBar->m_buttonArray, rows, cols, false);

        return true;
    };
};