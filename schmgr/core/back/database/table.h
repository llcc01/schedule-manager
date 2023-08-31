#ifndef SCHMGR_CORE_DATABASE_TABLE_H_
#define SCHMGR_CORE_DATABASE_TABLE_H_

#include <set>

#include "schmgr/utils/md5.h"

#include "schmgr/core/mtl/mtl.h"
#include "schmgr/core/back/database/model.h"
#include "schmgr/core/back/database/db.h"

namespace core::database
{

// 课程信息
class CourseTable : public Table
{
public:
    Index<mtl::string, int64_t> nameIndex;

    bool CalcIndex();
    int64_t AddCourse(const Course& course)
    {
        auto res = AddRec(course.ToRecord());
        if (res == -1)
        {
            return -1;
        }
        AddIndex(nameIndex, course.GetUid(), course.GetName());
        return res;
    }
    bool DeleteCourse(int64_t uid)
    {
        auto res = DeleteRec(uid);
        if (res == false)
        {
            return false;
        }
        RemoveIndex(nameIndex, uid);
        return true;
    }
    bool UpdateCourse(const Course& course)
    {
        auto res = UpdateRec(course.ToRecord());
        if (res == false)
        {
            return false;
        }
        RemoveIndex(nameIndex, course.GetUid());
        AddIndex(nameIndex, course.GetUid(), course.GetName());
        return true;
    }
};

// 课表
class ClassTable : public Table
{
public:
    Index<int64_t, int64_t> courseIndex;
    Index<time_t, int64_t> startTimeIndex;
    Index<int64_t, int64_t> weekIndex;

    ClassTable() { SetInfo(TableInfo("class")); };
    // ~ClassTable();

    bool CalcIndex();
    bool CalcIndexFilter(const std::set<int64_t>& courseIds);

    int64_t AddClass(const Class& cls);
    bool DeleteClass(int64_t uid);
    bool UpdateClass(const Class& cls);
};

// 个人事物表
class SchTable : public Table
{
public:
    Index<int64_t, int64_t> linkIndex;
    Index<time_t, int64_t> startTimeIndex;
    Index<time_t, int64_t> endTimeIndex;
    Index<mtl::string, int64_t> tagsIndex;
    Index<mtl::string, int64_t> nameIndex;

    bool CalcIndex();

    bool AddSch(const Schedule& sch);
    bool DeleteSch(int64_t uid);
    bool UpdateSch(const Schedule& sch);
};

// 用户表
class UserTable : public Table
{
public:
    Index<mtl::string, int64_t> nameIndex;
    Index<int64_t, int64_t> classIndex;
    bool CalcIndex() { return true; };

    bool Check(int64_t uid, const mtl::string& password) {
        auto r = Find(uid);
        if (r.size() == 0)
            return false;
        return utils::md5(password) == r[User::PasswordHashCol];
    }
    mtl::string GetPw(int64_t uid) {
        auto r = Find(uid);
        if (r.size() == 0)
            return "";
        return r[User::PasswordHashCol];
    }

    mtl::string GetName(int64_t uid) {
        auto r = Find(uid);
        if (r.size() == 0)
            return "";
        return r[User::NameCol];
    };

    int64_t AddUser(const User& user)
    {
        auto res = AddRec(user.ToRecord());
        if (res == -1)
        {
            return -1;
        }
        AddIndex(nameIndex, user.GetUid(), user.GetName());
        return res;
    }

};


// 考表
class ExamTable : public Table
{
public:
    Index<int64_t, int64_t> courseIndex;
public:
    bool CalcIndex();
    bool CalcIndexFilter(const std::set<int64_t>& courseIds);

    int64_t AddExam(const Exam& exam)
    {
        auto res = AddRec(exam.ToRecord());
        if (res == -1)
        {
            return -1;
        }
        AddIndex(courseIndex, exam.GetUid(), exam.GetCourseId());
        return res;
    }
    bool DeleteExam(int64_t uid)
    {
        auto res = DeleteRec(uid);
        if (res == false)
        {
            return false;
        }
        RemoveIndex(courseIndex, uid);
        return true;
    }
    bool UpdateExam(const Exam& exam)
    {
        auto res = UpdateRec(exam.ToRecord());
        if (res == false)
        {
            return false;
        }
        RemoveIndex(courseIndex, exam.GetUid());
        AddIndex(courseIndex, exam.GetUid(), exam.GetCourseId());
        return true;
    }
};

// 集体活动表，弃用
class GroupActivityTable : public Table
{
public:
    bool CalcIndex() { return true; };
};

// 闹钟表
class AlarmTable : public Table
{
public:
    bool CalcIndex() { return true; };

    bool AddAlarm(const Alarm& alarm);
    bool DeleteAlarm(int64_t uid);
    bool UpdateAlarm(const Alarm& alarm);

    void ToAlarms(mtl::map<int64_t, Alarm>& alarms);
    void FromAlarms(const mtl::map<int64_t, Alarm>& alarms);
};

} // namespace core::database

#endif