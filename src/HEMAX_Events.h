#pragma once

#include <functional>
#include <vector>

class HEMAX_3dsmaxHda;

enum class HEMAX_EventType
{
    SessionChanged,
    SessionReady,
    SessionStopped,
    ShelfUpdated,
    AssetLoaded,
    AssetRemoved,
    InputChanged,
    ParameterChanged,
    NodeSettingsChanged,
    HdaPreDeleteNotification,
    SelectionSetChanged
};

struct HEMAX_EventData
{
    virtual ~HEMAX_EventData() = default;
};

struct HEMAX_EventData_HdaPreDeleteNotification : public HEMAX_EventData
{
    HEMAX_3dsmaxHda* Hda = nullptr;
};

struct HEMAX_EventData_SelectionSetChanged : public HEMAX_EventData
{
    HEMAX_3dsmaxHda* Hda = nullptr;
    bool ForceUnlock = false;
};

class HEMAX_Events;

class HEMAX_EventHandler
{

public:

    HEMAX_EventHandler(HEMAX_Events& Registrar);
    ~HEMAX_EventHandler();

    void                RegisterCallback(HEMAX_EventType Type,
                            const std::function<void(HEMAX_EventData*)>& Callback);

private:

    HEMAX_Events*           EventRegistrar;
    const void*             RegistrationId;
};

class HEMAX_Events
{

private:

    struct HEMAX_EventCallback
    {
        HEMAX_EventCallback(HEMAX_EventType TheType,
                            const std::function<void(HEMAX_EventData*)> TheCallback)
            : Type(TheType)
            , Callback(TheCallback)
        {
            Id = this;
        }

        ~HEMAX_EventCallback() = default;

        const void*                             Id;
        HEMAX_EventType                         Type;
        std::function<void(HEMAX_EventData*)>   Callback;
    };

public:

                        HEMAX_Events() = default;
                        ~HEMAX_Events() = default;

    void                EmitEvent(HEMAX_EventType Type, HEMAX_EventData* data);

private:

    const void*         RegisterCallback(HEMAX_EventType Type,
                            const std::function<void(HEMAX_EventData*)>& Callback);
    void                UnregisterCallbacks(const void* HandlerId);

    std::vector<HEMAX_EventCallback> EventCallbacks;

    friend              HEMAX_EventHandler::~HEMAX_EventHandler();
    friend void         HEMAX_EventHandler::RegisterCallback(
                            HEMAX_EventType Type,
                            const std::function<void(HEMAX_EventData*)>& Callback);

};
