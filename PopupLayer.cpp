#include "PopupLayer.h"

#include "utility/gestures/SpriteInputRecognizer.h"
#include "utility/sprites/SpriteUtilityFunctions.h"

USING_NS_CC;
USING_NS_CC_EXT;

const int PopupLayer::Z_ORDER = 100;

PopupLayer* PopupLayer::create( PopupPanel *pPanel )
{
    CCAssert( pPanel != NULL, "PopupLayer cannot have a null PopupPanel object." );

    PopupLayer *pLayer = new PopupLayer();
    if ( pLayer && pLayer->init( pPanel ) ) { // Pass all function parameters to init().
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE( pLayer );
    return NULL;
}

// Constructor
PopupLayer::PopupLayer()
{
    _pPanel = NULL;
}

// Destructor
PopupLayer::~PopupLayer()
{}

// For custom init() with parameters.
bool PopupLayer::init( PopupPanel *pPanel )
{
    // Initialize the parent class first. This must still be called in custom init() functions.
    if ( !CCLayer::init() ) {
        return false;
    }

    setZOrder( PopupLayer::Z_ORDER );

    // Create a blocking sprite which will be used to block all user input behind the popup layer.
    CCEGLView *pEGLView = CCEGLView::sharedOpenGLView();
    CCSprite *pPopupBlockSprite = 
        NyagaApp::createBlankSprite( CCRectMake( pEGLView->getDesignResolutionSize().width / 2,
                                                 pEGLView->getDesignResolutionSize().height / 2,
                                                 pEGLView->getDesignResolutionSize().width, 
                                                 pEGLView->getDesignResolutionSize().height ),
                                     ccBLACK );
    pPopupBlockSprite->setAnchorPoint( CCPointZero );
    pPopupBlockSprite->setOpacity( 128 );
    addChild( pPopupBlockSprite, getZOrder() );

    // Create a gesture recognizer to catch all user input behind the popup layer.
    SpriteInputRecognizer *pRecognizer = SpriteInputRecognizer::create( getZOrder() );
    pRecognizer->addTargetOnDown( pPopupBlockSprite, this, SpriteInputSelector(PopupLayer::onSpriteTouch), getZOrder() );
    pRecognizer->addTargetOnUp( pPopupBlockSprite, this, SpriteInputSelector(PopupLayer::onSpriteTouch), getZOrder() );
    pRecognizer->addTargetOnEnter( pPopupBlockSprite, this, SpriteInputSelector(PopupLayer::onSpriteTouch), getZOrder() );
    pRecognizer->addTargetOnExit( pPopupBlockSprite, this, SpriteInputSelector(PopupLayer::onSpriteTouch), getZOrder() );
    
    addChild( pRecognizer );

    // Add the popup panel just above the blocking sprite.
    _pPanel = pPanel;
    addChild( _pPanel, getZOrder() + 1 );

    return true;
}

// Callback just prevents user input from going through to anything behind the popup layer.
bool PopupLayer::onSpriteTouch( CCSprite *pSprite, CCObject* pObject )
{
    return true;
}
