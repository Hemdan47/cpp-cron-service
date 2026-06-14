#pragma once
#include <string>
#include <chrono>

#include "JobData.h"
#include "domain/ScheduleType.h"
#include "domain/JobStatus.h"


class Job {
public:

    Job(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedulePayload)
        : _id(id) , _name(name) , _type(type), _command(command),
          _status(status), _schedulePayload(schedulePayload){}

    explicit Job(const JobData &data) {
        this->_id = data.id;
        this->_name = data.name;
        this->_type = data.type;
        this->_command = data.command;
        this->_status = data.status;
        this->_schedulePayload = data.schedulePayload;
    }

    virtual std::chrono::sys_seconds calculateNextRun(std::chrono::sys_seconds currentTime) = 0;
    virtual ScheduleType getType() const = 0;
    virtual std::string getSchedulePayload() const = 0 ;

    std::string get_id() const {
        return _id;
    }

    void set_id(const std::string &id) {
        this->_id = id;
    }

    std::string get_name() const {
        return _name;
    }

    void set_name(const std::string &name) {
        this->_name = name;
    }

    ScheduleType get_schedule_type() const {
        return _type;
    }

    void set_schedule_type(ScheduleType type) {
        this->_type = type;
    }

    std::string get_command() const {
        return _command;
    }

    void set_command(const std::string &command) {
        this->_command = command;
    }

    JobStatus get_status() const {
        return _status;
    }

    void set_status(JobStatus status) {
        this->_status = status;
    }

    std::string get_schedule_payload() const {
        return _schedulePayload;
    }

    void set_schedule_payload(const std::string &schedule_payload) {
        this->_schedulePayload = schedule_payload;
    }

    virtual ~Job() = default;



private:
    std::string  _id;
    std::string  _name;
    ScheduleType _type;
    std::string  _command;
    JobStatus    _status;
    std::string  _schedulePayload;
};