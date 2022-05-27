#ifndef THREADS_H
#define THREADS_H

void *thread_reader(void *);
void *thread_analyzer(void *);
void *thread_printer(void *);
void *thread_watchdog(void *);
void *thread_logger(void *);

#endif
