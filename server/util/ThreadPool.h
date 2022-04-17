#include <vector>
#include <thread>
#include "BlockingQueue.h"
#include <functional>

class ThreadPool
{
    private:
        BlockingQueue<std::function<void()>> workQueue;
        std::vector<std::thread*> threads;
    public:
        ThreadPool(int size);
        void Enqueue(std::function<void()> toDo);
};