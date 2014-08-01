
#include "AnimatedSpriteManager.h"

USING_NS_CC;

AnimatedSpriteManager* AnimatedSpriteManager::create( CCString* pObjectName )
{
    AnimatedSpriteManager *pManager = new AnimatedSpriteManager();
    if ( pManager && pManager->init( pObjectName ) ){
        pManager->autorelease();
        return pManager;
    }
    CC_SAFE_DELETE( pManager );
    return NULL;
}

bool AnimatedSpriteManager::init( CCString* pObjectName )
{
    CCAssert( pObjectName != NULL, "pObjectName cannot be null!" );

    _pName = pObjectName;
    _pName->retain();
    _pAnimations = CCDictionary::create();
    _pAnimations->retain();

    const char* nameCString = pObjectName->getCString();
    char str[100] = {0};

    // create the sprite batch node from the spritesheet
    sprintf( str, "images/%s/%s.png", nameCString, nameCString );
    CCSpriteBatchNode::initWithFile( str, kDefaultSpriteBatchCapacity );

    // cache the sprite frames for the spritesheet
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    sprintf( str, "images/%s/%s.plist", nameCString, nameCString );
    cache->addSpriteFramesWithFile( str );

    // create the animations from the animation config
    sprintf( str, "images/%s/data.plist", nameCString );
    CCArray *anims = CCArray::createWithContentsOfFile( str );
    CCObject* pObject = NULL;
    CCARRAY_FOREACH( anims, pObject ){
        CCDictionary *dict = static_cast<CCDictionary*>( pObject );
        const char *animNameCString = dict->valueForKey( "name" )->getCString();
        int frames = dict->valueForKey("frames")->intValue();
        float delay = dict->valueForKey("delay")->intValue() / 1000.0f;

        CCArray *animFrames = CCArray::createWithCapacity( frames );
        for ( int i=0; i < frames; i++ ){
            sprintf( str, "%s-%s-%d", nameCString, animNameCString, i );
            animFrames->addObject( cache->spriteFrameByName( str ) );
        }
        CCAnimation *animation = CCAnimation::createWithSpriteFrames( animFrames, delay );
        _pAnimations->setObject( animation, std::string( animNameCString ) );
    }
    return true;
}

CCSprite* AnimatedSpriteManager::createSprite( CCString* pDefaultAnimName )
{
    char str[100] = {0};
    sprintf( str, "%s-%s-0", _pName->getCString(), pDefaultAnimName->getCString() );
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName( str );
    this->addChild( pSprite );
    this->animateSprite( pSprite, pDefaultAnimName );
    return pSprite;
}

void AnimatedSpriteManager::destroySprite( CCSprite* pSprite )
{
    this->removeChild( pSprite, true );
}

void AnimatedSpriteManager::animateSprite( CCSprite* pSprite, CCString* pAnimName )
{
    pSprite->stopAllActions();
    CCAnimation *animation = (CCAnimation*)_pAnimations->objectForKey( std::string( pAnimName->getCString() ) );
    pSprite->runAction( CCRepeatForever::create( CCAnimate::create( animation ) ) );
}

void AnimatedSpriteManager::animateSpriteOnce( CCSprite* pSprite, CCString* pAnimName, CCString* pReturnAnim )
{
    pSprite->stopAllActions();
    CCAnimation *animation = (CCAnimation*)_pAnimations->objectForKey( std::string( pAnimName->getCString() ) );
    pReturnAnim->retain(); // retain this string the callback needs to use it.
    pSprite->runAction( CCSequence::createWithTwoActions( 
        CCAnimate::create( animation ),
        CCCallFuncND::create( this, callfuncND_selector( AnimatedSpriteManager::animationEndCallback ), pReturnAnim )
    ) );
}

void AnimatedSpriteManager::animationEndCallback(CCNode* pNode, void* data)
{
    pNode->stopAllActions();
    CCString* pAnimName = static_cast<CCString*>(data);
    CCAnimation *animation = (CCAnimation*)_pAnimations->objectForKey( std::string( pAnimName->getCString() ) );
    pNode->runAction( CCRepeatForever::create( CCAnimate::create( animation ) ) );

    pAnimName->release(); // release this string since it was previously retained.
}