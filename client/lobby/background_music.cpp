#include "background_music.h"

#include <QUrl>

BackgroundMusic::BackgroundMusic(QObject* parent): QObject(parent) {
    player = new QMediaPlayer(this);
    output = new QAudioOutput(this);

    player->setAudioOutput(output);
    player->setSource(QUrl("qrc:/derezzed.wav"));
    player->setLoops(QMediaPlayer::Infinite);
    output->setVolume(0.2);
}

void BackgroundMusic::start() { player->play(); }
void BackgroundMusic::stop() { player->stop(); }
