#include <thread>
#include <vector>
#include <sstream>
#include <concurrentqueue/blockingconcurrentqueue.h>
#include <fmt/format.h>
#include "tl/component/threadpool.h"

namespace tl::blacklist{
class ThreadPoolInternal {
public:
    using Queue = moodycamel::BlockingConcurrentQueue<Task *>;

    ThreadPoolInternal(int nthreads) : 
        q_(32 * moodycamel::BlockingConcurrentQueue<Task *>::BLOCK_SIZE, 10, 10),
        nthreads_(nthreads)
    {}

    ThreadPoolInternal(const ThreadPoolInternal &) = delete;

    void Start() {
        threads_.reserve(nthreads_);
        for (int i = 0; i < nthreads_; ++i) {
            threads_.emplace_back(std::thread(&ThreadPoolInternal::ThreadFunc, this, i));
        }
    }

    void ThreadFunc(int i) {
        std::string thread_name = "tl-blacklist-pool-" + std::to_string(i);
        pthread_setname_np(pthread_self(), thread_name.c_str());
        Queue::consumer_token_t token(q_);
        std::stringstream oss;
	    oss << std::this_thread::get_id();
        LOG_INFO("thread id : {}, name : {}", oss.str(),thread_name.c_str());
        for (;;) {
            if (auto r = stopped_.load(std::memory_order_relaxed); r != 0) {
                // TODO: task memory leak
                LOG_ERROR("task memory leak");
                return;
            }

            Task * task;
            // wait for 10ms
            if (auto success = q_.wait_dequeue_timed(token, task, 10 * 1000); success) {
                auto _ = std::unique_ptr<Task>(task);
                //LOG_INFO("exec task");
                (*task)();
            }
        }
    }

    void Stop() {
        if (auto r = stopped_.load(std::memory_order_acquire); r != 0) {
            return;
        }
        stopped_.store(1, std::memory_order_relaxed);
        for (int i = 0; i < threads_.size(); ++i) {
            threads_[i].join();
        }
        threads_.clear();
    }

    static Queue::producer_token_t & GetProducerToken(ThreadPoolInternal & internal) {
        static thread_local Queue::producer_token_t token(internal.q_);
        return token;
    }

    Queue q_;
    std::vector<std::thread> threads_;
    int nthreads_;
    std::atomic<int> stopped_{0};
};

ThreadPool::ThreadPool(int nthreads) {
    internal_.reset(new ThreadPoolInternal(nthreads));
    internal_->Start();
}

ThreadPool::~ThreadPool() {
    internal_->Stop();
}

void ThreadPool::Submit(Task * task) {
    internal_->q_.enqueue(ThreadPoolInternal::GetProducerToken(*internal_), task);
}

void ThreadPool::SubmitWithoutToken(Task * task) {
    internal_->q_.enqueue(task);
}

void ThreadPool::SubmitBulk(LambdaTaskIteratorWrapper wrapper, size_t count) {
    internal_->q_.enqueue_bulk(ThreadPoolInternal::GetProducerToken(*internal_), std::move(wrapper), count);
}

int ThreadPool::GetThreadCount() const {
    return internal_->nthreads_;
}

void ThreadPool::Stop() {
    internal_->Stop();
}
}
