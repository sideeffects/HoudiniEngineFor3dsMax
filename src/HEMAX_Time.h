#pragma once

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
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
