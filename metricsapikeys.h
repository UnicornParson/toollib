#ifndef METRICSAPIKEYS_H
#define METRICSAPIKEYS_H

#include <QString>
#include "toolsmacro.h"

namespace MetricsAPIKey
{
  // mgenn core
  CONST_LITERAL MGENN_METRIC_TIMER_EVENTS_SIZE("TimerEventsSize");
  CONST_LITERAL MGENN_METRIC_TIMER_EVENTS_CASHE_SIZE("TimerEventsCasheSize");
  CONST_LITERAL MGENN_METRIC_SINGLE_DB_WRITE_DURATION("SingleSqlWriteDuration");
  CONST_LITERAL MGENN_METRIC_BATCH_DB_WRITE_DURATION("BatchSqlWriteDuration");
  CONST_LITERAL MGENN_METRIC_DB_WRITE_QUEUE_SIZE("DBWriteQueueSize");
  CONST_LITERAL MGENN_METRIC_CORE_SAVE_DURATION("CoreSaveDuration");
  CONST_LITERAL MGENN_METRIC_CORE_TICK_DURATION("CoreTickDuration");

  // file downloader
  CONST_LITERAL FD_METRIC_API_FETCH_DELAY("FDFetchDelay");
  CONST_LITERAL FD_METRIC_API_SYNC_WAIT("FDSyncWait");

  // mgenn api client
  CONST_LITERAL MGENN_METRIC_API_FETCH_DELAY("ApiFetchDelay");

  // mgenn IO
  CONST_LITERAL MGENN_METRIC_IO_API_FETCH_DELAY("IOApiFetchDelay");

};
#endif // METRICSAPIKEYS_H
