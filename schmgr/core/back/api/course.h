#ifndef SCHMGR_CORE_BACK_API_COURSE_H_
#define SCHMGR_CORE_BACK_API_COURSE_H_

#include "schmgr/core/back/api_back.h"



namespace core::back::api
{

namespace course
{
    
void GetList(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Delete(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Update(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);

} // namespace course

namespace courseClass
{
    
void GetList(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Get(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Delete(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Update(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);

} // namespace courseClass

namespace exam
{
    
void GetList(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Get(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Delete(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Update(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);

} // namespace exam

}

#endif // SCHMGR_CORE_BACK_API_COURSE_H_