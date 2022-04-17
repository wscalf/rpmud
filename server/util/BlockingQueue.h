#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

template <typename T>
class BlockingQueue
{
    private:
        mutex _mutex;
        condition_variable _condition;
        queue<T> _queue;
    public:
        void push(T const& value)
        {
            {
                unique_lock<mutex> lock(_mutex);
                _queue.push(value);
            }

            this->_condition.notify_one();
        }

        T pop()
        {
            unique_lock<mutex> lock(_mutex);
            this->_condition.wait(lock, [=]{return !_queue.empty();});
            T item = _queue.front();
            _queue.pop();
            return item;
        }
};

