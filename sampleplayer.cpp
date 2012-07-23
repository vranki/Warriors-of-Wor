#include "sampleplayer.h"

SamplePlayer::SamplePlayer(QObject *parent) : QObject(parent)
{
    bgChannel = -1;
    audio_rate = 22050;
    audio_format = AUDIO_S16SYS;
    audio_channels = 2;
    audio_buffers = 4096;
    bgIsLoop = true;
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        qDebug() << "Unable to initialize SDL: " << SDL_GetError();
    }
    //Initialize SDL_mixer with our chosen audio settings
    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        qDebug()  << "Unable to initialize audio: " << Mix_GetError();
    }
    loadSample("samples/shoot.wav", GS_SHOOT);
    loadSample("samples/bg.wav", GS_BACKGROUND);
    loadSample("samples/death.wav", GS_DEATH);
    loadSample("samples/spawn.wav", GS_SPAWN);
    loadSample("samples/deathexplosion.wav", GS_DEATHEXPLOSION);
    loadSample("samples/intro_long2.wav", GS_INTROLONG2);
    loadSample("samples/enter.wav", GS_ENTER);
    loadSample("samples/score.wav", GS_SCORE);
    loadSample("samples/bg2.wav", GS_BACKGROUND_2);
    loadSample("samples/enemyshoot.wav", GS_ENEMYSHOOT);
    loadSample("samples/enemyvisible.wav", GS_ENEMYVISIBLE);
    loadSample("samples/bg_worluk.wav", GS_BACKGROUND_WORLUK);
    loadSample("samples/bg_1.wav", GS_BACKGROUND_1);
    loadSample("samples/bg_2.wav", GS_BACKGROUND_2);
    loadSample("samples/bg_3.wav", GS_BACKGROUND_3);
    loadSample("samples/worluk_died.wav", GS_WORLUKDIED);
    loadSample("samples/wizard_died.wav", GS_WIZARDDIED);
    connect(&bgTimer, SIGNAL(timeout()), this, SLOT(nextBgSound()));
    bgTimer.setInterval(1000);
    bgTimer.setSingleShot(false);
}

SamplePlayer::~SamplePlayer() {
    foreach(Mix_Chunk *sound, sounds)
        Mix_FreeChunk(sound);
    Mix_CloseAudio();
    SDL_Quit();
}

void SamplePlayer::loadSample(QString file, gameSample sample) {
    //Load our WAV file from disk
    Mix_Chunk* sound = Mix_LoadWAV(file.toUtf8().data());
    if(sound == NULL) {
        qDebug() << "Unable to load WAV file: " << file << Mix_GetError();
        return;
    }
    sounds[sample] = sound;
}

int SamplePlayer::playSound(gameSample sample, int loops) {
    Mix_Chunk* sound = sounds.value(sample);
    if(!sound) {
        qDebug() << "Can't play sample " << sample;
        return -1;
    }
    //Play our sound file, and capture the channel on which it is played
    channel = Mix_PlayChannel(-1, sound, loops);
    if(channel == -1) {
        qDebug() << "Unable to play sample file: " << Mix_GetError();
    }
    return channel;
}

int SamplePlayer::shoot() {
    return playSound(GS_SHOOT);
}

int SamplePlayer::background(gameSample sample) {
    if(sample >= 0) {
        bgChannel = playSound(sample, -1);
        bgIsLoop = true;
        return bgChannel;
    } else {
        bgSample = 99;
        bgIsLoop = false;
        bgTimer.start();
        nextBgSound();
    }
}

void SamplePlayer::stopbackground() {
    stopSound(bgChannel);
    bgChannel = 0;
    bgIsLoop = true;
    bgTimer.stop();
}

int SamplePlayer::death() {
    return playSound(GS_DEATH);
}

int SamplePlayer::spawn() {
    return playSound(GS_SPAWN);
}

void SamplePlayer::stopSound(int channel) {
    if(channel >= 0) {
        Mix_HaltChannel(channel);
    } else {
        qDebug() << "Trying to stop null channel!!";
    }
}
int SamplePlayer::deathexplosion() {
    return playSound(GS_DEATHEXPLOSION);
}
int SamplePlayer::introLong2() {
    return playSound(GS_INTROLONG2);
}

int SamplePlayer::enter() {
    return playSound(GS_ENTER);
}
int SamplePlayer::score() {
    return playSound(GS_SCORE);
}

int SamplePlayer::enemyShoot() {
    return playSound(GS_ENEMYSHOOT);
}

int SamplePlayer::enemyVisible() {
    return playSound(GS_ENEMYVISIBLE);
}

int SamplePlayer::worlukDied() {
    return playSound(GS_WORLUKDIED);
}

int SamplePlayer::wizardDied() {
    return playSound(GS_WIZARDDIED);
}

void SamplePlayer::setBackgroundInterval(int ms) {
    bgTimer.setInterval(ms);
}

void SamplePlayer::nextBgSound() {
    bgSample++;
    if(bgSample > 14)
        bgSample = 12;
    bgChannel = playSound(GS_BACKGROUND);
}
