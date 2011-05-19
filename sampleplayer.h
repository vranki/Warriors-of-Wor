#ifndef SAMPLEPLAYER_H
#define SAMPLEPLAYER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <QString>
#include <QMap>
#include <QDebug>
#include <QTimer>

class SamplePlayer
{
    enum gameSample {
        GS_SHOOT = 1,
        GS_BACKGROUND,
        GS_DEATH,
        GS_SPAWN,
        GS_DEATHEXPLOSION,
        GS_INTROLONG2,
        GS_ENTER,
        GS_SCORE,
        GS_ENEMYSHOOT,
        GS_ENEMYVISIBLE,
        GS_BACKGROUND_1,
        GS_BACKGROUND_2,
        GS_BACKGROUND_3,
        GS_WORLUKDIED,
        GS_WIZARDDIED
    };

public:
    SamplePlayer();
    ~SamplePlayer();
    void loadSample(QString file, gameSample sample);
    int playSound(gameSample sample, int loops=0);
    void stopSound(int channel);
    void setBackgroundInterval(int ms);
public slots:
    int shoot();
    int background(gameSample sample = 1);
    void stopbackground();
    int death();
    int spawn();
    int deathexplosion();
    int introLong2();
    int enter();
    int score();
    int enemyShoot();
    int enemyVisible();
    int worlukDied();
    int wizardDied();
private slots:
    void nextBgSound();
private:
    QMap<gameSample, Mix_Chunk*> sounds;
    int channel;		//Channel on which our sound is played
    int audio_rate;		//Frequency of audio playback
    Uint16 audio_format; 	//Format of the audio we're playing
    int audio_channels;		//2 channels = stereo
    int audio_buffers;		//Size of the audio buffers in memory
    int bgChannel;
    int bgSample;
    bool bgIsLoop;
    QTimer bgTimer;
};

#endif // SAMPLEPLAYER_H
