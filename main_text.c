/* main_text.c: text interface for usporth */
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "util.h"
#include "usporth.h"

#define DEFAULT_SR 44100
#define DEFAULT_N 10

static volatile int done = 0;
static usp_pipe *pipes = NULL;
static usp_ctx ctx;

static char *
read_file(const char *path)
{
  long len;
  size_t read;
  char *buf = NULL;
  FILE *fp = NULL;

  fp = fopen(path, "r");
  if (!fp)
    die("fail to open input file %s", path);

  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  rewind(fp);

  buf = xcalloc(len + 1, 1);
  read = fread(buf, 1, len, fp);
  if ((long)read < len)
    goto err;
  fclose(fp);
  return buf;
err:
  if (fp)
    fclose(fp);
  free(buf);
  return NULL;
}

static void
usage(const char *name)
{
  printf("usage: %s [-n nsamples] [-r sr] input.usp\n", name);
}

int
main(int argc, char *argv[])
{
  char *path, *src;
  int sr = DEFAULT_SR;
  long i, n = DEFAULT_N;

  int c;
  while ((c = getopt(argc, argv, "hn:r:")) != -1) {
    switch (c) {
    case 'h':
      usage(argv[0]);
      return 0;
    case 'n':
       n = atol(optarg);
      if (n < 1)
        die("nsamples must be at least 1");
      break;
    case 'r':
      sr = atol(optarg);
      if (sr < 1) {
        die("sample rate must be at least 1");
      }
      break;
    default:
      usage(argv[0]);
      return 1;
    }
  }

  if (optind < argc) {
    path = argv[optind];
  } else {
    usage(argv[0]);
    return 1;
  }
  src = read_file(path);
  if (!src)
    die("fail to read input file %s", argv[1]);

  /* 1. evaluate source code */
  pipes = usporth_eval(src);
  if (!pipes)
    die("fail to evaluate source code");
  free(src);
  /* 2. init runtime context */
  usporth_init_ctx(&ctx, sr);
  /* 3. init pipeline */
  pipes_init(&ctx, pipes);

  /* 4. compute samples */
  for (i = 0; i < n; ++i) {
    pipes_tick(&ctx, pipes);
    printf("%.9g\n", usp_pop_flt(&ctx));
  }

  /* 5. clean up */
  pipes_free(pipes);
  return 0;
}
