#include "schmgr/core/utils.h"
#include "schmgr/core/mtl/mtl.h"
#include "schmgr/core/back/database/db.h"
#include "schmgr/core/back/database/model.h"
#include "schmgr/core/back/database/table.h"

#include "schmgr/core/back/api_back.h"


namespace core::database
{

bool ClassTable::CalcIndex()
{
    courseIndex.clear();
    startTimeIndex.clear();
    weekIndex.clear();
    const mtl::map<int64_t, Record>& rs = GetRecords();
    for (auto it = rs.begin(); it != rs.end(); ++it)
    {
        courseIndex.insert(std::make_pair(std::stoll(it->second[Class::CourseIdCol]), it->first));
        startTimeIndex.insert(std::make_pair(std::stoll(it->second[Class::StartTimeCol]), it->first));
        uint64_t week = std::stoll(it->second[Class::WeekCol]);
        for (size_t i = 0; i < 64; i++)
            if (week & (1LL << i))
                weekIndex.insert(std::make_pair(i, it->first));
    }
    return true;
}

bool ClassTable::CalcIndexFilter(const std::set<int64_t>& courseIds)
{
    courseIndex.clear();
    startTimeIndex.clear();
    weekIndex.clear();
    const mtl::map<int64_t, Record>& rs = GetRecords();
    for (auto it = rs.begin(); it != rs.end(); ++it)
    {
        // BACK_DBG_LOG(std::to_string(it->first));
        int64_t courseId = std::stoll(it->second[Class::CourseIdCol]);
        if (courseIds.find(courseId) == courseIds.end())
            continue;
        courseIndex.insert(std::make_pair(courseId, it->first));
        startTimeIndex.insert(std::make_pair(std::stoll(it->second[Class::StartTimeCol]), it->first));
        uint64_t week = std::stoll(it->second[Class::WeekCol]);
        for (size_t i = 0; i < 64; i++)
        {
            if (week & (1LL << i))
            {
                weekIndex.insert(std::make_pair(i, it->first));
                // BACK_DBG_LOG("weekIndex.insert"+std::to_string(i)+","+std::to_string(it->first));
            }
        }
    }
    return true;
}

int64_t ClassTable::AddClass(const Class& cls)
{
    auto res = AddRec(cls.ToRecord());
    if (res == -1)
    {
        return -1;
    }
    AddIndex(courseIndex, cls.GetUid(), cls.GetCourseId());
    AddIndex(startTimeIndex, cls.GetUid(), cls.GetStartTime());
    uint64_t week = cls.GetWeek();
    for (size_t i = 0; i < 64; i++)
    {
        if (week & (1ULL << i))
        {
            AddIndex(weekIndex, i, cls.GetUid());
        }
    }
    return res;
}
bool ClassTable::DeleteClass(int64_t uid)
{
    auto r = Find(uid);
    auto res = DeleteRec(uid);
    if (res == false)
    {
        return false;
    }
    RemoveIndex(courseIndex, uid);
    RemoveIndex(startTimeIndex, uid);
    // back::sysLogger.Log("ClassTable", "DeleteClass: " + std::to_string(uid));
    // back::sysLogger.Log("ClassTable", "DeleteClass: week " + r[Class::WeekCol]);
    uint64_t week = std::stoll(r[Class::WeekCol]);
    for (size_t i = 0; i < 64; i++)
    {
        if (week & (1ULL << i))
        {
            RemoveIndex(weekIndex, uid);
        }
    }
    return true;
}
bool ClassTable::UpdateClass(const Class& cls)
{
    auto res = UpdateRec(cls.ToRecord());
    if (res == false)
    {
        return false;
    }
    RemoveIndex(courseIndex, cls.GetUid());
    RemoveIndex(startTimeIndex, cls.GetUid());
    auto r = Find(cls.GetUid());
    uint64_t week = std::stoll(r[Class::WeekCol]);
    for (size_t i = 0; i < 64; i++)
    {
        if (week & (1ULL << i))
        {
            RemoveIndex(weekIndex, cls.GetUid());
        }
    }
    AddIndex(courseIndex, cls.GetUid(), cls.GetCourseId());
    AddIndex(startTimeIndex, cls.GetUid(), cls.GetStartTime());
    week = cls.GetWeek();
    for (size_t i = 0; i < 64; i++)
    {
        if (week & (1LL << i))
        {
            AddIndex(weekIndex, i, cls.GetUid());
        }
    }
    return true;
}

bool SchTable::CalcIndex()
{
    linkIndex.clear();
    startTimeIndex.clear();
    endTimeIndex.clear();
    tagsIndex.clear();
    nameIndex.clear();
    const mtl::map<int64_t, Record>& rs = GetRecords();
    for (auto it = rs.begin(); it != rs.end(); ++it)
    {
        auto uid = it->first;
        AddIndex(linkIndex, uid, std::stoll(it->second[Schedule::LinkIdCol]));
        AddIndex(startTimeIndex, uid, time_t(std::stoll(it->second[Schedule::StartTimeCol])));
        AddIndex(endTimeIndex, uid, time_t(std::stoll(it->second[Schedule::EndTimeCol])));
        AddIndex(tagsIndex, uid, it->second[Schedule::TagsCol]);
        AddIndex(nameIndex, uid, it->second[Schedule::NameCol]);
    }
    return true;
}

bool SchTable::AddSch(const Schedule& sch)
{
    for (auto slb = --startTimeIndex.lower_bound(sch.GetStartTime());
        slb != startTimeIndex.end() && slb->first <= sch.GetStartTime();++slb)
    {
        Record r = Find(slb->second);
        Schedule s(r);

        if (s.GetStack() && sch.GetStack())
        {
            continue;
        }

        if (s.GetEndTime() > sch.GetStartTime())
        {
            return false;
        }

    }

    auto sti = startTimeIndex.find(sch.GetStartTime());
    if (sti != startTimeIndex.end())
    {
        Record r = Find(sti->second);
        Schedule s(r);

        if (!(s.GetStack() && sch.GetStack()))
        {
            return false;
        }
    }

    auto sub = startTimeIndex.upper_bound(sch.GetStartTime());
    if (sub != startTimeIndex.end() && sub->first < sch.GetEndTime())
    {
        Record r = Find(sub->second);
        Schedule s(r);

        if (!(s.GetStack() && sch.GetStack()))
        {
            return false;
        }
    }


    auto uid = AddRec(sch.ToRecord());
    if (uid == -1)
    {
        return false;
    }
    AddIndex(linkIndex, uid, sch.GetLinkId());
    AddIndex(startTimeIndex, uid, sch.GetStartTime());
    AddIndex(endTimeIndex, uid, sch.GetEndTime());
    AddIndex(tagsIndex, uid, sch.GetTags());
    AddIndex(nameIndex, uid, sch.GetName());
    return true;
}

bool SchTable::DeleteSch(int64_t uid)
{
    bool res = DeleteRec(uid);
    if (!res)
    {
        return false;
    }
    RemoveIndex(linkIndex, uid);
    RemoveIndex(startTimeIndex, uid);
    RemoveIndex(endTimeIndex, uid);
    RemoveIndex(tagsIndex, uid);
    RemoveIndex(nameIndex, uid);
    return true;
}

bool SchTable::UpdateSch(const Schedule& sch)
{
    auto oldSch = sch;
    auto uid = sch.GetUid();
    // BACK_DBG_LOG(std::to_string(uid));
    DeleteSch(uid);
    if (!AddSch(sch))
    {
        AddSch(oldSch);
        return false;
    }

    return true;
}

bool ExamTable::CalcIndex()
{
    courseIndex.clear();
    const mtl::map<int64_t, Record>& rs = GetRecords();
    for (auto it = rs.begin(); it != rs.end(); ++it)
    {
        courseIndex.insert(std::make_pair(std::stoll(it->second[Exam::CourseIdCol]), it->first));
    }
    return true;
}

bool ExamTable::CalcIndexFilter(const std::set<int64_t>& courseIds)
{
    courseIndex.clear();
    const mtl::map<int64_t, Record>& rs = GetRecords();
    for (auto it = rs.begin(); it != rs.end(); ++it)
    {
        // BACK_DBG_LOG(std::to_string(it->first));
        int64_t courseId = std::stoll(it->second[Class::CourseIdCol]);
        if (courseIds.find(courseId) == courseIds.end())
            continue;
        courseIndex.insert(std::make_pair(courseId, it->first));
    }
    return true;
}

bool CourseTable::CalcIndex()
{
    nameIndex.clear();
    const mtl::map<int64_t, Record>& rs = GetRecords();
    for (auto it = rs.begin(); it != rs.end(); ++it)
    {
        nameIndex.insert(std::make_pair(it->second[Course::NameCol], it->first));
    }
    return true;
}

bool AlarmTable::AddAlarm(const Alarm& alarm)
{
    AddRec(alarm.ToRecord());
    return true;
}

bool AlarmTable::DeleteAlarm(int64_t uid)
{
    DeleteRec(uid);
    return true;
}

bool AlarmTable::UpdateAlarm(const Alarm& alarm)
{
    DeleteAlarm(alarm.GetUid());
    AddAlarm(alarm);
    return true;
}

void AlarmTable::ToAlarms(mtl::map<int64_t, Alarm>& alarms)
{
    const mtl::map<int64_t, Record>& rs = GetRecords();
    for (auto it = rs.begin(); it != rs.end(); ++it)
    {
        Alarm alarm(it->second);
        alarms.insert(std::make_pair(alarm.GetUid(), alarm));
    }
}

void AlarmTable::FromAlarms(const mtl::map<int64_t, Alarm>& alarms)
{
    Clear();
    for (auto it = alarms.begin(); it != alarms.end(); ++it)
    {
        AddAlarm(it->second);
    }
}

} // namespace core::database