#ifndef CLASSES_UI_POPUPOPTIONS_H
#define CLASSES_UI_POPUPOPTIONS_H

#include <cocos2d.h>

USING_NS_CC;

class PopupButton : public CCObject
{
public:
    CREATE_FUNC( PopupButton );
    bool init(){ return true; }

    CC_SYNTHESIZE(const char*, _text, Text);
    CC_SYNTHESIZE(SEL_CallFunc, _handler, Handler);
    CC_SYNTHESIZE(CCObject*, _target, Target);
};

class PopupOptions : public CCObject
{
public:
    CREATE_FUNC( PopupOptions );
    bool init();

    PopupOptions();
    ~PopupOptions();

    CC_SYNTHESIZE(int, _headerFontSize, HeaderFontSize);
    CC_SYNTHESIZE(int, _bodyFontSize, BodyFontSize);
    CC_SYNTHESIZE(int, _buttonFontSize, ButtonFontSize);
    CC_SYNTHESIZE(CCSize, _insetPadding, InsetPadding);
    CC_SYNTHESIZE(CCPoint, _origin, Origin);
    CC_SYNTHESIZE(CCPoint, _target, Target);
    CC_SYNTHESIZE(const char*, _headerText, HeaderText);
    CC_SYNTHESIZE(const char*, _bodyText, BodyText);

    void addButton(const char* text, CCObject* pTarget, SEL_CallFunc pHandler);
    CCArray* getButtons();
private:
    CCArray* _buttons;
};

#endif // CLASSES_UI_POPUPOPTIONS_H