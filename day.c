#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
  struct timeval start_time;
  struct timeval end_time;

  if (gettimeofday(&start_time, NULL) != 0) {
    perror("Erorr getting the time!\n");
  }

  sleep(10);

  if (gettimeofday(&end_time, NULL) != 0) {
    perror("Error getting the time!\n");
  }

  printf("Start time: %ld seconds, %ld microseconds since Epoch.\n",
         start_time.tv_sec, start_time.tv_usec);

  printf("End time: %ld seconds, %ld microseconds since Epoch.\n",
         end_time.tv_sec, end_time.tv_usec);

  long delta_seconds = end_time.tv_sec - start_time.tv_sec;
  long delta_mu = end_time.tv_usec - start_time.tv_usec;

  printf("Delta seconds: %ld, Delta microseconds: %ld", delta_seconds,
         delta_mu);

  return 0;
}