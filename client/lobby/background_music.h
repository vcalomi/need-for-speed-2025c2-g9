#ifndef BACKGROUND_MUSIC_H
#define BACKGROUND_MUSIC_H

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QObject>
#include <QUrl>

class BackgroundMusic: public QObject {
    Q_OBJECT
public:
    explicit BackgroundMusic(QObject* parent = nullptr);
    void start();
    void stop();

private:
    QMediaPlayer* player = nullptr;
    QAudioOutput* output = nullptr;
};

#endif
