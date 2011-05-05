#include "sampleplayer.h"

SamplePlayer::SamplePlayer()
{
    bgChannel = -1;
    audio_rate = 22050;
    audio_format = AUDIO_S16SYS;
    audio_channels = 2;
    audio_buffers = 4096;
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        qDebug() << "Unable to initialize SDL: " << SDL_GetError();
    }
    //Initialize SDL_mixer with our chosen audio settings
    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        qDebug()  << "Unable to initialize audio: " << Mix_GetError();
    }
    loadSample("samples/shoot.wav");
    loadSample("samples/bg.wav");
    loadSample("samples/death.wav");
    loadSample("samples/spawn.wav");
    loadSample("samples/deathexplosion.wav");
    loadSample("samples/intro_long2.wav");
    loadSample("samples/enter.wav");
    loadSample("samples/score.wav");
    loadSample("samples/bg2.wav");
    loadSample("samples/enemyshoot.wav");
    loadSample("samples/enemyvisible.wav");
    loadSample("samples/bg_worluk.wav");
}

SamplePlayer::~SamplePlayer() {
    foreach(Mix_Chunk *sound, sounds)
        Mix_FreeChunk(sound);
    Mix_CloseAudio();
    SDL_Quit();
}

int SamplePlayer::loadSample(QString file)
{
    //Load our WAV file from disk
    Mix_Chunk* sound = Mix_LoadWAV(file.toUtf8().data());
    if(sound == NULL) {
        qDebug() << "Unable to load WAV file: " << file << Mix_GetError();
        return -1;
    }
    sounds.append(sound);
    return sounds.size()-1;
}

int SamplePlayer::playSound(int num, int loops) {
    Mix_Chunk* sound = sounds.value(num);
    if(!sound) {
        qDebug() << "Can't play sample " << num;
        return -1;
    }
    //Play our sound file, and capture the channel on which it is played
    channel = Mix_PlayChannel(-1, sound, loops);
    if(channel == -1) {
        qDebug() << "Unable to play WAV file: " << Mix_GetError();
    }
    return channel;
}
int SamplePlayer::shoot() {
    return playSound(0);
}

int SamplePlayer::background(int sample) {
    bgChannel = playSound(sample, -1);
    return bgChannel;
}
void SamplePlayer::stopbackground() {
    stopSound(bgChannel);
    bgChannel = 0;
}
int SamplePlayer::death() {
    return playSound(2);
}
int SamplePlayer::spawn() {
    return playSound(3);
}
void SamplePlayer::stopSound(int channel) {
    if(channel >= 0) {
        Mix_HaltChannel(channel);
    } else {
        qDebug() << "Trying to stop null channel!!";
    }
}
int SamplePlayer::deathexplosion() {
    return playSound(4);
}
int SamplePlayer::introLong2() {
    return playSound(5);
}

int SamplePlayer::enter() {
    return playSound(6);
}
int SamplePlayer::score() {
    return playSound(7);
}

int SamplePlayer::enemyShoot() {
    return playSound(9);
}
int SamplePlayer::enemyVisible() {
    return playSound(10);
}
