/* main_text.c: text interface for usporth */
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <usporth.h>
#include <usputil.h>

#define DEFAULT_SR 44100
#define DEFAULT_N 10

static usp_pipe *pipes = NULL;
static usp_ctx ctx;

/* ----------- in ugen redefined ----------- */
ugen_status
ugen_text_in_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt in;
  scanf("%f", &in);
  usp_push_flt(ctx, in);
  return UGEN_OK;
}
/* ----------- /in ugen redefined ----------- */

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
    die("fail to read input file %s", path);
  fclose(fp);
  return buf;
}

static void
usage(const char *name)
{
  printf("usage: %s [-h] [-n nsamples] [-r sr] in.usp\n", name);
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
      if (sr < 1)
        die("sample rate must be at least 1");
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

  /* 1. evaluate source code */
  pipes = usporth_eval(src);
  if (!pipes)
    die("fail to evaluate source code");
  free(src);
  /* 2. init runtime context */
  usporth_init_ctx(&ctx, sr);
  /* 3. overwrite `in` ugen */
  usp_ugens[UGEN_IN].tick = ugen_text_in_tick;
  /* 4. init pipeline */
  if (pipes_init(&ctx, pipes) != UGEN_OK)
    die("fail to init pipeline");

  /* 5. compute samples */
  for (i = 0; i < n; ++i) {
    pipes_tick(&ctx, pipes);
    printf("%.9g\n", usp_pop_flt(&ctx));
  }

  /* 6. clean up */
  pipes_free(pipes);
  return 0;
}
