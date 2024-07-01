#include <NDL.h>
#include <SDL.h>
#include <unistd.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  char buffer[16];
  read(3, buffer, sizeof(buffer));
  if (strcmp(buffer, "mmap ok\n") == 0) {
    event->type = SDL_USEREVENT;
  } else {
    char keynameS[8], type;
    sscanf("%c %s", &type, keynameS);
    if (type == 'u') event->type = SDL_KEYUP;
    else if (type == 'd') event->type = SDL_KEYDOWN;
    for (int i = 0; i < SDLK_COUNT; i++) {
      if (strcmp(keynameS, keyname[i]) == 0) {
        event->key.keysym.sym = i;
        break;
      }
    }
  }
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
