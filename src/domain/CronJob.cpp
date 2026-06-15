#include "domain/CronJob.h"




CronJob::CronJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload)
    : Job(id , name , type , command , status , schedule_payload)
{
    // CronJob::calculate_next_run();
}

CronJob::CronJob(const JobData &data) : Job(data) {}
