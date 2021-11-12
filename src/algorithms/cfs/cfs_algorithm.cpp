#include "algorithms/cfs/cfs_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

/*
    Here is where you should define the logic for the FCFS algorithm.
*/

CFSScheduler::CFSScheduler(int slice) {
    this->min_granularity = slice;
    weights.emplace(SYSTEM,88761.0);
    weights.emplace(INTERACTIVE,29154.0);
    weights.emplace(NORMAL,1024.0);
    weights.emplace(BATCH,15.0);
    sum_weights = 0;
}

//set a task's sime slice to 2^(q#)
//when it is chosen, move it to a subsequent queue

//if there are no threads: sp1->explanation = "No threads available for scheduling.";
std::shared_ptr<SchedulingDecision> CFSScheduler::get_next_thread() {
        // make scheduling decision to return later
        auto sp1 = std::make_shared<SchedulingDecision>();

        if(size() <= 0){
            sp1->explanation = "No threads available for scheduling.";
            sp1->thread = NULL;
            return sp1;
        }

        //choose the thread
        std::shared_ptr<Thread> thread = queue.top();

        double sched_latency = 48.0/queue.size();
        int task_slice = ((weights[thread->priority]/sum_weights)*sched_latency);

        //make sure time slice doesn't go below min granularity
        task_slice = std::max(task_slice,min_granularity);

        sum_weights -= weights[thread->priority];

        //explanation stuff
        int vruntime = (1024.0/weights[thread->priority])*thread->runtime;
        std::string X = std::to_string(queue.size());
        std::string V = std::to_string(vruntime);
        std::string Y = std::to_string(task_slice);
        sp1->explanation = "Selected from "+X+" threads (vruntime = "+V+"). Will run for at most "+Y+" ticks.";

        sp1->thread = thread;
        sp1->time_slice = task_slice;
        this->time_slice = task_slice;
        queue.pop();
        return sp1;
}

void CFSScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        
        thread->runtime += thread->service_time-thread->previous_service_time;
        thread->previous_service_time = thread->service_time;

        double vruntime = (1024.0/weights[thread->priority])*thread->runtime;

        queue.push(vruntime,thread);   
        sum_weights += weights[thread->priority];     

}

size_t CFSScheduler::size() const {
        return queue.size();
}
