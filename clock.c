#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main() {
  struct timespec start_time;
  struct timespec end_time;

  if (clock_gettime(CLOCK_REALTIME, &start_time) != 0) {
    perror("Erorr getting the time!\n");
  }

  sleep(10);

  if (clock_gettime(CLOCK_REALTIME, &end_time) != 0) {
    perror("Error getting the time!\n");
  }

  printf("Start time: %ld seconds, %ld nanoseconds since Epoch.\n",
         start_time.tv_sec, start_time.tv_nsec);

  printf("End time: %ld seconds, %ld nanoseconds since Epoch.\n",
         end_time.tv_sec, end_time.tv_nsec);

  long delta_seconds = end_time.tv_sec - start_time.tv_sec;
  long delta_nu = end_time.tv_nsec - start_time.tv_nsec;

  printf("Delta seconds: %ld, Delta nanoseconds: %ld", delta_seconds, delta_nu);

  return 0;
}