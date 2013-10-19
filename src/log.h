#ifndef CH_INCLUDE_GUARD_D6A483C5_6BAA_4EB9_B6AF_D81EA42921D1
#define CH_INCLUDE_GUARD_D6A483C5_6BAA_4EB9_B6AF_D81EA42921D1


#define CH_LOG_LEVEL_ERROR    1
#define CH_LOG_LEVEL_INFO     2
#define CH_LOG_LEVEL_DEBUG    3


void ch_log_write(unsigned int level, const char *fmt, ...);

#define CH_LOG_ERROR(msg...) ch_log_write(CH_LOG_LEVEL_ERROR, msg)
#define CH_LOG_INFO(msg...) ch_log_write(CH_LOG_LEVEL_INFO, msg)
#define CH_LOG_DEBUG(msg...) ch_log_write(CH_LOG_LEVEL_DEBUG, msg)


#endif /* end of include guard */
