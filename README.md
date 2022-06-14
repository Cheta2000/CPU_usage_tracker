# CPU_usage_tracker

Multithreaded console application to track usage of each CPU's core.
There are 5 threads:

- READER: reading data from /proc/stat
- ANALYZER: receivinf data from READER and calculate CPU usage
- PRINTER: printing results from ANALYZER
- LOGGER: receiving status from all threads and save it to file
- WATCHDOG: controlling flow of other threads, if it doesn't receive any signal from threads during 2 seconds, it stops app.

Program uses Queue, mutex and conditional variables to communicate beetwen threads and some new C features like: FAM, VLA, Compound Literals,..

## Run

```c
make
./main.out
```
