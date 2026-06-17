#include "service/JobRecoveryService.h"

JobRecoveryService::JobRecoveryService(
    std::shared_ptr<IJobRepository> repo,
    std::shared_ptr<JobFactory> factory,
    std::shared_ptr<SchedulerDaemon> daemon
) : _repo(std::move(repo)), _factory(std::move(factory)), _daemon(std::move(daemon)) {}

void JobRecoveryService::execute() {
    std::vector<JobData> active_jobs = _repo->find_active();

    auto now = std::chrono::time_point_cast<std::chrono::seconds>(
        std::chrono::system_clock::now()
    );

    for (auto& dto : active_jobs) {
        std::shared_ptr<Job> job = _factory->create_job(dto);
        const auto stored_next_run= job->get_next_run();

        if (stored_next_run < now) {
            // one time job — time has passed, mark as completed and skip
            if (job->get_schedule_type() == ScheduleType::ONETIME) {
                dto._status = JobStatus::COMPLETED;
                job->set_status(JobStatus::COMPLETED);
                _repo->update_job(dto);
                continue;
            }
            // calculate_next_run will save the next run internally in the job object
            const std::chrono::sys_seconds new_next_run = job->calculate_next_run();
            dto._next_run = new_next_run;
            _repo->update_job(dto);
        }

        _daemon->add_job(job);
    }
}