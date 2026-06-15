#include "domain/CronJob.h"




CronJob::CronJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedulePayload)
    : Job(id , name , type , command , status , schedulePayload)
{
    CronJob::calculate_next_run();
}

CronJob::CronJob(const JobData &data) : Job(data) {}
