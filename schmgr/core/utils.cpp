#include <string>
#include <map>
#include <windows.h>
#include <mutex>

#include "schmgr/shared/common/client_app.h"

#include "schmgr/core/utils.h"

namespace core
{
std::map<std::string, std::string> mimeMap{
    {"html", "text/html"},
    { "js", "text/javascript" },
    { "css", "text/css" },
    { "jpg", "image/jpeg" },
    { "jpeg", "image/jpeg" },
    { "png", "image/png" },
    { "gif", "image/gif" },
    { "svg", "image/svg+xml" },
    { "ico", "image/x-icon" },
    { "json", "application/json" },
    { "pdf", "application/pdf" },
    { "zip", "application/zip" },
    { "xml", "application/xml" },
    { "txt", "text/plain" },
};

std::string GetExePath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

std::string GetMime(const std::string& file)
{
    std::string::size_type pos = file.find_last_of(".");
    if (pos == std::string::npos)
        return "text/plain";
    std::string ext = file.substr(pos + 1);
    if (mimeMap.find(ext) == mimeMap.end())
        return "text/plain";
    return mimeMap[ext];
}

CefV8ValueList CefListValueToCefV8ValueList(CefRefPtr<CefListValue> value)
{
    CefV8ValueList result;
    size_t size = value->GetSize();
    for (size_t i = 0; i < size; i++)
    {
        result.push_back(CefValueToCefV8Value(value->GetValue(i)));
    }
    return result;
}



CefRefPtr<CefV8Value> CefValueToCefV8Value(CefRefPtr<CefValue> value)
{
    CefRefPtr<CefV8Value> result;
    switch (value->GetType())
    {
    case VTYPE_INVALID:
    {
        // std::cout << "Type: VTYPE_INVALID" << std::endl;
        result = CefV8Value::CreateNull();
    }
    break;
    case VTYPE_NULL:
    {
        // std::cout << "Type: VTYPE_NULL" << std::endl;
        result = CefV8Value::CreateNull();
    }
    break;
    case VTYPE_BOOL:
    {
        // std::cout << "Type: VTYPE_BOOL" << std::endl;
        result = CefV8Value::CreateBool(value->GetBool());
    }
    break;
    case VTYPE_INT:
    {
        // std::cout << "Type: VTYPE_INT" << std::endl;
        result = CefV8Value::CreateInt(value->GetInt());
    }
    break;
    case VTYPE_DOUBLE:
    {
        // std::cout << "Type: VTYPE_DOUBLE" << std::endl;
        result = CefV8Value::CreateDouble(value->GetDouble());
    }
    break;
    case VTYPE_STRING:
    {
        // std::cout << "Type: VTYPE_STRING" << std::endl;
        result = CefV8Value::CreateString(value->GetString());
    }
    break;
    case VTYPE_BINARY:
    {
        // std::cout << "Type: VTYPE_BINARY" << std::endl;
        result = CefV8Value::CreateNull();
    }
    break;
    case VTYPE_DICTIONARY:
    {
        // std::cout << "Type: VTYPE_DICTIONARY" << std::endl;
        result = CefV8Value::CreateObject(nullptr, nullptr);
        CefRefPtr<CefDictionaryValue> dict = value->GetDictionary();
        CefDictionaryValue::KeyList keys;
        dict->GetKeys(keys);
        for (unsigned int i = 0; i < keys.size(); i++)
        {
            CefString key = keys[i];
            result->SetValue(key, CefValueToCefV8Value(dict->GetValue(key)), V8_PROPERTY_ATTRIBUTE_NONE);
        }
    }
    break;
    case VTYPE_LIST:
    {
        // std::cout << "Type: VTYPE_LIST" << std::endl;
        CefRefPtr<CefListValue> list = value->GetList();
        int size = int(list->GetSize());
        result = CefV8Value::CreateArray(size);
        for (int i = 0; i < size; i++)
        {
            result->SetValue(i, CefValueToCefV8Value(list->GetValue(i)));
        }
    }
    break;
    }
    return result;
}

CefRefPtr<CefValue> CefV8ValueToCefValue(const CefRefPtr<CefV8Value> value)
{
    CefRefPtr<CefValue> result = CefValue::Create();

    if (!value->IsValid())
    {
        result->SetNull();
        return result;
    }

    if (value->IsNull())
    {
        result->SetNull();
        return result;
    }

    if (value->IsBool())
    {
        result->SetBool(value->GetBoolValue());
        return result;
    }

    if (value->IsDouble() || value->IsUInt() || value->IsInt())
    {
        result->SetDouble(value->GetDoubleValue());
        return result;
    }

    // if (value->IsUInt())
    // {
    //     result->SetDouble(value->GetUIntValue());
    //     return result;
    // }

    // if (value->IsInt())
    // {
    //     result->SetInt(value->GetIntValue());
    //     return result;
    // }

    if (value->IsString())
    {
        result->SetString(value->GetStringValue());
        return result;
    }

    if (value->IsArray())
    {
        CefRefPtr<CefListValue> list = CefListValue::Create();
        int size = value->GetArrayLength();
        for (int i = 0; i < size; i++)
        {
            list->SetValue(i, CefV8ValueToCefValue(value->GetValue(i)));
        }
        result->SetList(list);
        return result;
    }

    if (value->IsObject())
    {
        CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
        CefDictionaryValue::KeyList keys;
        value->GetKeys(keys);
        for (CefDictionaryValue::KeyList::iterator it = keys.begin(); it != keys.end(); ++it)
        {
            dict->SetValue(it->ToString(), CefV8ValueToCefValue(value->GetValue(it->ToString())));
        }
        result->SetDictionary(dict);
        return result;
    }

    if (value->IsArray())
    {
        CefRefPtr<CefListValue> list = CefListValue::Create();
        int size = value->GetArrayLength();
        for (int i = 0; i < size; i++)
        {
            list->SetValue(i, CefV8ValueToCefValue(value->GetValue(i)));
        }
        result->SetList(list);
        return result;
    }

    result->SetNull();

    return result;
}

std::string StringTime(time_t time)
{
    struct tm* timeinfo;
    char buffer[80];
    timeinfo = localtime(&time);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

int64_t CefValToI64(CefRefPtr<CefDictionaryValue> dict, const std::string& key, int64_t defaultValue)
{
    if (!dict->HasKey(key))
    {
        return defaultValue;
    }
    CefRefPtr<CefValue> value = dict->GetValue(key);
    if (value->GetType() == VTYPE_INT)
    {
        return value->GetInt();
    }
    if (value->GetType() == VTYPE_DOUBLE)
    {
        return (int64_t)value->GetDouble();
    }
    return defaultValue;
}

int64_t CefValToI64(CefRefPtr<CefValue> v, int64_t defaultValue)
{
    if (v->GetType() == VTYPE_INT)
    {
        return v->GetInt();
    }
    if (v->GetType() == VTYPE_DOUBLE)
    {
        return (int64_t)v->GetDouble();
    }
    return defaultValue;
}

int64_t CefValToI64(CefRefPtr<CefListValue> list, size_t index, int64_t defaultValue)
{
    if (index >= list->GetSize())
    {
        return defaultValue;
    }
    CefRefPtr<CefValue> value = list->GetValue(index);
    if (value->GetType() == VTYPE_INT)
    {
        return value->GetInt();
    }
    if (value->GetType() == VTYPE_DOUBLE)
    {
        return (int64_t)value->GetDouble();
    }
    return defaultValue;
}

int64_t GenUid()
{
    static std::mutex uidMtx;
    static int64_t uidLast = 0;

    uidMtx.lock();
    cef_time_t cef_now;
    cef_time_now(&cef_now);
    double now;
    cef_time_to_doublet(&cef_now, &now);
    auto uid = now * 1000;
    if (uidLast >= uid)
    {
        uid = uidLast + 1;
    }
    uidLast = uid;
    uidMtx.unlock();
    return uid;
}

}