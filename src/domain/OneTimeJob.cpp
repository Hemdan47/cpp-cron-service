#include "domain/OneTimeJob.h"



OneTimeJob::OneTimeJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedulePayload)
    : Job(id , name , type , command , status , schedulePayload)
{
    OneTimeJob::calculate_next_run();
}

OneTimeJob::OneTimeJob(const JobData &data) : Job(data) {}
