#include "background_music.h"

#include <QUrl>

BackgroundMusic::BackgroundMusic(QObject* parent): QObject(parent) {
    player = new QMediaPlayer(this);
    output = new QAudioOutput(this);

    player->setAudioOutput(output);
    player->setSource(QUrl("qrc:/derezzed.mp3"));
    player->setLoops(QMediaPlayer::Infinite);
    output->setVolume(0.2);
    QObject::connect(player, &QMediaPlayer::mediaStatusChanged, this,
                     [this](QMediaPlayer::MediaStatus s) {
                         if (s == QMediaPlayer::EndOfMedia)
                             player->play();
                     });
}

void BackgroundMusic::start() { player->play(); }
void BackgroundMusic::stop() { player->stop(); }
