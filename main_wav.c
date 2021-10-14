/* main_wav.c: WAV interface for usporth */
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <sndfile.h>
#include <usporth.h>
#include <usputil.h>

#define DEFAULT_SR 44100
#define DEFAULT_T 5
static double t = DEFAULT_T;
static int sr = DEFAULT_SR;
static int t_set = 0, sr_set = 0;
static size_t nframes = DEFAULT_SR*DEFAULT_T;

static SNDFILE *sf_in = NULL, *sf_out = NULL;

static void
sf_init(const char *in, const char *out)
{
  SF_INFO sfinfo = {0};

  if (in) {
    sf_in = sf_open(in, SFM_READ, &sfinfo);
    if (!sf_in)
      die("fail to open input audio file %s", in);
    if (sfinfo.channels != 1)
      die("currently only mono inputs are supported");
    /* update output options */
    sr = sfinfo.samplerate;
    if (t_set)
      sfinfo.frames = t * sr;
    nframes = sfinfo.frames;
  } else {
    sfinfo.samplerate = sr;
    sfinfo.frames = nframes;
    sfinfo.channels = 1;
  }
  /* only 24-bit WAV are supported for now */
  sfinfo.format = SF_FORMAT_WAV|SF_FORMAT_PCM_24;

  sf_out = sf_open(out, SFM_WRITE, &sfinfo);
  if (!sf_out)
    die("fail to open output audio file %s", out);
}

static void
sf_finish(void)
{
  if (sf_in)
    sf_close(sf_in);
  sf_close(sf_out);
}

static usp_pipe *pipes = NULL;
static usp_ctx ctx;
static float sample_in, sample_out;

/* ----------- in ugen redefined ----------- */
ugen_status
ugen_wav_in_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_push_flt(ctx, sample_in);
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
  printf("usage: %s [-h] [-t nsecs] -i in.wav [-o out.wav] in.usp\n", name);
  printf("       %s [-h] [-t nsecs] [-r sr] [-o out.wav] in.usp\n", name);
}

static int
is_wav(const char *s)
{
  s = strrchr(s, '.');
  return s && strcmp(s, ".wav") == 0;
}

int
main(int argc, char *argv[])
{
  char *path, *src;
  char *path_out = "out.wav", *path_in = NULL;
  size_t i;

  int c;
  while ((c = getopt(argc, argv, "hi:o:t:r:")) != -1) {
    switch (c) {
    case 'h':
      usage(argv[0]);
      return 0;
    case 'i':
      path_in = optarg;
      break;
    case 'o':
      path_out = optarg;
      if (!is_wav(path_out))
        die("output file must ends with .wav");
      break;
    case 't':
      t = strtod(optarg, NULL);
      if (t <= 0)
        die("nsecs must be positive");
      t_set = 1;
      break;
    case 'r':
      sr = atol(optarg);
      if (sr < 1)
        die("sample rate must be at least 1");
      sr_set = 1;
      break;
    default:
      usage(argv[0]);
      return 1;
    }
  }

  if (sr_set && path_in != NULL)
    die("unable to set sample rate when input audio is present");
  if (t_set)
    nframes = t * sr;
  sf_init(path_in, path_out);

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
  usp_ugens[UGEN_IN].tick = ugen_wav_in_tick;
  /* 4. init pipeline */
  if (pipes_init(&ctx, pipes) != UGEN_OK)
    die("fail to init pipeline");

  /* 5. compute samples */
  for (i = 0; i < nframes; ++i) {
    if (sf_in)
      sf_readf_float(sf_in, &sample_in, 1);
    pipes_tick(&ctx, pipes);
    sample_out = usp_pop_flt(&ctx);
    sf_writef_float(sf_out, &sample_out, 1);
  }

  /* 6. clean up */
  pipes_free(pipes);
  sf_finish();
  return 0;
}
