#include "algorithms/mlfq/mlfq_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

/*
    Here is where you should define the logic for the FCFS algorithm.
*/

MLFQScheduler::MLFQScheduler(int slice) {
}

//set a task's sime slice to 2^(q#)
//when it is chosen, move it to a subsequent queue

//if there are no threads: sp1->explanation = "No threads available for scheduling.";
std::shared_ptr<SchedulingDecision> MLFQScheduler::get_next_thread() {
        // make scheduling decision to return later
        auto sp1 = std::make_shared<SchedulingDecision>();

        if(size() <= 0){
            sp1->explanation = "No threads available for scheduling.";
            sp1->thread = NULL;
            return sp1;
        }

        for(int i = 0; i < 10; i++){
            if(mlfq[i].size() != 0){
                sp1->thread = mlfq[i].top();
                sp1->time_slice = pow(2,i);
                mlfq[i].pop();
                
                std::string Z = std::to_string(sp1->time_slice);
                std::string I = std::to_string(i);
                std::string P = "";
                std::string R = std::to_string(sp1->thread->runtime);

                switch(sp1->thread->priority){
                    case(SYSTEM):
                        P = "SYSTEM";
                        break;
                    case(INTERACTIVE):
                        P = "INTERACTIVE";
                        break;
                    case(NORMAL):
                        P = "NORMAL";
                        break;
                    case(BATCH):
                        P = "BATCH";
                        break;
                }

                sp1->explanation = ("Selected from queue "+I+" (priority = "+P+", runtime = "+R+"). Will run for at most "+Z+" ticks.");
                break;
            }
        }
        this->time_slice = sp1->time_slice;
        return sp1;
}

void MLFQScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        
        thread->runtime += thread->service_time-thread->previous_service_time;
        thread->previous_service_time = thread->service_time;

        int expected_time = (int)pow(2,thread->previous_queue);

        int queue_number = thread->previous_queue;
        
        if((thread->runtime >= expected_time && queue_number != 9) || queue_number == -1){
            queue_number++;
            thread->runtime = 0;
        }
        
        thread->previous_queue = queue_number;
        mlfq[queue_number].push(thread->priority,thread);

}

size_t MLFQScheduler::size() const {
        int sum = 0;
        for(int i = 0; i < 10; i++){
            sum += mlfq[i].size();
        }
        return sum;
}
