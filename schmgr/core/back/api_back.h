#ifndef SCHMGR_CORE_API_BACK_H_
#define SCHMGR_CORE_API_BACK_H_


#include "include/base/cef_callback.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/cef_parser.h"
#include "schmgr/shared/common/client_app.h"

#include "schmgr/utils/string.h"
#include "schmgr/core/core.h"
#include "schmgr/core/utils.h"
#include "schmgr/core/back/database/db.h"
#include "schmgr/core/back/database/table.h"
#include "schmgr/core/back/database/model.h"
#include "schmgr/core/back/logger.h"
#include "schmgr/core/back/timer/clock.h"
#include "schmgr/core/back/nav/nav.h"
#include "schmgr/core/api.h"


namespace core::back
{
extern core::timer::Clock sysClock;
extern database::UserTable userTable;
extern database::SchTable schTable;
extern database::CourseTable courseTable; // 课程信息表
extern database::ClassTable classTable;
extern database::ExamTable examTable;
extern database::AlarmTable alarmTable;
extern mtl::map<int64_t, database::Alarm> alarms;
extern mtl::map<int64_t, database::Alarm> preAlarms;
extern logger::Logger sysLogger;
extern mtl::map<mtl::string, mtl::string> sysConfig;
extern nav::Graph navGraph;

class CallbackInfo
{
private:
    CefRefPtr<CefFrame> frame;
    mtl::string callbackId;
    mtl::string callName;
public:

    CallbackInfo(CefRefPtr<CefFrame> frame, mtl::string callbackId, mtl::string callName)
        : frame(frame), callbackId(callbackId), callName(callName)
    {
    }

    CefRefPtr<CefFrame> GetFrame() const
    {
        return frame;
    }

    mtl::string GetCallbackId() const
    {
        return callbackId;
    }

    mtl::string GetCallName() const
    {
        return callName;
    }

};


void OnScheduleManagerBackMessageReceived(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefProcessMessage> message);
void ApiCallback(const CallbackInfo& cbi, CefRefPtr<CefValue> value);
void ApiCall(const mtl::string& name, CefRefPtr<CefListValue> funArgs);
void ApiErrorCallback(const CallbackInfo& cbi, const mtl::string& errorMsg);
bool log(mtl::string type = "INFO", mtl::string msg = "");
bool initTable();
bool sysInit();

namespace api
{
extern mtl::unordered_map<std::string,
    std::function<void(CallbackInfo,
        CefRefPtr<CefListValue>)>> jsAsyncFunsMap;

// void myaddAsync(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);

// 返回错误信息
CefRefPtr<CefValue> makeErrorRespoense(const mtl::string& errorMsg);

// 返回错误信息 utf8
CefRefPtr<CefValue> makeErrorRespoense(const std::wstring& errorMsg);

// 返回数据
CefRefPtr<CefValue> makeRespoense(CefRefPtr<CefValue> data);

CefRefPtr<CefValue> makeRespoense(CefRefPtr<CefListValue> list);

CefRefPtr<CefValue> makeRespoense(CefRefPtr<CefDictionaryValue> dict);

CefRefPtr<CefValue> makeRespoense(const mtl::string& text);

CefRefPtr<CefValue> makeRespoense(double d);

CefRefPtr<CefValue> makeRespoense(bool b);


// 返回数据NULL
CefRefPtr<CefValue> makeRespoense();

void init(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);

namespace db
{
void dumpTable(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
} // namespace db

} // namespace api

} // namespace core

#endif // SCHMGR_CORE_API_BACK_H_