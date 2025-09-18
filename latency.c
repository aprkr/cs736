#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

double now_sec(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec * 1e-9;
}
static void write_full(int fd, const void *buf, size_t n) {
  const char *p = (const char *)buf;
  size_t off = 0;
  while (off < n) {
    ssize_t k = write(fd, p + off, n - off);
    if (k > 0)
      off += (size_t)k;
    else if (k < 0 && errno == EINTR)
      continue;
    else {
      perror("write");
      exit(1);
    }
  }
}

static void read_full(int fd, void *buf, size_t n) {
  char *p = (char *)buf;
  size_t off = 0;
  while (off < n) {
    ssize_t k = read(fd, p + off, n - off);
    if (k > 0)
      off += (size_t)k;
    else if (k == 0) {
      fprintf(stderr, "EOF\n");
      exit(1);
    } else if (errno == EINTR)
      continue;
    else {
      perror("read");
      exit(1);
    }
  }
}

int main() {
  int fd1[2], fd2[2];
  int sizes[] = {4,        16,        64,        256,        1024,
                 4 * 1024, 16 * 1024, 64 * 1024, 256 * 1024, 512 * 1024};
  int nsizes = (int)(sizeof(sizes) / sizeof(sizes[0]));

  if (pipe(fd1) || pipe(fd2)) {
    perror("Pipe failed\n");
    exit(1);
  }

  pid_t p = fork();
  if (p < 0) {
    perror("Fork failed\n");
  } else if (p == 0) { // We are in the child
    close(fd1[1]);
    close(fd2[0]); // Not writing to the first or reading from second

    char buf[512 * 1024]; // Max size
    for (int i = 0; i < nsizes; ++i) {
      for (int j = 0; j < 1000; ++j) {
        read_full(fd1[0], buf, sizes[i]);
        write_full(fd2[1], buf, sizes[i]);
      }
    }
    exit(0);
  } // We are in the parent

  close(fd1[0]);
  close(fd2[1]);

  char buf[512 * 1024];
  for (int i = 0; i < nsizes; ++i) {
    double best = 1e9;

    for (int j = 0; j < 1000; ++j) {
      double t0 = now_sec();
      write_full(fd1[1], buf, sizes[i]);
      read_full(fd2[0], buf, sizes[i]);
      double t1 = now_sec();

      if (t1 - t0 < best)
        best = t1 - t0;
    }
    printf("Size %d bytes: one-way latency = %f microseconds \n", sizes[i],
           (best / 2) * 1e6);
  }
  close(fd1[1]);
  close(fd2[0]);
  wait(NULL);

  return 0;
}