#pragma once

#include "units.h"
#include "HEMAX_Types.h"

void
PushTimelineSettings();

void
PushCurrentTime(TimeValue Time);

static void
GetCurrentHAPITime();

bool
HasHAPITimeChanged();

void
MarkSynchronized();