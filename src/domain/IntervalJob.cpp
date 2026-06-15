#include "domain/IntervalJob.h"


IntervalJob::IntervalJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload)
    : Job(id , name , type , command , status , schedule_payload)
{
    // IntervalJob::calculate_next_run();
}

IntervalJob::IntervalJob(const JobData &data) : Job(data) {}
