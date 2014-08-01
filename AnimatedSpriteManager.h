/**
 * AnimatedSpriteManager.h
 * v0.1
 *
 * @author Leon
 */

#ifndef CLASSES_SPRITE_ANIMATEDSPRITEMANAGER_H
#define CLASSES_SPRITE_ANIMATEDSPRITEMANAGER_H

#include <cocos2d.h>
#include <map>

USING_NS_CC;

/**
 * @brief Manages animated sprite creation
 *
 * AnimatedSpriteManager extends CCSpriteBatchNode so we can batch draw
 * many sprites from the same spritesheet.
 *
 * Usage Example:
 * @code
 * AnimatedSpriteManager* pManager = AnimatedSpriteManager::create(CCString::create("smurf"));
 * pScene->addChild( pManager ); // pScene is any drawable CCNode i.e. CCScene, CCLayer
 *
 * CCSprite* pIdleSmurfSprite = pManager->createSprite(CCString::create("idle"));
 * pIdleSmurfSprite->setPosition(ccp(100, 300));
 *
 * CCSprite* pDancingSmurfSprite = pManager->createSprite(CCString::create("dancing"));
 * pDancingSmurfSprite->setPosition(ccp(200, 300));
 *
 * // destroy if not using anymore
 * pManager->destroySprite(pIdleSmurfSprite);
 * pManager->destroySprite(pDancingSmurfSprite);
 * @endcode
 */
class AnimatedSpriteManager : public CCSpriteBatchNode
{
public:
    /**
     * @brief Create a manager to manage the object
     * @param pObjectName Name of the object
     * @return          Pointer to the AnimatedSpriteManager instance
     */
    static AnimatedSpriteManager* create( CCString* pObjectName );

    AnimatedSpriteManager(){};
    virtual ~AnimatedSpriteManager(){};

    /**
     * @brief Initializes the manager for the named object, using the
     * spritesheet and data files in its resource directory
     * @param  pObjectName Name of the object
     * @return             Success of initialization
     */
    bool init( CCString* pObjectName );

    /**
     * @brief Created a sprite that starts with the given animName into the batch draw
     *
     * The sprite is automatically added to this sprite batch node,
     * so only the manager itself needs to be added to the scene.
     *
     * @param  animName Name of an animation
     * @return          Pointer to the added sprite
     */
    CCSprite* createSprite( CCString* animName );

    /**
     * @brief Destroys a sprite previously created with createSprite
     * @param pSprite Pointer to the sprite to remove
     */
    void destroySprite( CCSprite* pSprite );

    /**
     * @brief Runs an animation defined in animations.plist on the sprite
     * @param pSprite  Pointer to the sprite
     * @param animName Name of an animation
     */
    void animateSprite( CCSprite* pSprite, CCString* animName );
    /**
     * @brief Runs a one shot animation, and returns to the pReturnAnim
     * @param pSprite     Pointer to the sprite
     * @param pAnimName   Name of an animation
     * @param pReturnAnim Name of an animation
     */
    void animateSpriteOnce( CCSprite* pSprite, CCString* pAnimName, CCString* pReturnAnim );
private:
    CCString *_pName;
    CCDictionary *_pAnimations;

    void animationEndCallback( CCNode* pNode, void* data );
};

#endif // CLASSES_SPRITE_ANIMATEDSPRITE_H

