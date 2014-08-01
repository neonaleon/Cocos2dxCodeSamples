/**
 * PopupPanel
 * v0.1
 *
 *
 *
 * @author Leon Ho
 */

#ifndef CLASSES_UI_POPUPPANEL_H
#define CLASSES_UI_POPUPPANEL_H

#include <cocos2d.h>
#include <cocos-ext.h>

#include "PopupOptions.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PopupPanel : public CCScale9Sprite, public CCActionTweenDelegate
{
public:
    static PopupPanel* create(PopupOptions* options);
    bool initWithOptions(PopupOptions* options);

    void updateTweenAction(float value, const char* key);
    CCFiniteTimeAction* animateIn();
    CCFiniteTimeAction* animateOut();

    void onClose(CCObject* pSender);
    void end();
private:
    CCSprite* _contents;
    CCScale9Sprite* _panelInset;

    CCSize _originalContentSize;
    CCSize _minContentSize;

    CCPoint _origin;
    CCPoint _target;

    CCMenu* _pMenu;
};

#endif // CLASSES_UI_POPUPPANEL_H