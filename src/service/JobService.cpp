#include "service/JobService.h"
#include "exceptions/JobNotFoundException.h"


JobService::JobService(
    std::shared_ptr<IJobRepository> repo,
    std::shared_ptr<JobFactory> factory,
    std::shared_ptr<SchedulerDaemon> daemon
) : _repo(std::move(repo)), _factory(std::move(factory)), _daemon(std::move(daemon)) {}


JobData JobService::create_job(JobData data) {
    const std::string id = "id"; // leave a temp value for now later implement a uuid utility function
    data._id = id;
    data._status = JobStatus::ACTIVE;
    const std::shared_ptr<Job> job = _factory->create_job(data);
    job->calculate_next_run();
    data._next_run = job->get_next_run();

    JobData saved_job = _repo->save_job(data);
    _daemon->add_job(job);

    return saved_job;
}

JobData JobService::pause_job(const std::string& id) {
    std::optional<JobData> data = _repo->find_by_id(id);
    if (!data.has_value()) {
        throw std::invalid_argument("invalid job id");
    }

    data.value()._status = JobStatus::PAUSED;
    _daemon->remove_job(id);

    return _repo->update_job(data.value());
}

JobData JobService::resume_job(const std::string& id) {
    std::optional<JobData> data = _repo->find_by_id(id);
    if (!data.has_value()) {
        throw std::invalid_argument("invalid job id");
    }

    data.value()._status = JobStatus::ACTIVE;
    const std::shared_ptr<Job> job = _factory->create_job(data.value());
    job->calculate_next_run();
    data.value()._next_run = job->get_next_run();
    // the daemon will check internally if the job with this id already in the queue or not
    _daemon->add_job(job);

    return _repo->update_job(data.value());
}

JobData JobService::update_status(const std::string& id, JobStatus status) {
    std::optional<JobData> data = _repo->find_by_id(id);
    if (!data.has_value()) {
        throw JobNotFoundException("invalid job id");
    }
    data.value()._status = status;
    return _repo->update_job(data.value());
}

JobData JobService::reschedule_job(const std::string& id) {
    std::optional<JobData> data = _repo->find_by_id(id);
    if (!data.has_value()) {
        throw JobNotFoundException(id);
    }

    data.value()._status = JobStatus::ACTIVE;
    const std::shared_ptr<Job> job = _factory->create_job(data.value());
    job->calculate_next_run();
    data.value()._next_run = job->get_next_run();

    JobData updated = _repo->update_job(data.value());
    _daemon->add_job(job);
    return updated;
}


void JobService::delete_job(const std::string& id) {
    const int count = _repo->delete_job(id);
    if (!count) {
        throw std::invalid_argument("invalid job id");
    }

    _daemon->remove_job(id);
}

JobData JobService::get_job_by_id(const std::string& id) {
    auto job = _repo->find_by_id(id);

    if (!job.has_value()) {
        throw JobNotFoundException("Job not found");
    }

    return job.value();
}

std::vector<JobData> JobService::get_all_jobs() {
    return _repo->find_all();
}