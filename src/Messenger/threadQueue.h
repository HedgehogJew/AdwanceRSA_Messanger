#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>

using namespace boost;

//Взаимодействия с данными в очереди защищены

template <typename T>
class tQueue
{
public:
    tQueue() = default;
    tQueue(const tQueue<T>&) = delete;
    ~tQueue(){clear();}

    const T& front()
    {
        std::scoped_lock lock(muxQueue); //Мютекс потока сбрасывается при удалении метода со стека
        return deqQueue.front();
    }

    const T& back()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.back();
    }

    void push_back(const T& item)
    {
        {
            std::scoped_lock lock(muxQueue);
            deqQueue.emplace_back(std::move(item));
        }
        cvQueue.notify_one();
    }

    void push_front(const T& item)
    {
        {
            std::scoped_lock lock(muxQueue);
            deqQueue.emplace_front(std::move(item));
        }
        cvQueue.notify_one();
    }

    bool empty()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.empty();
    }

    size_t count()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.size();
    }

    void clear()
    {
        std::scoped_lock lock(muxQueue);
        deqQueue.clear();
    }

    T pop_front()
    {
        std::scoped_lock lock(muxQueue);
        T res = std::move(deqQueue.front());
        deqQueue.pop_front();
        return res;
    }

    T pop_back()
    {
        std::scoped_lock lock(muxQueue);
        T res = std::move(deqQueue.back());
        deqQueue.pop_back();
        return res;
    }
    
    //Основной метод, для ожидания заполнения очереди
    std::optional<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(muxQueue);
        cvQueue.wait(lock, [this]() { return !deqQueue.empty(); });
        auto t = std::move(deqQueue.front());
        deqQueue.pop_front();
        return t;
    }

    //Уведомление потоков о изменении в очереди
    void notify_all()
    {
        cvQueue.notify_all();
    }

private:
    std::mutex muxQueue;
    std::deque<T> deqQueue;
    std::condition_variable cvQueue;
};

#endif
