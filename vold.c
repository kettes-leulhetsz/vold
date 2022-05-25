#include <alloca.h>

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

#include <X11/XF86keysym.h>
#include <X11/Xlib.h>







Display* display;
int screen;
Window rootwindow;
KeyCode up;
KeyCode down;


const char *card = "default";
const char *selem_name = "Master";
snd_mixer_t *handle;
snd_mixer_selem_id_t *sid;


void volume(int updown) {
    long min, max;
    long delta = 0;
    long vol;

    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &vol);

    if (updown == up) {
        delta = 1;
    } else if (updown == down) {
        delta = -1;
    }

    long step = (max - min) / 32;
    vol +=  step * delta;
    if (vol < min) vol = min;
    if (vol > max) vol = max;

    // printf("vol = %ld\n", vol);

    snd_mixer_selem_set_playback_volume_all(elem, vol);
}

void setup() {
    display = XOpenDisplay(0);
    if (!display) {
        exit(1);
    }
    screen = XDefaultScreen(display);
    rootwindow = XRootWindow(display, screen);

    up = XKeysymToKeycode(display, XF86XK_AudioRaiseVolume);
    down = XKeysymToKeycode(display, XF86XK_AudioLowerVolume);

    XGrabKey(display, up, AnyModifier, rootwindow, False, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, down, AnyModifier, rootwindow, False, GrabModeAsync, GrabModeAsync);

    //if(res1 || res2) {
    //    printf("%d %d", res1, res2);
    //    abort();
    //}

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_malloc(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);

}

void run() {
    while (True) {
        XEvent ev;
        XNextEvent(display, &ev);
        switch(ev.type)
        {
            case KeyPress: {
                XKeyPressedEvent *xev = (XKeyPressedEvent*) &ev;
                // printf("KeyPress %d %d %d\n", (int) up, xev->keycode, (int) down);
                // fflush(stdout);

                if (xev->keycode == up) {
                        volume(up);
                };
                if (xev->keycode == down) {
                        volume(down);
                };
            }
            default:
                break;
        }
    }
}



int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    setup();

    run();
    return 0;
}
