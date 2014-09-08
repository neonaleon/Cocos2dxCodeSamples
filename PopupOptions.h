#ifndef CLASSES_UTILITY_UI_POPUPOPTIONS_H
#define CLASSES_UTILITY_UI_POPUPOPTIONS_H

#include <cocos2d.h>

USING_NS_CC;

/**
  * @brief Contains the option values for a popup.
  *
  * An object of this struct is required to create a popup.
  * @see NyagaApp::createPopup()
  */
struct PopupOptions : public CCObject
{
    CREATE_FUNC( PopupOptions );

    PopupOptions() {
        bodyFontSize = 0;
        bodyText = "";
        buttonFontSize = 0;
        footerFontSize = 0;
        footerText = "";
        headerFontSize = 0;
        headerText = "";
        insetPadding = CCSizeZero;
        origin = CCPointZero;
        target = CCPointZero;
        body = NULL;
    }
    ~PopupOptions() {}

    bool init() {
        CCEGLView *pEGLView = CCEGLView::sharedOpenGLView();
        bodyFontSize = 16;
        bodyText = "Placeholder Body Text";
        buttonFontSize = 16;
        footerFontSize = 16;
        footerText = "Placeholder Footer Text";
        headerFontSize = 16;
        headerText = "Placeholder Header Text";
        insetPadding = CCSizeMake( 10, 10 );
        origin = pEGLView->getDesignResolutionSize() * 0.5;
        target = pEGLView->getDesignResolutionSize() * 0.5;
        return true;
    }

    int bodyFontSize;
    const char* bodyText;
    int buttonFontSize;
    int footerFontSize;
    const char* footerText;
    int headerFontSize;
    const char* headerText;

    /** @brief The padding of the popup inset.*/
    CCSize insetPadding;

    /** @brief The popup will animate in from, and animate out to the Origin. */
    CCPoint origin;

    /** @brief The popup will animate in and reach its full size at the Target. */
    CCPoint target; 

    // pass a pointer to the popup, it will be retained by the popup
    CCSprite *body;
};

#endif // CLASSES_UTILITY_UI_POPUPOPTIONS_H