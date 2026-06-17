#include "service/JobService.h"

#include <asio/detail/memory.hpp>

JobService::JobService(
    std::shared_ptr<IJobRepository> repo,
    std::shared_ptr<JobFactory> factory,
    std::shared_ptr<SchedulerDaemon> daemon
) : _repo(std::move(repo)), _factory(std::move(factory)), _daemon(std::move(daemon)) {}


JobData JobService::create_job(JobData data) {
    const std::string id = "id"; // leave a temp value for now later implement a uuid utility function
    data._id = id;
    data._status = JobStatus::ACTIVE;
    std::shared_ptr<Job> job = _factory->create_job(data);
    job->calculate_next_run();
    data._next_run = job->get_next_run();

    JobData saved_job = _repo->save_job(data);
    _daemon->add_job(job);

    return saved_job;
}

JobData JobService::pause_job(const std::string& id) {

}

JobData JobService::resumeJob(const std::string& id) {

}

int JobService::delete_job(const std::string& id) {

}