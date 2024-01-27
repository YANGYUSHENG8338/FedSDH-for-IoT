/* Copyright 2020 The MathWorks, Inc. */

#ifndef logging2_core_rtw_logging_h
#define logging2_core_rtw_logging_h


typedef struct SscRTWLoggingTag SscRTWLogging;

/*
 * fwd declaration for the struct that is intended to contain a pointer to the
 * client side RTWLogging object. The client side RTWLogging class provides the
 * function for the C struct to interact with.
 */
typedef struct SscRTWLoggingObjectTag SscRTWLoggingObject;

/*!
  SscRTWLogging is a C interface to interact with the logging infrastructure
  from the generated code. SscRTWLogging is created through
  ssc_rtw_make_logging_struct function.

  Refer to RTWLogging.hpp
 */

struct SscRTWLoggingTag
{
    /* Setup the logger from the block */
    void (*mStart)(SscRTWLogging const*);

    /* Record the data for the specified time-step in the logger using the
     * specified buffer   */
    void (*mLog)(SscRTWLogging const*,
                 double        time,
                 double const* buffer,
                 unsigned int  num);
    
    /* Indicate that the logging is completed. */
    void (*mTerminate)(SscRTWLogging const*);

    /* Destructor - called from the custom deleter. Clients are not required to
     * call this function */
    void (*mDestroy)(SscRTWLogging*);

    /* Private data pointer to RTWLogging */
    SscRTWLoggingObject* mObject;
};

/* Create SscRTWLogging using RTWLogging instance. */
extern
SscRTWLogging* ssc_rtw_make_logging_struct(const char* modelName,
                                           const char* blockPath);

/* Wrapper function for mStart */
extern
void ssc_logger_start(SscRTWLogging const*);

/* Wrapper function for mTerminate */
extern
void ssc_logger_terminate(SscRTWLogging const*);

/* Wrapper function for mDestroy */
extern
void ssc_logger_destroy(SscRTWLogging*);

#endif
