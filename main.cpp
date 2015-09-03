#include <iostream>

#include <SDL.h>

#include "alsa_seq.h"

using namespace std;

const int Y_AXIS = 1;
const int L_AXIS = 4;
const int R_AXIS = 5;

// some how map 16 bit value to 14 bit value used for pitchbend
inline int AxisToPitchBend(int16_t axis)
{
    return axis>>2;
}

int main()
{
    // FIXME: We don't need video, but without it SDL will fail to work in SDL_WaitEvent()
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    snd_seq_t * seq = connect();
    if (!seq) return 1;

// Check for joystick
    if(SDL_NumJoysticks()>0)
    {
        // Open joystick
        SDL_Joystick *joy=SDL_JoystickOpen(0);

        if(joy)
        {
            printf("Opened Joystick 0\n");
            printf("Name: %s\n", SDL_JoystickName(0));
            printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
            printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
            printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));


            Sint16* axes    = (Sint16*)calloc(SDL_JoystickNumAxes(joy),    sizeof(Sint16));
            Uint8*  buttons = (Uint8*)calloc(SDL_JoystickNumButtons(joy), sizeof(Uint8));
            Uint8*  balls   = (Uint8*)calloc(SDL_JoystickNumBalls(joy),   2*sizeof(Sint16));


            bool quit = false;
            SDL_Event event;
            while(!quit)
            {
                SDL_Delay(50);

                bool something_new = false;
                while (SDL_PollEvent(&event))
                {
                    switch(event.type)
                    {
                    case SDL_JOYAXISMOTION:
                        if(event.jaxis.axis==L_AXIS || event.jaxis.axis==R_AXIS)
                        {
                            something_new = true;
                        }
                        axes[event.jaxis.axis] = event.jaxis.value;
                        break;

                    case SDL_JOYBUTTONDOWN:
                    case SDL_JOYBUTTONUP:
                        buttons[event.jbutton.button] = event.jbutton.state;
                        break;

                    case SDL_JOYHATMOTION:
                        // TODO
                        break;

                    case SDL_JOYBALLMOTION:
                        balls[2*event.jball.ball + 0] = event.jball.xrel;
                        balls[2*event.jball.ball + 1] = event.jball.yrel;
                        break;

                    case SDL_QUIT:
                        quit = 1;
                        printf("Recieved interrupt, exiting\n");
                        break;

                    default:
                        fprintf(stderr, "Error: Unhandled event type: %d\n", event.type);
                        break;
                    }
                }

                if (something_new)
                {
                    cout << "L: " << axes[L_AXIS] <<"\tR: " << axes[R_AXIS] << endl;
                    int pitchUp = AxisToPitchBend(axes[L_AXIS]);
                    int pitchDown = AxisToPitchBend(axes[R_AXIS]);

                    pitch(pitchUp-pitchDown);
                }
            }
        }
        else
        {
            printf("Couldn't open Joystick 0\n");
        }

        // Close if opened
        if(SDL_JoystickOpened(0))
        {
            SDL_JoystickClose(joy);
        }

        snd_seq_close(seq);
    }
}
