#include "service/JobRecoveryService.h"

JobRecoveryService::JobRecoveryService(
    std::shared_ptr<IJobRepository> repo,
    std::shared_ptr<JobFactory> factory,
    std::shared_ptr<SchedulerDaemon> daemon
) : repo(std::move(repo)), factory(std::move(factory)), daemon(std::move(daemon)) {}

void JobRecoveryService::execute() {
    std::vector<JobData> activeJobs = repo->findActive();

    auto now = std::chrono::time_point_cast<std::chrono::seconds>(
        std::chrono::system_clock::now()
    );

    for (auto& dto : activeJobs) {
        std::unique_ptr<Job> job = factory->createJob(dto);

        std::chrono::sys_seconds storedNextRun{
            std::chrono::seconds(dto.next_run)
        };

        if (storedNextRun < now) {
            std::chrono::sys_seconds newNextRun = job->calculateNextRun(now);
            dto.next_run = newNextRun.time_since_epoch().count();
            repo->updateJob(dto);
        }

        daemon->addJob(dto);
    }
}