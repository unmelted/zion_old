/*
 * LIVSMED CONFIDENTIAL
 *
 * Copyright (c) 2024 LIVSMED, INC.
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains the property
 * of LIVSMED and its suppliers, if any. The intellectual and technical concepts
 * contained herein are proprietary to LIVSMED and its suppliers and may be
 * covered by S.Korea and Foreign Patents, patents in process, and are
 * protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is
 * strictly forbidden unless prior written permission is obtained from LIVSMED.
 *
 * Created by Eunkyung Ma(ekma@livsmed.com) on 2024/01/05.
 *
 */

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template<class T>
class MessageQueue 
{
public:
    MessageQueue(void)
        : mQueue()
        , mMutex()
        , mCondition() 
    {
        mIsLoop = true;
    }

    ~MessageQueue(void) 
    {
    }

    void Clear() 
    {
        std::lock_guard<std::mutex> lock(mMutex);
        std::queue<T> emptyQueue;
        std::swap(mQueue, emptyQueue);
    }

    void Enqueue(T t) 
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mQueue.push(t);
        mCondition.notify_one();
    }

    void Destory() 
    {
        mIsLoop = false;
        mCondition.notify_one();
    }

    T Dequeue(void) 
    {
        std::unique_lock<std::mutex> lock(mMutex);
        while ((mIsLoop) && (mQueue.empty())) 
        {
            mCondition.wait(lock);
        }
        T val = mQueue.front();
        mQueue.pop();
        return val;
    }

    bool IsQueue() 
    {
        std::lock_guard<std::mutex> lock(mMutex);
        bool ret = !mQueue.empty();
        mCondition.notify_one();
        return ret;
    }

    int32_t GetSize() 
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return int32_t(mQueue.size());
    }


private:
    std::queue<T>           mQueue;
    mutable std::mutex      mMutex;
    std::condition_variable mCondition;
    bool                    mIsLoop;
};
