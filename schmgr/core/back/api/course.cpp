#include "schmgr/core/back/api/course.h"


namespace core::back::api
{

namespace course
{

void GetList(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    CefRefPtr<CefListValue> list = CefListValue::Create();

    for (auto& it : courseTable.GetRecords())
    {
        list->SetDictionary(list->GetSize(), database::Course(it.second).ToCefDict());
    }

    ApiCallback(cbi, makeRespoense(list));
}

void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    auto dict = funArgs->GetDictionary(0);
    dict->SetDouble("id", GenUid());

    database::Course course(dict);

    if (courseTable.AddCourse(course) != -1)
    {
        ApiCallback(cbi, makeRespoense(dict));
    }
    else
    {
        ApiCallback(cbi, makeErrorRespoense(L"添加失败"));
    }
}

void Delete(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    int64_t uid = funArgs->GetDouble(0);

    if (courseTable.DeleteCourse(uid))
    {
        ApiCallback(cbi, makeRespoense());
    }
    else
    {
        ApiCallback(cbi, makeErrorRespoense(L"删除失败"));
    }
}

void Update(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    auto dict = funArgs->GetDictionary(0);
    database::Course course(dict);

    if (courseTable.UpdateCourse(course))
    {
        ApiCallback(cbi, makeRespoense(dict));
    }
    else
    {
        ApiCallback(cbi, makeErrorRespoense(L"更新失败"));
    }
}

} // namespace course

namespace courseClass
{

void GetList(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    CefRefPtr<CefListValue> list = CefListValue::Create();

    // for (auto it: classTable.GetRecords())
    // {
    //     sysLogger.Log("INFO", "class: " + it.second.Dumps());
    // }

    auto pair = classTable.courseIndex.equal_range(CefValToI64(funArgs, 0));
    auto& rs = classTable.GetRecords();

    for (auto& it = pair.first;it != pair.second;++it)
    {
        list->SetDictionary(list->GetSize(), database::Class(rs.find(it->second)->second).ToCefDict());
    }

    ApiCallback(cbi, makeRespoense(list));
}

void Get(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    auto id = CefValToI64(funArgs, 0);
    auto it = classTable.GetRecords().find(id);
    if (it == classTable.GetRecords().end())
    {
        ApiCallback(cbi, makeErrorRespoense(L"ID 不存在"));
        return;
    }

    auto dict = database::Class(it->second).ToCefDict();
    ApiCallback(cbi, makeRespoense(dict));
}

void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    auto dict = funArgs->GetDictionary(0);
    dict->SetDouble(database::Class::ColNames[database::Class::Col::UidCol], GenUid());

    database::Class course(dict);

    if (classTable.AddClass(course) != -1)
    {
        ApiCallback(cbi, makeRespoense(dict));
    }
    else
    {
        ApiCallback(cbi, makeErrorRespoense(L"添加失败"));
    }
}

void Delete(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    int64_t uid = funArgs->GetDouble(0);
    // sysLogger.Log("INFO", "delete class: " + classTable.GetRecords().find(uid)->second.Dumps());
    if (classTable.DeleteClass(uid))
    {
        ApiCallback(cbi, makeRespoense());
    }
    else
    {
        ApiCallback(cbi, makeErrorRespoense(L"删除失败"));
    }
}

void Update(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    auto dict = funArgs->GetDictionary(0);
    database::Class course(dict);

    if (classTable.UpdateClass(course))
    {
        ApiCallback(cbi, makeRespoense(dict));
    }
    else
    {
        ApiCallback(cbi, makeErrorRespoense(L"更新失败"));
    }
}

} // namespace courseClass

namespace exam
{

void GetList(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    CefRefPtr<CefListValue> list = CefListValue::Create();

    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    list = CefListValue::Create();

    auto pair = examTable.courseIndex.equal_range(CefValToI64(funArgs, 0));
    auto& rs = examTable.GetRecords();

    for (auto& it = pair.first;it != pair.second;++it)
    {
        list->SetDictionary(list->GetSize(), database::Exam(rs.find(it->second)->second).ToCefDict());
    }

    ApiCallback(cbi, makeRespoense(list));
}

void Get(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    auto id = CefValToI64(funArgs, 0);
    auto it = examTable.GetRecords().find(id);
    if (it == examTable.GetRecords().end())
    {
        ApiCallback(cbi, makeErrorRespoense(L"ID 不存在"));
        return;
    }

    auto dict = database::Exam(it->second).ToCefDict();
    ApiCallback(cbi, makeRespoense(dict));
}

void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    auto dict = funArgs->GetDictionary(0);
    dict->SetDouble(database::Exam::ColNames[database::Exam::Col::UidCol], GenUid());

    database::Exam course(dict);

    if (examTable.AddExam(course))
    {
        ApiCallback(cbi, makeRespoense(dict));
    }
    else
    {
        ApiCallback(cbi, makeErrorRespoense(L"添加失败"));
    }
}

void Delete(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    int64_t uid = funArgs->GetDouble(0);

    if (examTable.DeleteExam(uid))
    {
        ApiCallback(cbi, makeRespoense());
    }
    else
    {
        ApiCallback(cbi, makeErrorRespoense(L"删除失败"));
    }
}

void Update(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    auto dict = funArgs->GetDictionary(0);
    database::Exam course(dict);

    if (examTable.UpdateExam(course))
    {
        ApiCallback(cbi, makeRespoense(dict));
    }
    else
    {
        ApiCallback(cbi, makeErrorRespoense(L"更新失败"));
    }
}

} // namespace exam


} // namespace core::back::api
