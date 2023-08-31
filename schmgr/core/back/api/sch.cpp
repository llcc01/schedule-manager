#include "schmgr/core/back/api/sch.h"
#include "schmgr/core/mtl/mtl.h"

namespace core::back::api::sch
{

const time_t kDayStart = 6 * 60 * 60;
const time_t kDayEnd = 22 * 60 * 60;
const time_t kDayTime = 24 * 60 * 60;

const mtl::vector<std::wstring> kGroupActivity = { L"班会", L"小组作业", L"创新创业", L"聚餐" };
const mtl::vector<std::wstring> kPersonActivity = { L"自习", L"锻炼", L"外出" };
const mtl::vector<std::wstring> kTempActivity = { L"购物",L"洗澡",L"取外卖",L"取快递",L"送取东西" };
const mtl::vector<std::string> kOrderName = { "name", "startTime", "endTime", "tags" };


bool isSomeActivity(const database::Schedule& s, const mtl::vector<std::wstring>& activityList)
{
    return std::find(activityList.begin(), activityList.end(), utils::to_wide_string(s.GetTags())) != activityList.end();
}

int16_t getWeekIndex(time_t time)
{
    time_t weekOffset = std::stoll(sysConfig["WeekOffset"]);
    int16_t weekIndex = (time - weekOffset) / (7 * 24 * 60 * 60);
    // BACK_DBG_LOG(std::to_string(time) + " " + std::to_string(weekIndex));
    return weekIndex;
}

// void getByDate(time_t t, CefRefPtr<CefListValue> list)
// {
//     tm* ltm = localtime(&t);
//     ltm->tm_hour = 0;
//     ltm->tm_min = 0;
//     ltm->tm_sec = 0;
//     time_t date = mktime(ltm);

//     auto& rs = schTable.GetRecords();
//     auto start = schTable.startTimeIndex.upper_bound(date);
//     auto end = schTable.startTimeIndex.upper_bound(date + 24 * 60 * 60);
//     size_t i = 0;
//     for (auto it = start;it != end;++it)
//     {
//         auto rec = rs.find(it->second);
//         if (rec == rs.end())
//         {
//             continue;
//         }

//         auto dict = database::Schedule(rec->second).ToCefDict();
//         // auto r = CefValue::Create();
//         // r->SetDictionary(dict);
//         // list->SetValue(i++, r);
//         list->SetDictionary(i++, dict);
//     }
// }

void getByWeek(time_t weekOffset, int16 week, CefRefPtr<CefListValue> list)
{
    time_t weekStart = weekOffset + week * 7 * 24 * 60 * 60;

    auto& rs = schTable.GetRecords();
    auto start = schTable.startTimeIndex.upper_bound(weekStart);
    auto end = schTable.startTimeIndex.upper_bound(weekStart + 7 * 24 * 60 * 60);
    size_t i = 0;
    for (auto it = start;it != end;++it)
    {
        auto dict = database::Schedule(rs.find(it->second)->second).ToCefDict();
        list->SetDictionary(i++, dict);
    }

    // auto& weekClassRs = classTable.GetRecords();
    // // BACK_DBG_LOG(std::to_string(classTable.weekIndex.size()));
    // auto weekClassStart = classTable.weekIndex.find(week);
    // if (weekClassStart == classTable.weekIndex.end())
    // {
    //     return;
    // }
    // auto weekClassEnd = classTable.weekIndex.upper_bound(week);
    // for (auto it = weekClassStart;it != weekClassEnd;++it)
    // {
    //     auto classOne = database::Class(weekClassRs.find(it->second)->second);
    //     auto course = database::Course(courseTable.Find(classOne.GetCourseId()));
    //     auto dict = database::Schedule(classOne.GetUid(), classOne.GetCourseId(),
    //         classOne.GetStartTime() + weekStart, classOne.GetEndTime() + weekStart,
    //         course.GetName(), classOne.GetLoc(), utils::to_byte_string(L"课程"), false).ToCefDict();
    //     list->SetDictionary(i++, dict);
    // }
}

time_t getDate(time_t time)
{
    tm* ltm = localtime(&time);
    ltm->tm_hour = 0;
    ltm->tm_min = 0;
    ltm->tm_sec = 0;
    return mktime(ltm);
}

std::wstring timeRange2Str(time_t start, time_t end)
{
    std::wstring msg;
    msg += utils::to_wide_string(StringTime(start));
    msg += L" - ";
    msg += utils::to_wide_string(StringTime(end));
    return msg;
};

std::wstring getAvailableTimeStr(const database::Schedule& s, database::SchTable& tab)
{
    time_t dateStart = getDate(s.GetStartTime());
    time_t duration = s.GetEndTime() - s.GetStartTime();
    time_t slotStart = dateStart + kDayStart;
    auto& rs = tab.GetRecords();

    std::wstring msg = L"";
    mtl::vector<std::pair<time_t, time_t>> slots; // start, duration

    for (;;)
    {
        auto it = tab.startTimeIndex.lower_bound(slotStart);
        if (it != tab.startTimeIndex.end())
        {
            int64_t nextSchId = it->second;
            auto nextSch = database::Schedule(rs.find(nextSchId)->second);
            if (nextSch.GetStartTime() >= dateStart + kDayEnd)
            {
                break;
            }
            if (nextSch.GetStartTime() > slotStart)
            {
                slots.push_back(std::make_pair(slotStart, nextSch.GetStartTime() - slotStart));
            }
            slotStart = nextSch.GetEndTime();
            continue;
        }
        break;
    }

    if (slotStart < dateStart + kDayEnd)
    {
        slots.push_back(std::make_pair(slotStart, dateStart + kDayEnd - slotStart));
    }

    if (slots.size() == 0)
    {
        msg += L"无可用时间";
        return msg;
    }

    // for (auto& tag : kGroupActivity)
    // {
    //     sysLogger.Log("INFO", "tag: " + utils::to_byte_string(tag));
    // }

    if (isSomeActivity(s, kGroupActivity))
    {
        mtl::map<time_t, std::pair<time_t, time_t>> sortedSlots;
        for (auto& slot : slots)
        {
            sortedSlots.insert(std::make_pair(-slot.first, slot));
        }

        for (auto& p : sortedSlots)
        {
            auto& slot = p.second;
            msg += timeRange2Str(slot.first, slot.first + slot.second) + L" 共" + std::to_wstring(slot.second / (60 * 60)) + L"小时\n";
        }

    }
    else
    {
        msg += L"可用时间段：\n";
        for (auto& slot : slots)
        {
            sysLogger.Log("INFO", "slot: " + std::to_string(slot.first) + " " + std::to_string(slot.second));
            if (slot.second >= duration)
            {
                msg += timeRange2Str(slot.first, slot.first + slot.second) + L" 共" + std::to_wstring(slot.second / (60 * 60)) + L"小时\n";
            }
        }
    }


    return msg;
}


// void GetToday(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
// {
//     auto list = CefListValue::Create();
//     auto now = sysClock.GetTime();

//     getByDate(now, list);

//     // auto data = CefValue::Create();
//     // data->SetList(list);
//     ApiCallback(cbi, makeRespoense(list));
// }

// void GetByDay(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
// {
//     auto list = CefListValue::Create();

//     auto dateStr = funArgs->GetString(0).ToString();

//     if (dateStr.size() == 10)
//     {
//         tm dateTm;
//         dateTm.tm_year = std::stoi(dateStr.substr(0, 4)) - 1900;
//         dateTm.tm_mon = std::stoi(dateStr.substr(5, 2)) - 1;
//         dateTm.tm_mday = std::stoi(dateStr.substr(8, 2));
//         time_t date = mktime(&dateTm);

//         getByDate(date, list);
//     }
//     ApiCallback(cbi, makeRespoense(list));
// }

void GetByWeek(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    auto list = CefListValue::Create();

    time_t date = CefValToI64(funArgs, 0);
    getByWeek(std::stoll(sysConfig["WeekOffset"]), getWeekIndex(date), list);

    ApiCallback(cbi, makeRespoense(list));
}

void GetCurrentWeek(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    auto list = CefListValue::Create();

    auto now = sysClock.GetTime();
    getByWeek(std::stoll(sysConfig["WeekOffset"]), getWeekIndex(now), list);

    ApiCallback(cbi, makeRespoense(list));
}

void Update(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{

    if (funArgs->GetSize() != 1)
    {
        ApiErrorCallback(cbi, "Invalid args");
        return;
    }

    auto dict = funArgs->GetDictionary(0);
    auto id = CefValToI64(dict, "id");
    if (id == 0)
    {
        ApiCallback(cbi, makeErrorRespoense("Invalid id 0"));
        return;
    }

    auto& rs = schTable.GetRecords();
    // for (auto it = rs.begin(); it != rs.end(); ++it)
    // {
    //     BACK_DBG_LOG(it->second.Dumps());
    // }
    auto it = rs.find(id);
    if (it == rs.end())
    {
        // BACK_DBG_LOG(std::to_string(id));
        ApiCallback(cbi, makeErrorRespoense("Not found"));
        return;
    }

    database::Schedule s(dict);
    if (isSomeActivity(s, kTempActivity))
    {
        s.SetStack(true);
    }

    if (s.GetStartTime() >= s.GetEndTime())
    {
        ApiCallback(cbi, makeErrorRespoense(L"时间有误"));
        return;
    }

    // sysLogger.Log("INFO", "Update schedule: " + s.ToRecord().Dumps());

    if (!schTable.UpdateSch(s))
    {
        // BACK_DBG_LOG(s.ToRecord().Dumps());
        // args->SetBool(false);
        // ApiCallback(cbi, args);
        std::wstring msg = getAvailableTimeStr(s, schTable);
        ApiCallback(cbi, makeErrorRespoense(L"时间冲突，修改失败\n" + msg));
        return;
    }

    ApiCallback(cbi, makeRespoense(dict));
}

void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    auto data = CefValue::Create();

    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    auto dict = funArgs->GetDictionary(0);
    // TODO 根据判断是否可重叠
    dict->SetDouble("id", GenUid());
    database::Schedule s(dict);
    if (isSomeActivity(s, kTempActivity))
    {
        s.SetStack(true);
    }

    if (s.GetStartTime() >= s.GetEndTime() || s.GetStartTime() < getDate(s.GetStartTime()) + kDayStart || s.GetEndTime() > getDate(s.GetEndTime()) + kDayEnd)
    {
        ApiCallback(cbi, makeErrorRespoense(L"时间有误"));
        return;
    }

    // sysLogger.Log("INFO", "Update schedule: " + s.ToRecord().Dumps());

    if (!schTable.AddSch(s))
    {
        std::wstring msg = getAvailableTimeStr(s, schTable);
        ApiCallback(cbi, makeErrorRespoense(L"时间冲突，添加失败\n" + msg));
        return;
    }

    // data->SetDictionary(dict);
    ApiCallback(cbi, makeRespoense(dict));
}

void Delete(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{

    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense("Invalid arguments"));
        return;
    }

    auto id = CefValToI64(funArgs, 0);

    if (!schTable.DeleteSch(id))
    {
        ApiCallback(cbi, makeErrorRespoense(L"ID 不存在，删除失败"));
        return;
    }

    ApiCallback(cbi, makeRespoense());
}

void GetById(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    auto id = CefValToI64(funArgs, 0);
    auto& rs = schTable.GetRecords();
    auto it = rs.find(id);
    if (it == rs.end())
    {
        ApiCallback(cbi, makeErrorRespoense(L"ID 不存在"));
        return;
    }

    auto dict = database::Schedule(it->second).ToCefDict();
    ApiCallback(cbi, makeRespoense(dict));
}

bool schFilterNameFirst(const database::Schedule& sch, const mtl::string& name, time_t startTime, time_t endTime, const mtl::string& tags)
{
    if (name.size() > 0 && sch.GetName().find(name) == mtl::string::npos)
    {
        return false;
    }
    if (startTime > 0 && sch.GetStartTime() < startTime)
    {
        return false;
    }
    if (endTime > 0 && sch.GetEndTime() > endTime)
    {
        return false;
    }
    if (tags.size() > 0 && tags.find(sch.GetTags()) == mtl::string::npos)
    {
        return false;
    }
    return true;
}

bool schFilterTimeFirst(const database::Schedule& sch, const mtl::string& name, time_t startTime, time_t endTime, const mtl::string& tags)
{
    if (startTime > 0 && sch.GetEndTime() < startTime)
    {
        return false;
    }
    if (endTime > 0 && sch.GetStartTime() > endTime)
    {
        return false;
    }
    if (name.size() > 0 && sch.GetName().find(name) == mtl::string::npos)
    {
        return false;
    }
    if (tags.size() > 0 && tags.find(sch.GetTags()) == mtl::string::npos)
    {
        return false;
    }
    return true;
}

bool schFilterTagsFirst(const database::Schedule& sch, const mtl::string& name, time_t startTime, time_t endTime, const mtl::string& tags)
{
    if (tags.size() > 0 && tags.find(sch.GetTags()) == mtl::string::npos)
    {
        return false;
    }
    if (startTime > 0 && sch.GetEndTime() < startTime)
    {
        return false;
    }
    if (endTime > 0 && sch.GetStartTime() > endTime)
    {
        return false;
    }
    if (name.size() > 0 && sch.GetName().find(name) == mtl::string::npos)
    {
        return false;
    }
    return true;
}

void GetByFilter(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    // TODO
    mtl::string name = funArgs->GetString(0).ToString();
    time_t startTime = funArgs->GetDouble(1);
    time_t endTime = funArgs->GetDouble(2);
    mtl::string tags = funArgs->GetString(3).ToString();
    mtl::string orderBy = funArgs->GetString(4).ToString(); // name, startTime, tags
    bool asc = funArgs->GetBool(5);

    // auto list = CefListValue::Create();
    // if (name.size() == 0 && startTime == 0 && endTime == 0 && tags.size() == 0)
    // {
    //     ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));
    //     return;
    // }

    auto& rs = schTable.GetRecords();
    size_t filterCount = 0;
    size_t searchIndex = 1;
    if (name.size() > 0)
    {
        filterCount++;
        searchIndex = 0;
    }
    if (startTime > 0)
    {
        filterCount++;
        searchIndex = 1;
    }
    if (tags.size() > 0)
    {
        filterCount++;
        searchIndex = 3;
    }

    if (filterCount > 1)
    {
        size_t size = kOrderName.size();
        for (size_t i = 0; i < size; i++)
        {
            if (orderBy == kOrderName[i])
            {
                searchIndex = i;
                break;
            }
        }
    }

    mtl::vector<database::Schedule> schList;
    if (searchIndex == 0)
    {
        // 名字搜索，全扫
        for (auto it = schTable.nameIndex.begin();it != schTable.nameIndex.end();++it)
        {
            database::Schedule sch(rs.find(it->second)->second);
            if (!schFilterNameFirst(sch, name, startTime, endTime, tags))
            {
                continue;
            }
            schList.push_back(sch);
        }
    }
    else if (searchIndex == 1)
    {
        // 开始时间搜索，使用索引
        auto start = schTable.startTimeIndex.lower_bound(startTime);
        auto end = schTable.startTimeIndex.upper_bound(endTime);
        for (auto it = start;it != end;++it)
        {
            database::Schedule sch(rs.find(it->second)->second);
            if (!schFilterTimeFirst(sch, name, startTime, endTime, tags))
            {
                continue;
            }
            schList.push_back(sch);
        }
    }
    else if (searchIndex == 3)
    {
        // 标签搜索，使用索引
        auto start = schTable.tagsIndex.lower_bound(tags);
        auto end = schTable.tagsIndex.upper_bound(tags);
        for (auto it = start;it != end;++it)
        {
            database::Schedule sch(rs.find(it->second)->second);
            if (!schFilterTagsFirst(sch, name, startTime, endTime, tags))
            {
                continue;
            }
            schList.push_back(sch);
        }
    }
    else
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));
        return;
    }

    mtl::vector<size_t> schListOrder;// schList的顺序
    size_t resSize = schList.size();
    // 排序
    if (orderBy == kOrderName[0] && searchIndex != 0)
    {
        mtl::multimap<mtl::string, size_t> nameIndex;
        for (size_t i = 0; i < resSize; i++)
        {
            nameIndex.insert(std::make_pair(schList[i].GetName(), i));
        }
        for (auto it = nameIndex.begin();it != nameIndex.end();++it)
        {
            schListOrder.push_back(it->second);
        }
    }
    else if (orderBy == kOrderName[1] && searchIndex != 1)
    {
        mtl::multimap<time_t, int64_t> startTimeIndex;
        for (size_t i = 0; i < resSize; i++)
        {
            startTimeIndex.insert(std::make_pair(schList[i].GetStartTime(), i));
        }
        for (auto it = startTimeIndex.begin();it != startTimeIndex.end();++it)
        {
            schListOrder.push_back(it->second);
        }
    }
    else if (orderBy == kOrderName[3] && searchIndex != 3)
    {
        mtl::multimap<mtl::string, int64_t> tagsIndex;
        for (size_t i = 0; i < resSize; i++)
        {
            tagsIndex.insert(std::make_pair(schList[i].GetTags(), i));
        }
        for (auto it = tagsIndex.begin();it != tagsIndex.end();++it)
        {
            schListOrder.push_back(it->second);
        }
    }
    else
    {
        for (size_t i = 0; i < resSize; i++)
        {
            schListOrder.push_back(i);
        }
    }


    if (!asc)
    {
        // 逆序
        size_t size = schListOrder.size();
        for (size_t i = 0; i < size / 2; i++)
        {
            std::swap(schListOrder[i], schListOrder[size - i - 1]);
        }
    }

    auto list = CefListValue::Create();
    for (auto& index : schListOrder)
    {
        auto dict = schList[index].ToCefDict();
        list->SetDictionary(list->GetSize(), dict);
    }
    ApiCallback(cbi, makeRespoense(list));
}


void ImportClass()
{
    auto& rs = classTable.startTimeIndex;

    // BACK_DBG_LOG(std::to_string(classTable.weekIndex.size()));

    for (auto it = rs.begin();it != rs.end();++it)
    {
        auto classOne = database::Class(classTable.Find(it->second));
        auto classWeek = classOne.GetWeek();
        for (size_t i = 0; i < 64; i++)
        {
            if (classWeek & (1LL << i))
            {
                time_t weekStart = std::stoll(sysConfig["WeekOffset"]) + i * 7 * 24 * 60 * 60;
                auto course = database::Course(courseTable.Find(classOne.GetCourseId()));
                auto sch = database::Schedule(GenUid(), classOne.GetCourseId(),
                    classOne.GetStartTime() + weekStart, classOne.GetEndTime() + weekStart,
                    course.GetName(), classOne.GetLoc(), utils::to_byte_string(L"课程"), false);
                schTable.AddSch(sch);
                // sysLogger.Log("INFO", "Import class: " + sch.ToRecord().Dumps());
            }
        }
    }
}

void ImportExam()
{
    auto& rs = examTable.GetRecords();

    for (auto it = rs.begin();it != rs.end();++it)
    {
        auto exam = database::Exam(it->second);
        auto course = database::Course(courseTable.Find(exam.GetCourseId()));
        auto sch = database::Schedule(GenUid(), exam.GetCourseId(),
            exam.GetStartTime(), exam.GetEndTime(),
            course.GetName() + utils::to_byte_string(std::wstring(L" - 考试")), exam.GetLoc(), utils::to_byte_string(L"考试"), false);
        schTable.AddSch(sch);
        // sysLogger.Log("INFO", "Import exam: " + sch.ToRecord().Dumps());
    }
}

void ImportClassApi(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    ImportClass();
    ApiCallback(cbi, makeRespoense());
}

void RemoveClass()
{
    auto& rs = schTable.GetRecords();
    for (auto it = rs.begin();it != rs.end();++it)
    {
        auto sch = database::Schedule(it->second);
        if (sch.GetTags() == utils::to_byte_string(L"课程"))
        {
            schTable.DeleteSch(sch.GetUid());
        }
    }
}

void RemoveClassApi(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    RemoveClass();
    ApiCallback(cbi, makeRespoense());
}

void RemoveExam()
{
    auto& rs = schTable.GetRecords();
    for (auto it = rs.begin();it != rs.end();++it)
    {
        auto sch = database::Schedule(it->second);
        if (sch.GetTags() == utils::to_byte_string(L"考试"))
        {
            schTable.DeleteSch(sch.GetUid());
        }
    }
}

void RemoveExamApi(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    RemoveExam();
    ApiCallback(cbi, makeRespoense());
}

} // namespace sch