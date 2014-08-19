//
// Created by Shubin Fedor on 19/08/14.
// Copyright (c) 2014 SOOMLA. All rights reserved.
//


#ifndef __CCPurchasingMission_H_
#define __CCPurchasingMission_H_

#include "CCMission.h"

namespace soomla {
    class CCPurchasingMission: public CCMission {
    public:
        CCPurchasingMission(): CCMission() {
        }

        static CCPurchasingMission *create(cocos2d::__String *id, cocos2d::__String *name,
                cocos2d::__String *associatedItemId);

        static CCPurchasingMission *create(cocos2d::__String *id, cocos2d::__String *name,
                cocos2d::__Array *rewards, cocos2d::__String *associatedItemId);

        SL_CREATE_WITH_DICTIONARY(CCPurchasingMission);

        virtual const char *getType() const;
    };
}

#endif //__CCPurchasingMission_H_
