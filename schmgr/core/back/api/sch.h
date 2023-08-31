#ifndef SCHMGR_CORE_BACK_API_SCH_H_
#define SCHMGR_CORE_BACK_API_SCH_H_

#include "schmgr/core/back/api_back.h"



namespace core::back::api::sch
{

// enum class DefaultId {
//     Temp = -3,
//     Task = -2,
//     None = 0
// };

bool isSomeActivity(const database::Schedule& s,const mtl::list<std::wstring>& activityList);


void GetCurrentWeek(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void GetByWeek(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Update(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Delete(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void GetById(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void GetByFilter(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);

void ImportClass();
void ImportExam();
void ImportClassApi(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void RemoveClass();
void RemoveClassApi(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void RemoveExam();
void RemoveExamApi(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);

} // namespace sch

#endif // SCHMGR_CORE_BACK_API_SCH_H_