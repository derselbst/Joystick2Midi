#ifndef ALSA_SEQ_H_INCLUDED
#define ALSA_SEQ_H_INCLUDED

#include <alsa/asoundlib.h>

snd_seq_t * connect();
void controller(int controller, int val, int channel=0);
void note_off(int note, int velocity=60, int channel=0);
void note_on(int note, int velocity=60, int channel=1);
void pitch(int val, int channel=0);


#endif // ALSA_SEQ_H_INCLUDED
