#ifndef OM_LOG_H
#define OM_LOG_H

#define OPENMONERO_LOG_CATEGORY "openmonero"

#define OMINFO  CLOG(INFO, OPENMONERO_LOG_CATEGORY)
#define OMWARN  CLOG(WARNING, OPENMONERO_LOG_CATEGORY)
#define OMERROR CLOG(ERROR, OPENMONERO_LOG_CATEGORY)

#define OMINFO_IF(cond)  CLOG_IF(cond, INFO, OPENMONERO_LOG_CATEGORY)
#define OMWARN_IF(cond)  CLOG_IF(cond, WARNING, OPENMONERO_LOG_CATEGORY)
#define OMERROR_IF(cond) CLOG_IF(cond, ERROR, OPENMONERO_LOG_CATEGORY)


#endif // OM_LOG_H
