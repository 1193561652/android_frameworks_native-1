/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <utils/Timers.h>
#include "VSyncDispatch.h"

namespace android::scheduler {
/*
 * VSyncTracker is an interface for providing estimates on future Vsync signal times based on
 * historical vsync timing data.
 */
class VSyncTracker {
public:
    virtual ~VSyncTracker();

    /*
     * Adds a known timestamp from a vsync timing source (HWVsync signal, present fence)
     * to the model.
     *
     * \param [in] timestamp    The timestamp when the vsync signal was.
     */
    virtual void addVsyncTimestamp(nsecs_t timestamp) = 0;

    /*
     * Access the next anticipated vsync time such that the anticipated time >= timePoint.
     * This will always give the best accurate at the time of calling; multiple
     * calls with the same timePoint might give differing values if the internal model
     * is updated.
     *
     * \param [in] timePoint    The point in time after which to estimate a vsync event.
     * \return                  A prediction of the timestamp of a vsync event.
     */
    virtual nsecs_t nextAnticipatedVSyncTimeFrom(nsecs_t timePoint) const = 0;

protected:
    VSyncTracker(VSyncTracker const&) = delete;
    VSyncTracker& operator=(VSyncTracker const&) = delete;
    VSyncTracker() = default;
};

} // namespace android::scheduler