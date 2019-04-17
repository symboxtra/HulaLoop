#ifndef HL_SEMAPHORE_H
#define HL_SEMAPHORE_H

#include <condition_variable>
#include <mutex>

/**
 * Basic semaphore implementation using
 * C++ condition_variables so that it can
 * be locked in one thread and unlocked in another.
 */
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

        /**
         * Notify one waiter that the semaphore is available.
         */
        void notify() {
            std::lock_guard<std::mutex> lock(mtx);
            ++count;
            cond.notify_one();
        }

        /**
         * Wait for the semaphore to be available.
         */
        void wait() {
            std::unique_lock<std::mutex> lock(mtx);
            while(!count)
                cond.wait(lock);
            --count;
        }
};


#endif