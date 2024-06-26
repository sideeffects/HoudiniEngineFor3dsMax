#pragma once

#pragma warning(push, 0)
#include "units.h"
#pragma warning(pop)

#include "HEMAX_Types.h"

namespace HEMAX_Time
{
    void
    PushTimelineSettings();

    void
    PushCurrentTime(TimeValue Time);

    void
    PushCurrentFrame(int Frame);

    static void
    GetCurrentHAPITime();

    bool
    HasHAPITimeChanged();

    void
    MarkSynchronized();
}
