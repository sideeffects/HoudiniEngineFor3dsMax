#pragma once

#include "units.h"
#include "HEMAX_Types.h"

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
