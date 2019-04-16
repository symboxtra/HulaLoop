#ifndef HL_SEMAPHORE_H
#define HL_SEMAPHORE_H

#include <condition_variable>
#include <mutex>

class Semaphore {

    private:
        std::mutex mtx;
        std::condition_variable cond;
        unsigned long count = 1;

    public:
        Semaphore(){}

        // Semaphore(const Semaphore& s){
        //     this->count = s.count;
        // }

        // Semaphore()

        ~Semaphore(){};

        void notify() {
            std::lock_guard<std::mutex> lock(mtx);
            ++count;
            cond.notify_one();
        }

        void wait() {
            std::unique_lock<std::mutex> lock(mtx);
            while(!count)
                cond.wait(lock);
            --count;
        }
};


#endif