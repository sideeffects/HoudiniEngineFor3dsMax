#include "HEMAX_Time.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_SessionManager.h"

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <interval.h>
#include <maxapi.h>
#pragma warning(pop)

namespace HEMAX_Time
{

HAPI_TimelineOptions HEMAX_GlobalTimeOptions;
double CurrentHAPITime;
bool HasTimeChanged;

void
PushTimelineSettings()
{
    HEMAX_GlobalTimeOptions.fps = (double)GetFrameRate();

    Interval TickInterval = GetCOREInterface()->GetAnimRange();

    TimeValue Start = TickInterval.Start();
    TimeValue End = TickInterval.End();

    int StartFrame = Start / GetTicksPerFrame();
    int EndFrame = End / GetTicksPerFrame();

    double StartSeconds = (double)StartFrame / (double)GetFrameRate();
    double EndSeconds = (double)EndFrame / (double)GetFrameRate();

    HEMAX_GlobalTimeOptions.startTime = StartSeconds;
    HEMAX_GlobalTimeOptions.endTime = EndSeconds;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::SetTimelineOptions(&SM.Session,
        &HEMAX_GlobalTimeOptions);

    HEMAX_HoudiniApi::GetTime(&SM.Session, &CurrentHAPITime);
}

void
PushCurrentTime(TimeValue Time)
{
    int Frame = Time / GetTicksPerFrame();
    int FPS = GetFrameRate();
    double Seconds = (double)Frame / (double)FPS;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::SetTime(&SM.Session, Seconds);

    GetCurrentHAPITime();
}

void
PushCurrentFrame(int Frame)
{
    int FPS = GetFrameRate();
    double Seconds = (double)Frame/(double)FPS;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::SetTime(&SM.Session, Seconds);

    GetCurrentHAPITime();
}

static void
GetCurrentHAPITime()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    float OldTime = CurrentHAPITime;

    HEMAX_HoudiniApi::GetTime(&SM.Session, &CurrentHAPITime);

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

}
