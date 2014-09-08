#include "PopupPanel.h"

#include "utility/sprites/LabelBMFontWithSize.h"
#include "utility/ui/PopupButton.h"
#include "utility/UtilityMacros.h"

const int PopupPanel::BMFONT_SIZE = 16;
const float PopupPanel::POPUP_MOVE_TIME = 0.3f;
const float PopupPanel::POPUP_CONTENTS_FADE_TIME = 0.0f;

PopupPanel* PopupPanel::create( const PopupOptions &rOptions, const CCArray &rButtons )
{
    PopupPanel* pPanel = new PopupPanel();
    if ( pPanel && pPanel->init( rOptions, rButtons ) ) {
        pPanel->autorelease();
        return pPanel;
    } else {
        pPanel = NULL;
        return NULL;
    } 
}

PopupPanel::PopupPanel()
{
    _pContents = NULL;
    _pMenu = NULL;
    _originalContentSize = CCSizeZero;
    _minContentSize = CCSizeZero;
    _origin = CCPointZero;
    _target = CCPointZero;
}

bool PopupPanel::init( const PopupOptions &rOptions, const CCArray &rButtons )
{
    // Popup panels need at least one button.
    if ( rButtons.count() < 1 ) {
        return false;
    }

    if ( !CCScale9Sprite::initWithFile( "images/frame/frame_pop_up.png" ) ) {
        return false;
    }
    // rect inside the image
    CCScale9Sprite::setCapInsets( CCRectMake( 28, 28, 8, 8 ) );

    // contents
    _pContents = CCSprite::create();
    if ( _pContents == NULL ) {
        return false;
    }

    // some local variables
    CCEGLView *pEGLView = CCEGLView::sharedOpenGLView();
    float maxWidth = pEGLView->getDesignResolutionSize().width;
    float maxContentWidth = maxWidth - rOptions.insetPadding.width * 2;

    // HEADER SECTION
    LabelBMFontWithSize *pHeader = LabelBMFontWithSize::create( rOptions.headerText, 
                                                    "fonts/MSGothic14_bold.fnt",
                                                    static_cast<float>( rOptions.headerFontSize ),
                                                    maxContentWidth,
                                                    kCCTextAlignmentCenter );
    if ( pHeader == NULL ) {
        return false;
    }
    pHeader->setColor( ccBLACK );
    CCSize headerContentSize = CCSizeMake( maxContentWidth, 28 );
    _pContents->addChild( pHeader );

    // BODY SECTION
    CCSprite *pBody = CCSprite::create();
    if ( pBody == NULL ){
        return false;
    }
    CCSize bodyContentSize = CCSizeMake( maxWidth, 0 );
    if ( rOptions.body != NULL ) {
        pBody = rOptions.body;
        // padding-top and bottom of user's body content
        bodyContentSize.height += rOptions.body->getContentSize().height 
                                  + rOptions.insetPadding.height * 2;
    }
    pBody->setCascadeOpacityEnabled( true );
    _pContents->addChild( pBody );

    // FOOTER SECTION
    CCSprite *pFooter = CCSprite::create();
    if ( pFooter == NULL ){
        return false;
    }
    pFooter->setCascadeOpacityEnabled( true );
    CCSize footerContentSize = CCSizeMake( 320, 0 );
    LabelBMFontWithSize *pFooterLabel = LabelBMFontWithSize::create( rOptions.footerText,
                                                  "fonts/MSGothic14.fnt",
                                                  static_cast<float>( rOptions.footerFontSize ),
                                                  maxContentWidth,
                                                  kCCTextAlignmentCenter );
    if ( pFooterLabel == NULL ){
        return false;
    }
    pFooterLabel->setLineBreakWithoutSpace( true );
    pFooterLabel->setColor( ccBLACK );
    float footerLabelHeight = ( pFooterLabel->getContentSize() * pFooterLabel->getScale() ).height;
    footerContentSize.height += footerLabelHeight;
    pFooter->addChild( pFooterLabel );

    // BUTTONS
    _pMenu = CCMenu::create();
    if ( _pMenu == NULL ) {
        return false;
    }
    float menuHeight = 0.0f;
    CCObject *pObject = NULL;
    CCARRAY_FOREACH( &rButtons, pObject ) {
        PopupButton *pButton = NULL;
        SAFE_CAST( pButton, pObject, PopupButton* )
        LabelBMFontWithSize *pLabel =
            LabelBMFontWithSize::create( pButton->pText,
                                         "fonts/MSGothic14_bold.fnt",
                                         static_cast<float>( rOptions.buttonFontSize ),
                                         maxContentWidth,
                                         kCCTextAlignmentCenter );
        if ( pLabel == NULL ) {
            return false;
        }
        pLabel->setColor( ccBLACK );
        CCMenuItem *pItem = NULL;
        if ( pButton->pNormalImageFilepath != NULL ) {
            pItem = CCMenuItemImage::create( pButton->pNormalImageFilepath, 
                                             pButton->pPressedImageFilepath, 
                                             this, 
                                             menu_selector( PopupPanel::onClose ) );
            if ( pItem == NULL ) {
                return false;
            }
            pLabel->setPosition( pItem->getContentSize() * 0.5 );
            pItem->addChild( pLabel );
        } else {
            CCScale9Sprite *pDefaultSprite = NULL;
            CCScale9Sprite *pPressedSprite = NULL;
            CCScale9Sprite *pDisabledSprite = NULL;
            switch ( pButton->type ){
            case PopupButton::ButtonType::PRIMARY:
                pDefaultSprite = CCScale9Sprite::create( CCRectMake( 30, 0, 2, 32 ), "images/button/btn_yellow_paw_default.png" );
                pDefaultSprite->setContentSize( CCSizeMake( pLabel->getContentSize().width + 2 * 32, 32 ) );
                pPressedSprite = CCScale9Sprite::create( CCRectMake( 30, 0, 2, 32 ), "images/button/btn_yellow_paw_pressed.png" );
                pPressedSprite->setContentSize( CCSizeMake( pLabel->getContentSize().width + 2 * 32, 32 ) );
                break;
            case PopupButton::ButtonType::MAIN:
                pDefaultSprite = CCScale9Sprite::create( CCRectMake( 15, 0, 2, 32 ), "images/button/btn_yellow_default.png" );
                pDefaultSprite->setContentSize( CCSizeMake( pLabel->getContentSize().width + 2 * 15, 32 ) );
                pPressedSprite = CCScale9Sprite::create( CCRectMake( 15, 0, 2, 32 ), "images/button/btn_yellow_pressed.png" );
                pPressedSprite->setContentSize( CCSizeMake( pLabel->getContentSize().width + 2 * 15, 32 ) );
                break;
            case PopupButton::ButtonType::SECONDARY:
                pDefaultSprite = CCScale9Sprite::create( CCRectMake( 15, 0, 2, 32 ), "images/button/btn_grey_default.png" );
                pDefaultSprite->setContentSize( CCSizeMake( pLabel->getContentSize().width + 2 * 15, 32 ) );
                pPressedSprite = CCScale9Sprite::create( CCRectMake( 15, 0, 2, 32 ), "images/button/btn_grey_pressed.png" );
                pPressedSprite->setContentSize( CCSizeMake( pLabel->getContentSize().width + 2 * 15, 32 ) );
                break;
            default:
                CCLOG("%s", "[PopupPanel][init] Failed to create popup");
                break;
            }
            if ( pDefaultSprite == NULL ){
                return false;
            }
            pItem = CCMenuItemSprite::create( pDefaultSprite,
                                              pPressedSprite,
                                              NULL,
                                              this,
                                              menu_selector( PopupPanel::onClose ) );
            if ( pItem == NULL ) {
                return false;
            }
            pLabel->setPosition( pItem->getContentSize() * 0.5 );
            pItem->addChild( pLabel );
        }
        pItem->setUserObject( pButton );
        menuHeight = MAX( menuHeight, pItem->getContentSize().height );
        _pMenu->addChild( pItem );
    }
    _pMenu->alignItemsHorizontallyWithPadding( 40 );
    // add the menu's height to the footer content size
    footerContentSize.height += menuHeight;
    // add padding to the footer
    footerContentSize.height += rOptions.insetPadding.height * 3;
    pFooter->setContentSize( footerContentSize );
    pFooter->addChild( _pMenu );

    pFooterLabel->setPositionX( footerContentSize.width * 0.5 );
    pFooterLabel->setPositionY( footerContentSize.height * 0.5 + menuHeight * 0.5 + rOptions.insetPadding.height * 0.5 );
    _pMenu->setPositionX( footerContentSize.width * 0.5 );
    _pMenu->setPositionY( footerContentSize.height * 0.5 - footerLabelHeight * 0.5 - rOptions.insetPadding.height * 0.5 );

    _pContents->addChild( pFooter );

    // panel
    _minContentSize = CCSizeMake( 60, 60 );
    _originalContentSize = CCSizeZero;
    _originalContentSize.width = maxWidth;
    _originalContentSize.height = headerContentSize.height + 
                                  bodyContentSize.height + 
                                  footerContentSize.height;
    _origin = rOptions.origin;
    _target = rOptions.target;

    pHeader->setPositionY( _originalContentSize.height * 0.5
                           -headerContentSize.height * 0.5 );
    pBody->setPositionY( _originalContentSize.height * 0.5
                         -headerContentSize.height
                         -bodyContentSize.height * 0.5 );
    pFooter->setPositionY( _originalContentSize.height * 0.5
                           -headerContentSize.height
                           -bodyContentSize.height
                           -footerContentSize.height * 0.5 );

    // contents start as invisible and fades in at the end of tween
    _pContents->setCascadeOpacityEnabled( true );
    _pContents->setOpacity( 0 );
    _pContents->setPosition( _originalContentSize * 0.5 );

    this->addChild( _pContents );

    return true;
}

void PopupPanel::updateTweenAction(float value, const char *pKey)
{
    setContentSize( _minContentSize + (_originalContentSize - _minContentSize) * value );
    setPosition( _origin + (_target - _origin) * value );
}

CCFiniteTimeAction* PopupPanel::animateIn()
{
    CCFiniteTimeAction *pPanelMovesIn =
        CCTargetedAction::create( this, CCEaseExponentialOut::create( CCActionTween::create( POPUP_MOVE_TIME, "", 0, 1 ) ) );
    CCFiniteTimeAction *pContentsFadesIn = CCTargetedAction::create( _pContents, CCFadeIn::create( POPUP_CONTENTS_FADE_TIME ) );
    return static_cast<CCFiniteTimeAction*>( runAction( CCSequence::create( pPanelMovesIn, pContentsFadesIn, NULL ) ) );
}

CCFiniteTimeAction* PopupPanel::animateOut()
{
    CCFiniteTimeAction *pContentsFadesOut = CCTargetedAction::create(_pContents, CCFadeOut::create( POPUP_CONTENTS_FADE_TIME ));
    CCFiniteTimeAction *pPanelMovesOut =
        CCTargetedAction::create (this, CCEaseExponentialIn::create( CCActionTween::create( POPUP_MOVE_TIME, "", 1, 0 ) ) );
    return static_cast<CCFiniteTimeAction*>( runAction( CCSequence::create( pContentsFadesOut, pPanelMovesOut, NULL ) ) );
}

void PopupPanel::onClose(CCObject *pSender)
{
    CCNode *pNode = static_cast<CCNode*>(pSender);
    PopupButton *pButton = static_cast<PopupButton*>(pNode->getUserObject());
    _pMenu->setTouchEnabled(false);
    CCFiniteTimeAction *pContentsFadesOut = CCTargetedAction::create( _pContents, CCFadeOut::create( POPUP_CONTENTS_FADE_TIME ) );
    CCFiniteTimeAction *pPanelMovesOut =
        CCTargetedAction::create( this, CCEaseExponentialIn::create( CCActionTween::create( POPUP_MOVE_TIME, "", 1, 0 ) ) );
    CCFiniteTimeAction *pExecuteCallback = CCCallFunc::create( pButton->pTarget, pButton->pHandler );
    CCFiniteTimeAction *pRemoveSelf =
        CCTargetedAction::create( this, CCCallFunc::create( this, callfunc_selector( PopupPanel::onAnimationEnd ) ) );
    runAction(CCSequence::create(pContentsFadesOut, pPanelMovesOut, pExecuteCallback, pRemoveSelf, NULL));
}

void PopupPanel::onAnimationEnd()
{
    // remove the PopupLayer
    getParent()->removeFromParent();
}

void PopupPanel::onEnter()
{
    CCScale9Sprite::onEnter();
    animateIn();
}

void PopupPanel::onExit()
{
    CCScale9Sprite::onExit();
}