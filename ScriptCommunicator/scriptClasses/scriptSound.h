#ifndef SCRIPTSOUND_H
#define SCRIPTSOUND_H


#include <QAudioOutput>
#include "scriptObject.h"

#ifdef Q_OS_WIN32
#include <QMediaPlayer>
#else
#include <QSoundEffect>
#endif

///This wrapper class is used to access a QSound object from a script.
class ScriptSound : public QObject, public ScriptObject
{
    Q_OBJECT

    ///Returns a semicolon separated list with all public functions, signals and properties.
    Q_PROPERTY(QString publicScriptElements READ getPublicScriptElements CONSTANT)

public:
    explicit ScriptSound(QObject *parent, QString filename) : QObject(parent), m_player(this)
    {
        //connect the necessary signals with the wrapper slots (in this slots the
        //events of the wrapper class are generated, the script can connect to this
        //wrapper events)

#ifdef Q_OS_WIN32
      m_player.setAudioOutput(&m_audioOutput);
      m_player.setSource(QUrl::fromLocalFile(filename));
      m_audioOutput.setVolume(100);
#else
      m_player.setSource(QUrl::fromLocalFile(filename));
#endif

    }

    ///Returns a semicolon separated list with all public functions, signals and properties.
    virtual QString getPublicScriptElements(void)
    {
        return MainWindow::parseApiFile("ScriptSound.api");
    }

    ///Returns the filename associated with this script sound object.
    Q_INVOKABLE QString fileName(void){return m_player.source().fileName();}

    ///Returns true if the sound has finished playing; otherwise returns false.
    Q_INVOKABLE bool isFinished(void)
    {
#ifdef Q_OS_WIN32
        return (m_player.playbackState() == QMediaPlayer::PlayingState) ? false : true;
#else
        return !m_player.isPlaying();
#endif
    }

    ///Starts playing the sound specified by this QSound object.
    Q_INVOKABLE void play(void){m_player.play();}

    ///Stops the sound playing.
    Q_INVOKABLE void stop(void){m_player.stop();}


signals:


private:

#ifdef Q_OS_WIN32
    ///The wrapped QSoundEffect object.
    QMediaPlayer m_player;
    QAudioOutput m_audioOutput;
#else
    QSoundEffect m_player;
#endif

};
#endif // SCRIPTSOUND_H

