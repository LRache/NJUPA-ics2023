#include <NDL.h>
#include <SDL.h>
#include <unistd.h>

#define BUF_SIZE 4096

static int pause_on = 0;
static uint8_t buffer[BUF_SIZE];
static void (*callback)(void *userdata, uint8_t *stream, int len) = NULL;
extern SDL_Surface *screen;

void CallbackHelper() {
  printf("buffer at %p, %p\n", buffer, buffer+BUF_SIZE);
  if (NDL_QueryAudio() < BUF_SIZE) {
    return ;
  }
  if (callback != NULL) {
    callback(NULL, buffer, BUF_SIZE);
    //NDL_PlayAudio(buffer, BUF_SIZE);
    printf("at call back %p\n", screen->pixels);
    return;
  }
}

int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained) {
  NDL_OpenAudio(desired->freq, desired->channels, desired->samples);
  callback = desired->callback;
  printf("SDL_OpenAudio\n");
  return 0;
}

void SDL_CloseAudio() {
}

void SDL_PauseAudio(int pause_on) {
}

void SDL_MixAudio(uint8_t *dst, uint8_t *src, uint32_t len, int volume) {
}

SDL_AudioSpec *SDL_LoadWAV(const char *file, SDL_AudioSpec *spec, uint8_t **audio_buf, uint32_t *audio_len) {
  return NULL;
}

void SDL_FreeWAV(uint8_t *audio_buf) {
}

void SDL_LockAudio() {
}

void SDL_UnlockAudio() {
}
