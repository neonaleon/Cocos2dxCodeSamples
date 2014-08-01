
#include "PopupOptions.h"

bool PopupOptions::init(){
    _buttons = CCArray::create();
    _buttons->retain();
    return true;
}

PopupOptions::PopupOptions():
    _headerFontSize(12),
    _bodyFontSize(12),
    _buttonFontSize(12),
    _origin(CCSizeMake(160, 284)),
    _target(CCSizeMake(160, 284)),
    _headerText("placeholder header text"),
    _bodyText("placeholder body text"),
    _insetPadding(CCSizeMake(10, 10))
{}

PopupOptions::~PopupOptions()
{
    _buttons->release();
}

void PopupOptions::addButton(const char* text, CCObject* pTarget, SEL_CallFunc pHandler)
{
    PopupButton* button = PopupButton::create();
    button->setText(text);
    button->setTarget(pTarget);
    button->setHandler(pHandler);
    _buttons->addObject(button);
}

CCArray* PopupOptions::getButtons()
{
    return _buttons;
}