#ifndef CCLayout_h__
#define CCLayout_h__


#include "cocos2d.h"
USING_NS_CC;


// design size.
// you can change it yourself.
#define CC_DESIGN_W (480.0f)
#define CC_DESIGN_H (320.0f)


inline float CCGetScaleX()
{
        CCSize winSize = 
                CCDirector::sharedDirector()->getWinSize();
        return winSize.width  / CC_DESIGN_W;
}


inline float CCGetScaleY()
{
        CCSize winSize = 
                CCDirector::sharedDirector()->getWinSize();
        return winSize.height / CC_DESIGN_H;
}


inline float CCGetCenterX()
{
        CCSize winSize = 
                CCDirector::sharedDirector()->getWinSize();
        return winSize.width / 2.0f;
}


inline float CCGetCenterY()
{
        CCSize winSize = 
                CCDirector::sharedDirector()->getWinSize();
        return winSize.height / 2.0f;
}


inline CCPoint CCGetCenterXY()
{
        CCSize winSize = 
                CCDirector::sharedDirector()->getWinSize();
        return ccp(winSize.width / 2.0f, winSize.height / 2.0f);
}


enum CCAlignment
{
        CC_ALIGNMENT_LB = 0,  // left bottom
        CC_ALIGNMENT_CB = 1,  // center bottom
        CC_ALIGNMENT_RB = 2,  // right bottom
        CC_ALIGNMENT_LC = 3,  // left center
        CC_ALIGNMENT_CC = 4,  // center center
        CC_ALIGNMENT_RC = 5,  // right center
        CC_ALIGNMENT_LT = 6,  // left top
        CC_ALIGNMENT_CT = 7,  // center top
        CC_ALIGNMENT_RT = 8,  // right top
};


inline CCPoint CCGetScreenPoint(float x, float y, CCAlignment alignment)
{
        CCSize winSize = 
                CCDirector::sharedDirector()->getWinSize();
        const float scaleX = winSize.width  / CC_DESIGN_W;
        const float scaleY = winSize.height / CC_DESIGN_H;


        CCPoint result;


        if (alignment % 3 == 0)
        {
                result.x = x * scaleX;
        }
        else if  (alignment % 3 == 2)
        {
                result.x = winSize.width + x * scaleX;
        }
        else
        {
                result.x = winSize.width / 2.0f + x * scaleY;
        }


        if (alignment / 3 == 0)
        {
                result.y = y * scaleY;
        }
        else if (alignment / 3 == 2)
        {
                result.y = winSize.height + y * scaleY;
        }
        else
        {
                result.y = winSize.height / 2.0f + y * scaleY;
        }


        return result;
}


inline CCPoint CCGetScreenPoint(const CCPoint& point, CCAlignment alignment)
{
        return CCGetScreenPoint(point.x, point.y, alignment);
}


#endif // CCLayout_h__  