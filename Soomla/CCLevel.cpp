//
// Created by Shubin Fedor on 22/08/14.
// Copyright (c) 2014 SOOMLA. All rights reserved.
//

#include "CCLevel.h"
#include "CCLevelStorage.h"
#include "CCSoomlaUtils.h"

namespace soomla {

    #define TAG "SOOMLA Level"

    USING_NS_CC;

    CCLevel *CCLevel::create(cocos2d::__String *id) {
        CCLevel *ret = new CCLevel();
        if (ret->init(id)) {
            ret->autorelease();
        }
        else {
            CC_SAFE_DELETE(ret);
        }

        return ret;
    }

    CCLevel *CCLevel::create(cocos2d::__String *id, CCGate *gate, cocos2d::__Dictionary *scores, cocos2d::__Array *missions) {
        CCLevel *ret = new CCLevel();
        if (ret->init(id, gate, NULL, scores, missions)) {
            ret->autorelease();
        }
        else {
            CC_SAFE_DELETE(ret);
        }

        return ret;
    }

    CCLevel *CCLevel::create(cocos2d::__String *id, CCGate *gate, cocos2d::__Dictionary *innerWorlds, cocos2d::__Dictionary *scores, cocos2d::__Array *missions) {
        CCLevel *ret = new CCLevel();
        if (ret->init(id, gate, innerWorlds, scores, missions)) {
            ret->autorelease();
        }
        else {
            CC_SAFE_DELETE(ret);
        }

        return ret;
    }

    char const *CCLevel::getType() const {
        return CCLevelUpConsts::JSON_JSON_TYPE_LEVEL;
    }

    CCLevel::~CCLevel() {

    }

    int CCLevel::getTimesStarted() {
        return CCLevelStorage::getInstance()->getTimesStarted(this);
    }

    int CCLevel::getTimesPlayed() {
        return CCLevelStorage::getInstance()->getTimesPlayed(this);
    }

    long CCLevel::getSlowestDurationMillis() {
        return CCLevelStorage::getInstance()->getSlowestDurationMillis(this);
    }

    long CCLevel::getFastestDurationMillis() {
        return CCLevelStorage::getInstance()->getFastestDurationMillis(this);
    }

    bool CCLevel::start() {
        if (mState == LevelState::Running) {
            return false;
        }

        CCSoomlaUtils::logDebug(TAG,
                __String::createWithFormat("Starting level with world id: %s", getId()->getCString())->getCString());

        if (!this->canStart()) {
            return false;
        }

        if (mState != LevelState::Paused) {
            mElapsed = 0;
            CCLevelStorage::getInstance()->incTimesStarted(this);
        }

        mStartTime = getCurrentTimeMs();
        mState = LevelState::Running;
        return true;
    }

    void CCLevel::pause() {
        if (mState != LevelState::Running) {
            return;
        }

        long now = getCurrentTimeMs();
        mElapsed += now - mStartTime;
        mStartTime = 0;

        mState = LevelState::Paused;
    }

    long CCLevel::getPlayDurationMillis() {

        long now = getCurrentTimeMs();
        long duration = mElapsed;
        if (mStartTime != 0) {
            duration += now - mStartTime;
        }

        return duration;
    }


    void CCLevel::end(bool completed) {

        // check end() called without matching start()
        if(mStartTime == 0) {
            CCSoomlaUtils::logError(TAG, "end() called without prior start()! ignoring.");
            return;
        }

        mState = LevelState::Ended;

        if (completed) {
            long duration = getPlayDurationMillis();

            // Calculate the slowest \ fastest durations of level play

            if (duration > getSlowestDurationMillis()) {
                CCLevelStorage::getInstance()->setSlowestDurationMillis(this, duration);
            }

            if (duration < getFastestDurationMillis()) {
                CCLevelStorage::getInstance()->setFastestDurationMillis(this, duration);
            }

            DictElement* el = NULL;
            CCScore *score;
            __Dictionary *scoresDict = getScores();
            CCDICT_FOREACH(scoresDict, el) {
                    score = (CCScore *) el->getObject();
                    score->reset(true); // resetting scores
                }

            // Count number of times this level was played
            CCLevelStorage::getInstance()->incTimesPlayed(this);

            // reset timers
            mStartTime = 0;
            mElapsed = 0;

            setCompleted(true);
        }
    }

    void CCLevel::restart(bool completed) {
        if (mState == LevelState::Running || mState == LevelState::Paused) {
            end(completed);
        }
        start();
    }

    void CCLevel::setCompleted(bool completed) {
        mState = LevelState::Completed;
        CCWorld::setCompleted(completed);
    }

    long CCLevel::getCurrentTimeMs() {
        struct timeval now;
        gettimeofday(&now, NULL);

        return (long) (1000 * now.tv_sec + now.tv_usec / 1000.0f);
    }
}