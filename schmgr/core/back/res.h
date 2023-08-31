#ifndef SCHMGR_CORE_RES_H_
#define SCHMGR_CORE_RES_H_

#include "schmgr/shared/common/client_app.h"


namespace core::back
{
    bool ResourceHandler(scoped_refptr<CefResourceManager::Request> request);
}

#endif // SCHMGR_CORE_RES_H_