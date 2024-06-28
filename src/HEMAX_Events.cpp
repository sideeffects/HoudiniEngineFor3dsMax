#include "HEMAX_Events.h"

#include <algorithm>

HEMAX_EventHandler::HEMAX_EventHandler(HEMAX_Events& Registrar)
{
    EventRegistrar = &Registrar;
    RegistrationId = nullptr;
}

HEMAX_EventHandler::~HEMAX_EventHandler()
{
    if (RegistrationId)
       EventRegistrar->UnregisterCallbacks(RegistrationId); 
}

void
HEMAX_EventHandler::RegisterCallback(HEMAX_EventType Type,
        const std::function<void(HEMAX_EventData*)>& Callback)
{
    RegistrationId = EventRegistrar->RegisterCallback(Type, Callback);
}

void
HEMAX_Events::EmitEvent(HEMAX_EventType Type, HEMAX_EventData* Data)
{
    for (auto&& EventCallback : EventCallbacks)
    {
        if (EventCallback.Type == Type)
            EventCallback.Callback(Data);
    }
}

const void*
HEMAX_Events::RegisterCallback(HEMAX_EventType Type,
    const std::function<void(HEMAX_EventData*)>& Callback)
{
    HEMAX_EventCallback& EventCallback = EventCallbacks.emplace_back(
        Type, Callback);
    return EventCallback.Id;
}

void
HEMAX_Events::UnregisterCallbacks(const void* HandlerId)
{
    EventCallbacks.erase(
        std::remove_if(EventCallbacks.begin(), EventCallbacks.end(),
            [HandlerId](HEMAX_EventCallback Callback) {
                return Callback.Id == HandlerId;
        }),
        EventCallbacks.end());
}
