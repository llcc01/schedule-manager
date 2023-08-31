#include "schmgr/core/back/api/nav.h"

namespace core::back::api::nav
{
void GetDistance(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 2)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));
        return;
    }

    int v1 = funArgs->GetDouble(0);
    int v2 = funArgs->GetDouble(1);
    int d = navGraph.getDistance(v1, v2);
    auto data = CefValue::Create();
    data->SetDouble(d);
    ApiCallback(cbi, makeRespoense(data));
}

void GetShortestDistance(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 2)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));
        return;
    }

    int v1 = funArgs->GetDouble(0);
    int v2 = funArgs->GetDouble(1);
    int d = navGraph.getShortestDistance(v1, v2);
    auto data = CefValue::Create();
    data->SetDouble(d);
    ApiCallback(cbi, makeRespoense(data));
}

void GetShortestPath(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 2)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));
        return;
    }

    int start = funArgs->GetDouble(0);
    int end = funArgs->GetDouble(1);
    auto path = navGraph.getShortestPath(start, end);
    auto data = CefListValue::Create();
    for (auto& it : path)
    {
        data->SetDouble(data->GetSize(), it);
    }
    ApiCallback(cbi, makeRespoense(data));
}

void GetShortestTrip(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 2)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));
        return;
    }

    int start = funArgs->GetDouble(0);
    auto destinationList = funArgs->GetList(1);
    mtl::vector<int> destinations;
    for (int i = 0; i < destinationList->GetSize(); ++i)
    {
        destinations.push_back(destinationList->GetDouble(i));
    }
    auto trip = navGraph.getShortestTrip(start, destinations);
    auto data = CefListValue::Create();
    for (auto& it : trip)
    {
        data->SetDouble(data->GetSize(), it);
    }
    ApiCallback(cbi, makeRespoense(data));
}
} // namespace nav