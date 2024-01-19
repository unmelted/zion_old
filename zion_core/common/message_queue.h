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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/01/05.
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
        : queue_()
        , mutex_()
        , condition_()
    {
        isLoop_ = true;
    }

    ~MessageQueue(void) 
    {
    }

    void Clear() 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::queue<T> emptyQueue;
        std::swap(queue_, emptyQueue);
    }

    void Enqueue(T t) 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(t);
        condition_.notify_one();
    }

    void Destory() 
    {
        isLoop_ = false;
        condition_.notify_one();
    }

    T Dequeue()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while ((isLoop_) && (queue_.empty()))
        {
            condition_.wait(lock);
        }
        T val = queue_.front();
        queue_.pop();
        return val;
    }

    bool IsQueue() 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        bool ret = !queue_.empty();
        condition_.notify_one();
        return ret;
    }

    int32_t GetSize() 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return int32_t(queue_.size());
    }


private:
    std::queue<T>           queue_;
    mutable std::mutex      mutex_;
    std::condition_variable condition_;
    bool                    isLoop_;
};
