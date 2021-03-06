#include "util/ThreadPool.h"

using namespace std;


void consumeWork(BlockingQueue<function<void()>>* workQueue)
{
    while (true)
    {
        auto todo = workQueue->pop();
        todo();
    }
}

ThreadPool::ThreadPool(int size)
{
    for (auto i = 0; i < size; i++)
       threads.push_back(new thread(consumeWork, &workQueue)); //Can we make that an instance method?
}

void ThreadPool::Enqueue(function<void()> toDo)
{
    workQueue.push(toDo);
}
