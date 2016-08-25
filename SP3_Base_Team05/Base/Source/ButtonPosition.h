/******************************************************************************/
/*!
\file	ButtonPosition.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Button Header File to store position of buttons on screen
*/
/******************************************************************************/

#ifndef BUTTON_POSITION_H
#define BUTTON_POSITION_H

/******************************************************************************/
/*!
Struct ButtonPosition:
\brief	Stores button information
*/
/******************************************************************************/
class ButtonPosition
{
public:
    ButtonPosition(){}
    ~ButtonPosition(){}
    float minX, maxX, minY, maxY;       ///< Minimum and Maximum X and Y for hit-box

    void Set(float factorMinX, float factorMaxX, float factorMinY, float factorMaxY);
};

#endif