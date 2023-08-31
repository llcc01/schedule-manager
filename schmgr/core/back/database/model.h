#ifndef SCHMGR_CORE_DATABASE_MODEL_H_
#define SCHMGR_CORE_DATABASE_MODEL_H_

#include <vector>

#include "schmgr/shared/common/client_app.h"
#include "schmgr/core/mtl/mtl.h"
#include "schmgr/core/back/database/db.h"

namespace core::database
{

// enum Gender
// {
//     Man,
//     Women,
// };


class Schedule
{
private:
    int64_t uid_ = 0;
    int64_t linkId_;
    time_t startTime_;
    time_t endTime_;
    mtl::string name_;
    mtl::string loc_;
    mtl::string tags_;
    bool stack_;

public:
    Schedule() {};
    Schedule(int64_t uid, int64_t linkId, time_t startTime, time_t endTime, mtl::string name, mtl::string loc, mtl::string tags, bool stack)
        : uid_(uid), linkId_(linkId), startTime_(startTime), endTime_(endTime), name_(name), loc_(loc), tags_(tags), stack_(stack) {}
    Schedule(const Record&);
    Schedule(CefRefPtr<CefDictionaryValue> dict);
    ~Schedule() {};

    const Record ToRecord() const;
    CefRefPtr<CefDictionaryValue> ToCefDict() const;

    int64_t GetUid() const { return uid_; }
    int64_t GetLinkId() const { return linkId_; }
    time_t GetStartTime() const { return startTime_; }
    time_t GetEndTime() const { return endTime_; }
    const mtl::string& GetName() const { return name_; }
    const mtl::string& GetLoc() const { return loc_; }
    const mtl::string& GetTags() const { return tags_; }
    bool GetStack() const { return stack_; }

    void SetUid(int64_t uid) { uid_ = uid; }
    void SetLinkId(int64_t linkId) { linkId_ = linkId; }
    void SetStartTime(time_t startTime) { startTime_ = startTime; }
    void SetEndTime(time_t endTime) { endTime_ = endTime; }
    void SetName(mtl::string name) { name_ = name; }
    void SetLoc(mtl::string loc) { loc_ = loc; }
    void SetTags(mtl::string tags) { tags_ = tags; }
    void SetStack(bool stack) { stack_ = stack; }

    enum Col
    {
        UidCol,
        LinkIdCol,
        StartTimeCol,
        EndTimeCol,
        NameCol,
        LocCol,
        TagsCol,
        StackCol,
    };
    static const mtl::vector<mtl::string> ColNames;
};

class Class
{

private:
    int64_t uid = 0;
    int64_t courseId;
    time_t startTime;
    time_t endTime;
    uint64_t week;
    mtl::string loc;

public:
    Class(int64_t uid, int64_t courseId, time_t startTime, time_t endTime, uint64_t week, mtl::string loc)
        : uid(uid), courseId(courseId), startTime(startTime), endTime(endTime), week(week), loc(loc) {}
    Class(const Record&);
    Class(CefRefPtr<CefDictionaryValue> dict);

    CefRefPtr<CefDictionaryValue> ToCefDict() const;
    const Record ToRecord() const;

    int64_t GetUid() const { return uid; }
    int64_t GetCourseId() const { return courseId; }
    time_t GetStartTime() const { return startTime; }
    time_t GetEndTime() const { return endTime; }
    uint64_t GetWeek() const { return week; }
    const mtl::string& GetLoc() const { return loc; }

public:
    enum Col
    {
        UidCol,
        CourseIdCol,
        StartTimeCol,
        EndTimeCol,
        WeekCol,
        LocCol,
    };
    static const mtl::vector<mtl::string> ColNames;
};

class User
{
private:
    int64_t uid = 0;
    mtl::string name;
    int64_t groupId;
    mtl::string passwordHash;
    mtl::vector<int64_t> courseIds;
public:
    User(int64_t uid, mtl::string name, int64_t groupId, mtl::string passwordHash, mtl::vector<int64_t> courseIds)
        : uid(uid), name(name), groupId(groupId), passwordHash(passwordHash), courseIds(courseIds) {}
    User(const Record&);
    User(CefRefPtr<CefDictionaryValue> dict);
    Record ToRecord() const;

    int64_t GetUid() const { return uid; }
    const mtl::string& GetName() const { return name; }
    int64_t GetGroupId() const { return groupId; }
    const mtl::string& GetPasswordHash() const { return passwordHash; }
    const mtl::vector<int64_t>& GetCourseIds() const { return courseIds; }


public:
    enum Col
    {
        UidCol,
        NameCol,
        GroupIdCol,
        PasswordHashCol,
        CourseIdsCol,
    };
    static const mtl::vector<mtl::string> ColNames;
};


class Course
{
private:
    int64_t uid = 0;
    mtl::string name;
    mtl::vector<mtl::string> teacherNames;
    mtl::vector<int64_t> examIds;
    mtl::string extra;
public:
    Course(int64_t uid, mtl::string name, mtl::vector<mtl::string> teacherNames, mtl::vector<int64_t> examIds, mtl::string extra)
        : uid(uid), name(name), teacherNames(teacherNames), examIds(examIds), extra(extra) {}
    Course(const Record&);
    Course(CefRefPtr<CefDictionaryValue> dict);

    const Record ToRecord() const;
    CefRefPtr<CefDictionaryValue> ToCefDict() const;

    int64_t GetUid() const { return uid; }
    const mtl::string& GetName() const { return name; }
    const mtl::vector<mtl::string>& GetTeacherIds() const { return teacherNames; }
    const mtl::vector<int64_t>& GetExamIds() const { return examIds; }
    const mtl::string& GetExtra() const { return extra; }

public:
    enum Col
    {
        UidCol,
        NameCol,
        TeacherNamesCol,
        ExamIdsCol,
        ExtraCol,
    };
    static const mtl::vector<mtl::string> ColNames;
};

class Exam
{
private:
    int64_t uid = 0;
    int64_t courseId;
    mtl::string name;
    time_t startTime;
    time_t endTime;
    mtl::string loc;
    mtl::string extra;

public:
    Exam(const Record&);
    Exam(int64_t uid, int64_t courseId, mtl::string name, time_t startTime, time_t endTime, mtl::string loc, mtl::string extra)
        : uid(uid), courseId(courseId), name(name), startTime(startTime), endTime(endTime), loc(loc), extra(extra) {}
    Exam(CefRefPtr<CefDictionaryValue> dict);

    const Record ToRecord() const;
    CefRefPtr<CefDictionaryValue> ToCefDict() const;

    int64_t GetUid() const { return uid; }
    int64_t GetCourseId() const { return courseId; }
    const mtl::string& GetName() const { return name; }
    time_t GetStartTime() const { return startTime; }
    time_t GetEndTime() const { return endTime; }
    const mtl::string& GetLoc() const { return loc; }
    const mtl::string& GetExtra() const { return extra; }

public:
    enum Col
    {
        UidCol,
        CourseIdCol,
        NameCol,
        StartTimeCol,
        EndTimeCol,
        LocCol,
        ExtraCol,
    };
    static const mtl::vector<mtl::string> ColNames;
};

class Alarm
{
private:
    int64_t uid;
    int64_t linkId;
    time_t time;
    time_t interval;
    mtl::string loc;
    bool online;
    mtl::string name;
public:
    Alarm(const Record&);
    Alarm(int64_t uid, int64_t linkId, time_t time, time_t interval, mtl::string loc, bool online, mtl::string name)
        : uid(uid), linkId(linkId), time(time), interval(interval), loc(loc), online(online), name(name) {}
    Alarm(CefRefPtr<CefDictionaryValue> dict);

    const Record ToRecord() const;
    CefRefPtr<CefDictionaryValue> ToCefDict() const;

    int64_t GetUid() const { return uid; }
    int64_t GetLinkId() const { return linkId; }
    time_t GetTime() const { return time; }
    void SetTime(time_t t) { time = t; }
    time_t GetInterval() const { return interval; }

public:
    enum Col
    {
        UidCol,
        LinkIdCol,
        TimeCol,
        IntervalCol,
        LocCol,
        OnlineCol,
        NameCol,
    };
    static const mtl::vector<mtl::string> ColNames;

};

} // namespace core::database




#endif // SCHMGR_CORE_DATABASE_MODEL_H_