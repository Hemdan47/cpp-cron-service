#pragma once
#include <string>
#include <chrono>

#include "JobData.h"
#include "domain/ScheduleType.h"
#include "domain/JobStatus.h"


class Job {
public:

    Job(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload)
        : _id(id) , _name(name) , _type(type), _command(command),
          _status(status), _schedule_payload(schedule_payload){}

    explicit Job(const JobData &data) {
        this->_id = data._id;
        this->_name = data._name;
        this->_type = data._type;
        this->_command = data._command;
        this->_status = data._status;
        this->_next_run = data._next_run;
        this->_schedule_payload = data._schedule_payload;
    }

    virtual std::chrono::sys_seconds calculate_next_run() = 0;

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

    std::chrono::sys_seconds get_next_run() {
        return _next_run;
    }

    void set_next_run(std::chrono::sys_seconds next_run) {
        this->_next_run = next_run;
    }

    std::string get_schedule_payload() const {
        return _schedule_payload;
    }

    void set_schedule_payload(const std::string &schedule_payload) {
        this->_schedule_payload = schedule_payload;
    }

    virtual ~Job() = default;



private:
    std::string  _id;
    std::string  _name;
    ScheduleType _type;
    std::string  _command;
    JobStatus    _status;
    std::chrono::sys_seconds _next_run;
    std::string  _schedule_payload;
};