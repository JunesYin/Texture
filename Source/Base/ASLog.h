//
//  ASLog.h
//  Texture
//
//  Copyright (c) Facebook, Inc. and its affiliates.  All rights reserved.
//  Changes after 4/13/2017 are: Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import <AsyncDisplayKit/ASAvailability.h>
#import <AsyncDisplayKit/ASBaseDefines.h>
#import <Foundation/Foundation.h>
#import <os/activity.h>
#import <os/log.h>

#ifndef ASEnableVerboseLogging
#define ASEnableVerboseLogging 0
#endif

/**
 * Disable all logging.
 *
 * You should only use this function if the default log level is
 * annoying during development. By default, logging is run at
 * the appropriate system log level (see the os_log_* functions),
 * so you do not need to worry generally about the performance
 * implications of log messages.
 *
 * For example, virtually all log messages generated by Texture
 * are at the `debug` log level, which the system
 * disables in production.
 */
AS_EXTERN void ASDisableLogging(void);

/**
 * Restore logging that has been runtime-disabled via ASDisableLogging().
 *
 * Logging can be disabled at runtime using the ASDisableLogging() function.
 * This command restores logging to the level provided in the build
 * configuration. This can be used in conjunction with ASDisableLogging()
 * to allow logging to be toggled off and back on at runtime.
 */
AS_EXTERN void ASEnableLogging(void);

/// Log for general node events e.g. interfaceState, didLoad.
#define ASNodeLogEnabled 1
AS_EXTERN os_log_t ASNodeLog(void);

/// Log for layout-specific events e.g. calculateLayout.
#define ASLayoutLogEnabled 1
AS_EXTERN os_log_t ASLayoutLog(void);

/// Log for display-specific events e.g. display queue batches.
#define ASDisplayLogEnabled 1
AS_EXTERN os_log_t ASDisplayLog(void);

/// Log for collection events e.g. reloadData, performBatchUpdates.
#define ASCollectionLogEnabled 1
AS_EXTERN os_log_t ASCollectionLog(void);

/// Log for ASNetworkImageNode and ASMultiplexImageNode events.
#define ASImageLoadingLogEnabled 1
AS_EXTERN os_log_t ASImageLoadingLog(void);

/// Specialized log for our main thread deallocation trampoline.
#define ASMainThreadDeallocationLogEnabled 0
AS_EXTERN os_log_t ASMainThreadDeallocationLog(void);

/**
 * The activity tracing system changed a lot between iOS 9 and 10.
 * In iOS 10, the system was merged with logging and became much more powerful
 * and adopted a new API.
 *
 * The legacy API is visible, but its functionality is extremely limited and the API is so different
 * that we don't bother with it. For example, activities described by os_activity_start/end are not
 * reflected in the log whereas activities described by the newer
 * os_activity_scope are. So unfortunately we must use these iOS 10
 * APIs to get meaningful logging data.
 */
#if OS_LOG_TARGET_HAS_10_12_FEATURES

#define OS_ACTIVITY_NULLABLE nullable
#define AS_ACTIVITY_CURRENT OS_ACTIVITY_CURRENT
#define as_activity_scope(activity) os_activity_scope(activity)
#define as_activity_apply(activity, block) os_activity_apply(activity, block)
#define as_activity_create(description, parent_activity, flags) os_activity_create(description, parent_activity, flags)
#define as_activity_scope_enter(activity, statePtr) os_activity_scope_enter(activity, statePtr)
#define as_activity_scope_leave(statePtr) os_activity_scope_leave(statePtr)
#define as_activity_get_identifier(activity, outParentID) os_activity_get_identifier(activity, outParentID)

#else

#define OS_ACTIVITY_NULLABLE
#define AS_ACTIVITY_CURRENT OS_ACTIVITY_NULL
#define as_activity_scope(activity)
#define as_activity_apply(activity, block)
#define as_activity_create(description, parent_activity, flags) OS_ACTIVITY_NULL
#define as_activity_scope_enter(activity, statePtr)
#define as_activity_scope_leave(statePtr)
#define as_activity_get_identifier(activity, outParentID) (os_activity_id_t)0

#endif  // OS_LOG_TARGET_HAS_10_12_FEATURES

// Create activities only when verbose enabled. Doesn't materially impact performance, but good if we're cluttering up
// activity scopes and reducing readability.
#if ASEnableVerboseLogging
#define as_activity_scope_verbose(activity) as_activity_scope(activity)
#else
#define as_activity_scope_verbose(activity)
#endif

// Convenience for: as_activity_scope(as_activity_create(description, AS_ACTIVITY_CURRENT, OS_ACTIVITY_FLAG_DEFAULT))
#define as_activity_create_for_scope(description) \
  as_activity_scope(as_activity_create(description, AS_ACTIVITY_CURRENT, OS_ACTIVITY_FLAG_DEFAULT))

/**
 * The logging macros are not guarded by deployment-target checks like the activity macros are, but they are
 * only available on iOS >= 9 at runtime, so just make them conditional.
 */

#define as_log_create(subsystem, category)        \
  ({                                              \
    os_log_t __val;                               \
    if (AS_AVAILABLE_IOS_TVOS(9, 9)) {            \
      __val = os_log_create(subsystem, category); \
    } else {                                      \
      __val = (os_log_t)0;                        \
    }                                             \
    __val;                                        \
  })

#define as_log_debug(log, format, ...)        \
  if (AS_AVAILABLE_IOS_TVOS(9, 9)) {          \
    os_log_debug(log, format, ##__VA_ARGS__); \
  } else {                                    \
    (void)0;                                  \
  }

#define as_log_info(log, format, ...)        \
  if (AS_AVAILABLE_IOS_TVOS(9, 9)) {         \
    os_log_info(log, format, ##__VA_ARGS__); \
  } else {                                   \
    (void)0;                                 \
  }

#define as_log_error(log, format, ...)        \
  if (AS_AVAILABLE_IOS_TVOS(9, 9)) {          \
    os_log_error(log, format, ##__VA_ARGS__); \
  } else {                                    \
    (void)0;                                  \
  }

#define as_log_fault(log, format, ...)        \
  if (AS_AVAILABLE_IOS_TVOS(9, 9)) {          \
    os_log_fault(log, format, ##__VA_ARGS__); \
  } else {                                    \
    (void)0;                                  \
  }

#if ASEnableVerboseLogging
#define as_log_verbose(log, format, ...) as_log_debug(log, format, ##__VA_ARGS__)
#else
#define as_log_verbose(log, format, ...)
#endif
