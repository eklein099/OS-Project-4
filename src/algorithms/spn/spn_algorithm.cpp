#include "algorithms/spn/spn_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

/*
    Here is where you should define the logic for the FCFS algorithm.
*/

SPNScheduler::SPNScheduler(int slice) {
    if (slice != -1) {
        throw("FCFS must have a timeslice of -1");
    }

}

std::shared_ptr<SchedulingDecision> SPNScheduler::get_next_thread() {
        // make scheduling decision to return later
        auto sp1 = std::make_shared<SchedulingDecision>();

        //if there are no threads, return that description
        if(thread_priority_queue.empty()){
            sp1->explanation = "No threads available for scheduling.";
            return sp1;
        }

        //add generic explanation
        std::string x = std::to_string(thread_priority_queue.size());
        sp1->explanation = ("Selected from "+x+" threads. Will run to completion of burst.");

        //choose the first thread and add it to sp1
        std::shared_ptr<Thread> thread = thread_priority_queue.top();
        thread_priority_queue.pop();
        sp1->thread = thread;

        //the time slice is irrelevant for this algorithm.
        sp1->time_slice = -1;
        
        return sp1;
}

void SPNScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        thread_priority_queue.push(thread->bursts.front()->length,thread);        
}

size_t SPNScheduler::size() const {
        return thread_priority_queue.size();
}
