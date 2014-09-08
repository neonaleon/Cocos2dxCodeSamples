/**
 * PopupPanel
 * v0.1
 *
 * @author Leon Ho
 */

#ifndef CLASSES_UTILITY_UI_POPUPPANEL_H
#define CLASSES_UTILITY_UI_POPUPPANEL_H

#include <cocos2d.h>
#include <cocos-ext.h>

#include "PopupOptions.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
  * @brief A popup panel with one or more buttons.
  *
  * The popup panel animates in and out of the screen.
  * Do not create an object of this class manually. Instead use NyagaApp::createPopup().
  */
class PopupPanel : public CCScale9Sprite, public CCActionTweenDelegate
{
public:
    static PopupPanel* create( const PopupOptions &pOptions, const CCArray &rButtons );
    bool init( const PopupOptions &rOptions, const CCArray &rButtons );

protected:
    PopupPanel();
    virtual ~PopupPanel() {};

private:
    static const int BMFONT_SIZE;
    static const float POPUP_MOVE_TIME;
    static const float POPUP_CONTENTS_FADE_TIME;

    CCFiniteTimeAction* animateIn();
    CCFiniteTimeAction* animateOut();

    /** @brief implements CCActionTweenDelegate */
    virtual void updateTweenAction( float value, const char *pKey );

    virtual void onEnter();
    virtual void onExit();

    /**
     * @brief Callback for items in _pMenu;
     *
     * All buttons will call this function when they are pressed.
     * The popup will begin animating out, and calls the registered callback as well.
     */
    void onClose( CCObject *pSender );

    /** @brief Removes this popup from the parent. */
    void onAnimationEnd();

    /** @brief root CCNode for contents within this PopupPanel. */
    CCSprite *_pContents;

    /* @brief menu for buttons */
    CCMenu *_pMenu;

    /** @brief size of this PopupPanel, calculated from contents */
    CCSize _originalContentSize;
    /** @brief a minimum size required due to caps of Scale9 image */
    CCSize _minContentSize;

    /** @brief @see PopupOptions.h */
    CCPoint _origin;
    /** @brief @see PopupOptions.h */
    CCPoint _target;
};

#endif // CLASSES_UTILITY_UI_POPUPPANEL_H
