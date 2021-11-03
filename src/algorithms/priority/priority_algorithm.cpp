#include "algorithms/priority/priority_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

/*
    Here is where you should define the logic for the FCFS algorithm.
*/

PRIORITYScheduler::PRIORITYScheduler(int slice) {
    if (slice != -1) {
        throw("FCFS must have a timeslice of -1");
    }
    priority_counts.emplace(SYSTEM,0);
    priority_counts.emplace(INTERACTIVE,0);
    priority_counts.emplace(NORMAL,0);
    priority_counts.emplace(BATCH,0);
}

std::shared_ptr<SchedulingDecision> PRIORITYScheduler::get_next_thread() {
        // make scheduling decision to return later
        auto sp1 = std::make_shared<SchedulingDecision>();

        //if there are no threads, return that description
        if(thread_priority_queue.empty()){
            sp1->explanation = "No threads available for scheduling.";
            return sp1;
        }

        std::string u[4] = {std::to_string(priority_counts[SYSTEM]),std::to_string(priority_counts[INTERACTIVE]),std::to_string(priority_counts[NORMAL]),std::to_string(priority_counts[BATCH])};
        
        //choose the first thread and add it to sp1
        std::shared_ptr<Thread> thread = thread_priority_queue.top();
        thread_priority_queue.pop();
        sp1->thread = thread;
        priority_counts[thread->priority]--;
        
        std::string v[4] = {std::to_string(priority_counts[SYSTEM]),std::to_string(priority_counts[INTERACTIVE]),std::to_string(priority_counts[NORMAL]),std::to_string(priority_counts[BATCH])};
        
        sp1->explanation = (" [S: "+u[0]+" I: "+u[1]+" N: "+u[2]+" B: "+u[3]+"] -> [S: "+v[0]+" I: "+v[1]+" N: "+v[2]+" B: "+v[3]+"]. Will run to completion of burst.");

        

        //the time slice is irrelevant for this algorithm.
        sp1->time_slice = -1;
        
        return sp1;
}

void PRIORITYScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        thread_priority_queue.push(thread->priority,thread);   
        priority_counts[thread->priority]++;
}

size_t PRIORITYScheduler::size() const {
        return thread_priority_queue.size();
}
