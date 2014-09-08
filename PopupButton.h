#ifndef CLASSES_UI_POPUPBUTTON_H
#define CLASSES_UI_POPUPBUTTON_H

#include <cocos2d.h>

USING_NS_CC;

/**
  * @brief Contains data for a single button in a popup. 
  *
  * This object should be added to a CCArray that is passed into the
  * NyagaApp::createPopup() function when creating a popup.
  *
  * Do not create an instance of this struct manually. Instead use NyagaApp::createPopupButton().
  */
struct PopupButton : public CCObject
{
    struct ButtonType {
        enum {
            PRIMARY = 0, // give yellow paw button
            MAIN,        // gives yellow non-paw button
            SECONDARY    // gives grey button
        };
    };

    CREATE_FUNC( PopupButton );

    PopupButton():
        pText( "" ),
        pHandler( NULL ),
        pTarget( NULL ),
        pNormalImageFilepath( "" ),
        pPressedImageFilepath( "" ),
        type( 0 )
    {};

    bool init() { return true; }

    /** @brief Semantic type of the button. Used to determine what style to apply. */
    int type;

    /** @brief Callback function when button is pressed.*/
    SEL_CallFunc pHandler;

    /** @brief Filepath of button image when not being pressed.*/
    const char* pNormalImageFilepath;

    /** @brief Filepath of button image when being pressed.*/
    const char* pPressedImageFilepath;

    /** @brief Object containing the callback function.*/
    CCObject *pTarget;

    /** @brief Button text.*/
    const char *pText;
};

#endif // CLASSES_UI_POPUPBUTTON_H
