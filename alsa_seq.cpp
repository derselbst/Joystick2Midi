#include <cstdio>
#include <alsa/asoundlib.h>

#include "alsa_seq.h"

static snd_seq_t * seq;
static int source_port;

snd_seq_t * connect()
{
    snd_seq_t * s;
    if (snd_seq_open(&s, "default", SND_SEQ_OPEN_OUTPUT, 0) < 0)
    {
        fprintf(stderr, "Error: snd_seq_open\n");
        return NULL;
    }

    if (snd_seq_set_client_name(s, "JoystickWriter") < 0)
    {
        fprintf(stderr, "Could not set client name\n");
    }

    source_port = snd_seq_create_simple_port(s, "Joystick", SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);
    if (source_port < 0)
    {
        fprintf(stderr, "Error: snd_seq_create_simple_port\n");
        snd_seq_close(s);
        return NULL;
    }

    seq=s;
    return s;
}

snd_seq_event_t ev;

void send_ev()
{
    // pass event diretly to seq (not via buffer)
    snd_seq_ev_set_direct(&ev);
    snd_seq_ev_set_source(&ev, source_port);

    // broadcast to subscribers
    snd_seq_ev_set_subs(&ev);
//    snd_seq_ev_set_dest(&ev, dest_client, dest_port);

    int i = snd_seq_event_output(seq, &ev);
    snd_seq_drain_output(seq);
}

void note_on(int note, int velocity, int channel)
{
    snd_seq_ev_set_noteon(&ev, channel, note, velocity);
    send_ev();
}

void note_off(int note, int velocity, int channel)
{
    snd_seq_ev_set_noteoff(&ev, channel, note, velocity);
    send_ev();
}

void controller(int controller, int val, int channel)
{
    snd_seq_ev_set_controller(&ev, channel, controller, val);
    send_ev();
}


// val: -8192 to 8191
void pitch(int val, int channel)
{
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_pitchbend(&ev, channel, val);
    send_ev();
}
