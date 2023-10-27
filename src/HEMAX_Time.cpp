#include "HEMAX_Time.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_SessionManager.h"

HAPI_TimelineOptions HEMAX_GlobalTimeOptions;
float CurrentHAPITime;
bool HasTimeChanged;

void
PushTimelineSettings()
{
    HEMAX_GlobalTimeOptions.fps = (float)GetFrameRate();

    Interval TickInterval = GetCOREInterface()->GetAnimRange();

    TimeValue Start = TickInterval.Start();
    TimeValue End = TickInterval.End();

    int StartFrame = Start / GetTicksPerFrame();
    int EndFrame = End / GetTicksPerFrame();

    float StartSeconds = (float)StartFrame / (float)GetFrameRate();
    float EndSeconds = (float)EndFrame / (float)GetFrameRate();

    HEMAX_GlobalTimeOptions.startTime = StartSeconds;
    HEMAX_GlobalTimeOptions.endTime = EndSeconds;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::SetTimelineOptions(SM.Session,
        &HEMAX_GlobalTimeOptions);

    HEMAX_HoudiniApi::GetTime(SM.Session, &CurrentHAPITime);
}

void
PushCurrentTime(TimeValue Time)
{
    int Frame = Time / GetTicksPerFrame();
    int FPS = GetFrameRate();
    float Seconds = (float)Frame / (float)FPS;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::SetTime(SM.Session, Seconds);

    GetCurrentHAPITime();
}

void
PushCurrentFrame(int Frame)
{
    int FPS = GetFrameRate();
    float Seconds = (float)Frame/(float)FPS;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::SetTime(SM.Session, Seconds);

    GetCurrentHAPITime();
}

static void
GetCurrentHAPITime()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    float OldTime = CurrentHAPITime;

    HEMAX_HoudiniApi::GetTime(SM.Session, &CurrentHAPITime);

    if (OldTime != CurrentHAPITime)
    {
	HasTimeChanged = true;
    }
}

bool
HasHAPITimeChanged()
{
    return HasTimeChanged;
}

void
MarkSynchronized()
{
    HasTimeChanged = false;
}
