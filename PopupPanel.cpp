#include "PopupPanel.h"

PopupPanel* PopupPanel::create(PopupOptions* options)
{
    PopupPanel* pPanel = new PopupPanel();
    if (pPanel && pPanel->initWithOptions(options)){
        pPanel->autorelease();
        return pPanel;
    } else {
        pPanel = NULL;
        return NULL;
    }
}

bool PopupPanel::initWithOptions(PopupOptions* options)
{
    if ( !CCScale9Sprite::initWithFile("images/PopupPanelBG9Scale.png") ){
        return false;
    }
    // body text 
    CCLabelBMFont* _body = CCLabelBMFont::create(options->getBodyText(), "fonts/debug_font.fnt", _originalContentSize.width, kCCTextAlignmentCenter);
    //CCLOG("w %f h %f", _body->getContentSize().width, _body->getContentSize().height);
    // header text
    CCLabelBMFont* _header = CCLabelBMFont::create(options->getHeaderText(), "fonts/debug_font.fnt", _originalContentSize.width, kCCTextAlignmentCenter);
    
    // panel
    CCScale9Sprite::setCapInsets(CCRectMake(10, 10, 80, 80)); // rect inside the image
    _minContentSize = CCSizeMake(20, 20);
    _originalContentSize = _body->getContentSize() + options->getInsetPadding() * 4 + CCSizeMake(0, _header->getContentSize().height * 2);
    _origin = options->getOrigin();
    _target = options->getTarget();

    // contents
    _contents = CCSprite::create();

    // header
    _header->setPosition(CCPointMake(_originalContentSize.width * 0.5, _originalContentSize.height - 20));
    _contents->addChild(_header);

    // body
    // body inset
    _panelInset = CCScale9Sprite::create("images/PopupPanelInset9Scale.png");
    _panelInset->setContentSize(_body->getContentSize() + options->getInsetPadding() * 2); // depends on body text
    _panelInset->setPosition(_originalContentSize * 0.5);
    _contents->addChild(_panelInset);
    
    // body text
    _body->setPosition(_originalContentSize * 0.5);
    _contents->addChild(_body);

    // buttons
    _pMenu = CCMenu::create();
    CCObject* pObject = NULL;
    CCARRAY_FOREACH( options->getButtons(), pObject ){
        PopupButton* button = static_cast<PopupButton*>( pObject );
        CCMenuItem* item = CCMenuItemLabel::create(CCLabelBMFont::create( button->getText(), "fonts/debug_font.fnt", _originalContentSize.width, kCCTextAlignmentCenter), this, menu_selector(PopupPanel::onClose) );
        item->setUserObject( button );
        _pMenu->addChild( item );
    }
    _pMenu->alignItemsHorizontallyWithPadding( 40 );
    //CCLOG("w %f h %f", _pMenu->getContentSize().width, _pMenu->getContentSize().height);
    _pMenu->setPosition(CCPointMake(_originalContentSize.width * 0.5, 16));
    _contents->addChild(_pMenu);

    _contents->setCascadeOpacityEnabled(true);
    _contents->setOpacity(0);
    addChild(_contents);

    return true;
}

void PopupPanel::updateTweenAction(float value, const char* key)
{
    setContentSize( _minContentSize + (_originalContentSize - _minContentSize) * value );
    setPosition( _origin + (_target - _origin) * value );
}

CCFiniteTimeAction* PopupPanel::animateIn()
{
    CCFiniteTimeAction* panelMovesIn = CCTargetedAction::create(this, CCEaseExponentialOut::create(CCActionTween::create(0.3f, "", 0, 1)));
    CCFiniteTimeAction* contentsFadesIn = CCTargetedAction::create(_contents, CCFadeIn::create(0.1f));
    return static_cast<CCFiniteTimeAction*>(runAction(CCSequence::create(panelMovesIn, contentsFadesIn, NULL)));
}

CCFiniteTimeAction* PopupPanel::animateOut()
{
    CCFiniteTimeAction* contentsFadesOut = CCTargetedAction::create(_contents, CCFadeOut::create(0.1f));
    CCFiniteTimeAction* panelMovesOut = CCTargetedAction::create(this, CCEaseExponentialIn::create(CCActionTween::create(0.3f, "", 1, 0)));
    return static_cast<CCFiniteTimeAction*>(runAction(CCSequence::create(contentsFadesOut, panelMovesOut, NULL)));
}

void PopupPanel::onClose(CCObject* pSender)
{
    CCNode* pNode = static_cast<CCNode*>(pSender);
    PopupButton* pButton = static_cast<PopupButton*>(pNode->getUserObject());
    _pMenu->setTouchEnabled(false);
    CCFiniteTimeAction* contentsFadesOut = CCTargetedAction::create(_contents, CCFadeOut::create(0.1f));
    CCFiniteTimeAction* panelMovesOut = CCTargetedAction::create(this, CCEaseExponentialIn::create(CCActionTween::create(0.3f, "", 1, 0)));
    CCFiniteTimeAction* executeCallback = CCCallFunc::create(pButton->getTarget(), pButton->getHandler());
    CCFiniteTimeAction* removeSelf = CCTargetedAction::create(this, CCCallFunc::create(this, callfunc_selector(PopupPanel::end)));
    runAction(CCSequence::create(contentsFadesOut, panelMovesOut, executeCallback, removeSelf, NULL));
}

void PopupPanel::end()
{
    // remove the PopupLayer
    getParent()->removeFromParent();
}