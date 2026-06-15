#include "domain/IntervalJob.h"


IntervalJob::IntervalJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedulePayload)
    : Job(id , name , type , command , status , schedulePayload)
{
    IntervalJob::calculate_next_run();
}

IntervalJob::IntervalJob(const JobData &data) : Job(data) {}
