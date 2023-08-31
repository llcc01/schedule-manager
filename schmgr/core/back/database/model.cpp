
#include "schmgr/core/back/database/model.h"
#include "schmgr/core/utils.h"
#include "schmgr/utils/string.h"


namespace core::database
{

// Schedule

const mtl::vector<mtl::string> Schedule::ColNames = {
    "id",
    "linkId",
    "startTime",
    "endTime",
    "name",
    "loc",
    "tags",
    "stack",
};

Schedule::Schedule(const Record& r)
{
    uid_ = std::stoll(r[UidCol]);
    linkId_ = std::stoll(r[LinkIdCol]);
    startTime_ = std::stoll(r[StartTimeCol]);
    endTime_ = std::stoll(r[EndTimeCol]);
    name_ = r[NameCol];
    loc_ = r[LocCol];
    tags_ = r[TagsCol];
    stack_ = std::stoi(r[StackCol]);
}

Schedule::Schedule(CefRefPtr<CefDictionaryValue> dict)
{
    uid_ = CefValToI64(dict, ColNames[UidCol]);
    linkId_ = CefValToI64(dict, ColNames[LinkIdCol]);
    startTime_ = CefValToI64(dict, ColNames[StartTimeCol]);
    endTime_ = CefValToI64(dict, ColNames[EndTimeCol]);
    name_ = dict->GetString(ColNames[NameCol]);
    loc_ = dict->GetString(ColNames[LocCol]);
    tags_ = dict->GetString(ColNames[TagsCol]);
    stack_ = dict->GetBool(ColNames[StackCol]);
}

const Record Schedule::ToRecord() const
{
    Record r;
    r.push_back(std::to_string(uid_));
    r.push_back(std::to_string(linkId_));
    r.push_back(std::to_string(startTime_));
    r.push_back(std::to_string(endTime_));
    r.push_back(name_);
    r.push_back(loc_);
    r.push_back(tags_);
    r.push_back(std::to_string(stack_));
    return r;
}

CefRefPtr<CefDictionaryValue> Schedule::ToCefDict() const
{
    CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
    dict->SetDouble(ColNames[UidCol], uid_);
    dict->SetDouble(ColNames[LinkIdCol], linkId_);
    dict->SetDouble(ColNames[StartTimeCol], startTime_);
    dict->SetDouble(ColNames[EndTimeCol], endTime_);
    dict->SetString(ColNames[NameCol], name_);
    dict->SetString(ColNames[LocCol], loc_);
    dict->SetString(ColNames[TagsCol], tags_);
    dict->SetBool(ColNames[StackCol], stack_);
    return dict;
}

// Class
const mtl::vector<mtl::string> Class::ColNames = {
    "id",
    "courseId",
    "startTime",
    "endTime",
    "weeks",
    "loc",
};

Class::Class(const Record& r)
{
    uid = std::stoll(r[UidCol]);
    courseId = std::stoll(r[CourseIdCol]);
    startTime = std::stoll(r[StartTimeCol]);
    endTime = std::stoll(r[EndTimeCol]);
    week = std::stoll(r[WeekCol]);
    loc = r[LocCol];
}

Class::Class(CefRefPtr<CefDictionaryValue> dict)
{
    uid = CefValToI64(dict, ColNames[UidCol]);
    courseId = CefValToI64(dict, ColNames[CourseIdCol]);
    startTime = CefValToI64(dict, ColNames[StartTimeCol]);
    endTime = CefValToI64(dict, ColNames[EndTimeCol]);
    auto weekList = dict->GetList(ColNames[WeekCol]);
    week = 0;
    for (size_t i = 0; i < weekList->GetSize(); ++i)
    {
        week |= (1ULL << uint64_t(weekList->GetDouble(i)));
    }
    loc = dict->GetString(ColNames[LocCol]);
}

const Record Class::ToRecord() const
{
    Record r;
    r.push_back(std::to_string(uid));
    r.push_back(std::to_string(courseId));
    r.push_back(std::to_string(startTime));
    r.push_back(std::to_string(endTime));
    r.push_back(std::to_string(week));
    r.push_back(loc);
    return r;
}

CefRefPtr<CefDictionaryValue> Class::ToCefDict() const
{
    CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
    dict->SetDouble(ColNames[UidCol], uid);
    dict->SetDouble(ColNames[CourseIdCol], courseId);
    dict->SetDouble(ColNames[StartTimeCol], startTime);
    dict->SetDouble(ColNames[EndTimeCol], endTime);

    CefRefPtr<CefListValue> weekList = CefListValue::Create();
    for (size_t i = 0; i < 64; i++)
    {
        if (week & (1ULL << i))
        {
            auto v = CefValue::Create();
            v->SetDouble(i);
            weekList->SetValue(weekList->GetSize(), v);
        }
    }
    dict->SetList(ColNames[WeekCol], weekList);

    dict->SetString(ColNames[LocCol], loc);
    return dict;
}

// User

const mtl::vector<mtl::string> User::ColNames = {
    "id",
    "name",
    "groupId",
    "courseIds",
    "password",
};

User::User(const Record& r)
{
    uid = std::stoll(r[UidCol]);
    name = r[NameCol];
    groupId = std::stoll(r[GroupIdCol]);
    courseIds = r.GetVectorData(CourseIdsCol);
    passwordHash = utils::md5(r[PasswordHashCol]);
}

User::User(CefRefPtr<CefDictionaryValue> dict)
{
    uid = CefValToI64(dict, ColNames[UidCol]);
    name = dict->GetString(ColNames[NameCol]);
    groupId = CefValToI64(dict, ColNames[GroupIdCol]);
    auto courseIdList = dict->GetList(ColNames[CourseIdsCol]);
    passwordHash = utils::md5(dict->GetString(ColNames[PasswordHashCol]));
    for (size_t i = 0;i < courseIdList->GetSize();++i)
    {
        courseIds.push_back(courseIdList->GetDouble(i));
    }
}

Record User::ToRecord() const
{
    Record r;
    r.push_back(std::to_string(uid));
    r.push_back(name);
    r.push_back(std::to_string(groupId));
    r.push_back(passwordHash);
    r.push_back("");
    r.SetVectorData(CourseIdsCol, courseIds);
    return r;
}

// Course

const mtl::vector<mtl::string> Course::ColNames = {
    "id",
    "name",
    "teacherNames",
    "examIds",
    "extra",
};

Course::Course(const Record & r)
{
    uid = std::stoll(r[UidCol]);
    name = r[NameCol];
    teacherNames = r.GetVectorString(TeacherNamesCol);
    examIds = r.GetVectorData(ExamIdsCol);
    extra = r[ExtraCol];
}

Course::Course(CefRefPtr<CefDictionaryValue> dict)
{
    uid = CefValToI64(dict, ColNames[UidCol]);
    name = dict->GetString(ColNames[NameCol]);
    auto teacherList = dict->GetList(ColNames[Col::TeacherNamesCol]);
    for (size_t i = 0;i < teacherList->GetSize();++i)
    {
        teacherNames.push_back(teacherList->GetString(i));
    }
    auto examIdList = dict->GetList(ColNames[Col::ExamIdsCol]);
    for (size_t i = 0;i < examIdList->GetSize();++i)
    {
        examIds.push_back(examIdList->GetDouble(i));
    }
    extra = dict->GetString(ColNames[ExtraCol]);
}

const Record Course::ToRecord() const
{
    Record r;
    r.push_back(std::to_string(uid));
    r.push_back(name);
    r.push_back("");
    r.SetVectorString(TeacherNamesCol, teacherNames);
    r.push_back("");
    r.SetVectorData(ExamIdsCol, examIds);
    r.push_back(extra);
    return r;
}

CefRefPtr<CefDictionaryValue> Course::ToCefDict() const
{
    CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
    dict->SetDouble(ColNames[UidCol], uid);
    dict->SetString(ColNames[NameCol], name);
    CefRefPtr<CefListValue> teacherList = CefListValue::Create();
    for (size_t i = 0;i < teacherNames.size();++i)
    {
        teacherList->SetString(i, teacherNames[i]);
    }
    dict->SetList(ColNames[TeacherNamesCol], teacherList);
    CefRefPtr<CefListValue> examIdList = CefListValue::Create();
    for (size_t i = 0;i < examIds.size();++i)
    {
        examIdList->SetDouble(i, examIds[i]);
    }
    dict->SetList(ColNames[ExamIdsCol], examIdList);
    dict->SetString(ColNames[ExtraCol], extra);
    return dict;
}

// Exam
const mtl::vector<mtl::string> Exam::ColNames = {
    "id",
    "courseId",
    "name",
    "startTime",
    "endTime",
    "loc",
    "extra",
};

Exam::Exam(const Record& r)
{
    uid = std::stoll(r[UidCol]);
    courseId = std::stoll(r[CourseIdCol]);
    name = r[NameCol];
    startTime = std::stoll(r[StartTimeCol]);
    endTime = std::stoll(r[EndTimeCol]);
    loc = r[LocCol];
    extra = r[ExtraCol];
}

Exam::Exam(CefRefPtr<CefDictionaryValue> dict)
{
    uid = CefValToI64(dict, ColNames[UidCol]);
    courseId = CefValToI64(dict, ColNames[CourseIdCol]);
    name = dict->GetString(ColNames[NameCol]);
    startTime = CefValToI64(dict, ColNames[StartTimeCol]);
    endTime = CefValToI64(dict, ColNames[EndTimeCol]);
    loc = dict->GetString(ColNames[LocCol]);
    extra = dict->GetString(ColNames[ExtraCol]);
}

const Record Exam::ToRecord() const
{
    Record r;
    r.push_back(std::to_string(uid));
    r.push_back(std::to_string(courseId));
    r.push_back(name);
    r.push_back(std::to_string(startTime));
    r.push_back(std::to_string(endTime));
    r.push_back(loc);
    r.push_back(extra);
    return r;
}

CefRefPtr<CefDictionaryValue> Exam::ToCefDict() const
{
    CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
    dict->SetDouble(ColNames[UidCol], uid);
    dict->SetDouble(ColNames[CourseIdCol], courseId);
    dict->SetString(ColNames[NameCol], name);
    dict->SetDouble(ColNames[StartTimeCol], startTime);
    dict->SetDouble(ColNames[EndTimeCol], endTime);
    dict->SetString(ColNames[LocCol], loc);
    dict->SetString(ColNames[ExtraCol], extra);
    return dict;
}

// Alarm
const mtl::vector<mtl::string> Alarm::ColNames = {
    "id",
    "linkId",
    "time",
    "interval",
    "loc",
    "online",
    "name"
};

Alarm::Alarm(const Record& r)
{
    uid = std::stoll(r[UidCol]);
    linkId = std::stoll(r[LinkIdCol]);
    time = std::stoll(r[TimeCol]);
    interval = std::stoll(r[IntervalCol]);
    loc = r[LocCol];
    online = std::stoi(r[OnlineCol]);
    name = r[NameCol];
}

Alarm::Alarm(CefRefPtr<CefDictionaryValue> dict)
{
    uid = CefValToI64(dict, ColNames[UidCol]);
    linkId = CefValToI64(dict, ColNames[LinkIdCol]);
    time = CefValToI64(dict, ColNames[TimeCol]);
    interval = CefValToI64(dict, ColNames[IntervalCol]);
    loc = dict->GetString(ColNames[LocCol]);
    online = dict->GetBool(ColNames[OnlineCol]);
    name = dict->GetString(ColNames[NameCol]);
}

const Record Alarm::ToRecord() const
{
    Record r;
    r.push_back(std::to_string(uid));
    r.push_back(std::to_string(linkId));
    r.push_back(std::to_string(time));
    r.push_back(std::to_string(interval));
    r.push_back(loc);
    r.push_back(std::to_string(online));
    r.push_back(name);
    return r;
}

CefRefPtr<CefDictionaryValue> Alarm::ToCefDict() const
{
    CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
    dict->SetDouble(ColNames[UidCol], uid);
    dict->SetDouble(ColNames[LinkIdCol], linkId);
    dict->SetDouble(ColNames[TimeCol], time);
    dict->SetDouble(ColNames[IntervalCol], interval);
    dict->SetString(ColNames[LocCol], loc);
    dict->SetBool(ColNames[OnlineCol], online);
    dict->SetString(ColNames[NameCol], name);
    return dict;
}




} // namespace core::database
