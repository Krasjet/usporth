/* main_jack.c: jack interface for usporth */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <jack/jack.h>
#include <usporth.h>
#include <usputil.h>

static volatile int done = 0;

static jack_client_t *client = NULL;
static jack_port_t *port_out = NULL, *port_in = NULL;
static jack_nframes_t sr;

static usp_pipe *pipes = NULL;
static usp_ctx ctx;

/* ----------- in ugen redefined ----------- */
static usp_flt sample_in;

ugen_status
ugen_jack_in_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_push_flt(ctx, sample_in);
  return UGEN_OK;
}
/* ----------- /in ugen redefined ----------- */

static void
on_signal(int signum)
{
  done = 1;
}

static int
on_process(jack_nframes_t nframes, void *arg)
{
  usp_flt *in, *out;
  jack_nframes_t i;

  in  = jack_port_get_buffer(port_in,  nframes);
  out = jack_port_get_buffer(port_out, nframes);

  for (i = 0; i < nframes; ++i) {
    sample_in = in[i];
    pipes_tick(&ctx, pipes);
    out[i] = usp_pop_flt(&ctx);
  }

  return 0;
}

static void
on_shutdown(void *arg)
{
  client = NULL;
  die("jack server is down, exiting...");
}

static void
jack_init(void)
{
  client = jack_client_open("usporth", JackNoStartServer, NULL);
  if (!client)
    die("fail to open client");

  sr = jack_get_sample_rate(client);

  jack_on_shutdown(client, on_shutdown, NULL);
  if (jack_set_process_callback(client, on_process, NULL) != 0)
    die("fail to set up process callback");

  port_in  = jack_port_register(client, "in", JACK_DEFAULT_AUDIO_TYPE,
                                JackPortIsInput, 0);
  port_out = jack_port_register(client, "out", JACK_DEFAULT_AUDIO_TYPE,
                                JackPortIsOutput, 0);
  if (!port_out)
    die("fail to register ports");
}

static void
jack_autoconnect(void)
{
  const char **ports = jack_get_ports(client, NULL, NULL,
    JackPortIsInput|JackPortIsPhysical);
  if (ports) {
    int i;
    for (i = 0; ports[i]; ++i)
      jack_connect(client, jack_port_name(port_out), ports[i]);
    jack_free(ports);
  }
}

static void
jack_finish(void)
{
  jack_deactivate(client);
  jack_client_close(client);
}

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
  printf("usage: %s in.usp\n", name);
}

int
main(int argc, char *argv[])
{
  char *src;
  jack_init();

  if (argc < 2) {
    usage(argv[0]);
    return 1;
  }
  src = read_file(argv[1]);

  /* 1. evaluate source code */
  pipes = usporth_eval(src);
  if (!pipes)
    die("fail to evaluate source code");
  free(src);
  /* 2. init runtime context */
  usporth_init_ctx(&ctx, sr);
  /* 3. overwrite `in` ugen */
  usp_ugens[UGEN_IN].tick = ugen_jack_in_tick;
  /* 4. init pipeline */
  if (pipes_init(&ctx, pipes) != UGEN_OK)
    die("fail to init pipeline");

  /* 5. start jack */
  if (jack_activate(client) != 0)
    die("fail to activate client");
  jack_autoconnect();

  /* catch signals for grace shutdown */
  signal(SIGINT, on_signal);
  signal(SIGTERM, on_signal);
#ifndef _WIN32
  signal(SIGQUIT, on_signal);
  signal(SIGHUP, on_signal);
#endif

  while (!done)
    sleep(1); /* idle main thread */

  /* 6. clean up */
  jack_finish();
  pipes_free(pipes);
  return 0;
}
