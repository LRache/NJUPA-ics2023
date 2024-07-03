#include <NDL.h>
#include <SDL.h>
#include <unistd.h>

static int pause_on = 0;
static void (*callback)(void *userdata, uint8_t *stream, int len) = NULL;

void CallbackHelper() {
  printf("CallBack\n");
  if (NDL_QueryAudio() < 8192) {
    return ;
  }
  printf("CallBack\n");
  if (callback != NULL) {
    uint8_t buffer[8192];
    callback(NULL, buffer, 8192);
    NDL_PlayAudio(buffer, 8192);
  }
  printf("CallBack\n");
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
