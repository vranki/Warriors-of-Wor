#ifndef SAMPLEPLAYER_H
#define SAMPLEPLAYER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <QString>
#include <QList>
#include <QDebug>

class SamplePlayer
{
public:
    SamplePlayer();
    ~SamplePlayer();
    int loadSample(QString file);
    int playSound(int num, int loops=0);
    void stopSound(int channel);
public slots:
    int shoot();
    int background(int sample = 1);
    void stopbackground();
    int death();
    int spawn();
    int deathexplosion();
    int introLong2();
    int enter();
    int score();
    int enemyShoot();
    int enemyVisible();
private:
    QList<Mix_Chunk*> sounds;
    int channel;				//Channel on which our sound is played
    int audio_rate;			//Frequency of audio playback
    Uint16 audio_format; 	//Format of the audio we're playing
    int audio_channels;			//2 channels = stereo
    int audio_buffers;		//Size of the audio buffers in memory
    int bgChannel;
};

#endif // SAMPLEPLAYER_H
