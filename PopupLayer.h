#ifndef CLASSES_UTILITY_UI_POPUPLAYER_H
#define CLASSES_UTILITY_UI_POPUPLAYER_H

#include <cocos2d.h>
#include <cocos-ext.h>

#include "utility/ui/PopupPanel.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
  * @brief A popup layer that holds a popup panel with one or more buttons.
  *
  * Whenever a popup layer is created, all user input to elements behind the popup
  * layer is blocked. This is also indicated visually by dimming all those elements.
  * This affects all elements with z-order lower than PopupLayer::Z_ORDER.
  *
  * Once the user selects any button on the popup, the layer is removed and user
  * input functionality restored to normal.
  *
  * It is recommended to use NyagaApp::createPopup() to create a popup layer.
  *
  * Cross-functionality issues:
  * - Requires Cocos2Dx's v2. (Tested on v2.2.5)
  * - Requires PopupPanel class and PopupOptions, PopupButton structs.
  */
class PopupLayer : public CCLayer
{
public:
    /** @brief The starting z-order of popup layers.
      *
      * Child elements of the layer such as the panel is incremented from this
      * value.
      */
    static const int Z_ORDER;

    /**
      * @brief Creates a PopupLayer object as an autorelease object.
      *
      * @param pPanel Pointer to the panel object to display.
      * @return The layer object that was just created.
      */
    static PopupLayer* create( PopupPanel *pPanel );

    /**
      * @brief Automatically called when a PopupLayer object is created.
      *
      * This is in contrast with PopupLayer::onEnter(), which is called when it is
      * added to a scene. Thus lazy loading of assets are usually done in here.
      * @param pPanel Pointer to the panel object to disp
      * @return True if the initialization was successful.
      */
    bool init( PopupPanel *pPanel ); // Custom init method taking in parameters if required

protected:
    // Constructor and destructor are set to protected scope to ensure only the create()
    // functions are used to create PopupLayer instances.
    PopupLayer();
    virtual ~PopupLayer();

private:
    // Callback when the blocking sprite is touched.
    bool onSpriteTouch( CCSprite *pSprite, CCObject* pObject );

    PopupPanel *_pPanel;

    // Prevents implicit copying and assigning of the layer.
    DISALLOW_COPY_AND_ASSIGN(PopupLayer);
};

#endif // CLASSES_UTILITY_UI_POPUPLAYER_H
