#include "domain/OneTimeJob.h"



OneTimeJob::OneTimeJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload)
    : Job(id , name , type , command , status , schedule_payload)
{
    // OneTimeJob::calculate_next_run();
}

OneTimeJob::OneTimeJob(const JobData &data) : Job(data) {}
