#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <alphalaneous.editortab_api/include/EditorTabAPI.hpp>

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

        if (!m_editButtonBar) return true;
        
        alpha::editor_tabs::addTab("mods-tab"_spr, alpha::editor_tabs::EDIT,
        [&] {
            std::vector<Ref<CCNode>> items;

            auto buttonsArray = CCArray::create();
            bool placeButtons = false;

            for (int i = 0; i < m_editButtonBar->m_buttonArray->count(); i++) {
                auto* object = static_cast<CCMenuItemSpriteExtra*>(m_editButtonBar->m_buttonArray->objectAtIndex(i));

                if (object->getID() == "warp-button") {
                    placeButtons = true;
                    continue;
                }
                
                if (placeButtons) {
                    if (static_cast<std::string>(object->getID()).find("move") == std::string::npos || object->getID() == "") {
                        // log::debug("Adding button {} to custom bar", i);
                        items.push_back(object);
                        buttonsArray->addObject(object);
                    };
                }
            }

            m_editButtonBar->m_buttonArray->removeObjectsInArray(buttonsArray);

            auto rows = GameManager::sharedState()->getIntGameVariable("0049");
            auto cols = GameManager::sharedState()->getIntGameVariable("0050");
            m_editButtonBar->loadFromItems(m_editButtonBar->m_buttonArray, rows, cols, false);

            return alpha::editor_tabs::createEditButtonBar(items);
        },
        [] {
            CCLabelBMFont* textLabel = CCLabelBMFont::create("Mods", "bigFont.fnt");
            textLabel->setScale(0.25f);

            return textLabel;
        });

        return true;
    };
};