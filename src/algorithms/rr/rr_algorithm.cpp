#include "algorithms/rr/rr_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

/*
    Here is where you should define the logic for the FCFS algorithm.
*/

RRScheduler::RRScheduler(int slice) {
    if (slice == -1) {
        throw("RR must have a positive slice");
    }
    this->time_slice = slice;

}

std::shared_ptr<SchedulingDecision> RRScheduler::get_next_thread() {
        // make scheduling decision to return later
        auto sp1 = std::make_shared<SchedulingDecision>();

        //if there are no threads, return that description
        if(thread_queue.empty()){
            sp1->explanation = "No threads available for scheduling.";
            return sp1;
        }

        //add generic explanation
        std::string x = std::to_string(thread_queue.size());
        std::string y = std::to_string(this->time_slice);
        sp1->explanation = ("Selected from "+x+" threads. Will run for at most "+y+" ticks.");

        //choose the first thread and add it to sp1
        std::shared_ptr<Thread> thread = thread_queue.front();
        thread_queue.pop();
        sp1->thread = thread;

        //the time slice is irrelevant for this algorithm.
        sp1->time_slice = -1;
        
        return sp1;
}

void RRScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        thread_queue.push(thread);        
}

size_t RRScheduler::size() const {
        return thread_queue.size();
}
