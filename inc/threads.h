#ifndef THREADS_H
#define THREADS_H

void *thread_reader(void *arg);
void *thread_analyzer(void *arg);
void *thread_printer(void *arg);
void *thread_watchdog(void *arg);
void *thread_logger(void *arg);

#endif
