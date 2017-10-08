#ifndef AUDIO_H
#define AUDIO_H

#include <QMainWindow>
#include <QByteArray>
#include <qaudioinput.h>
#include <qaudiooutput.h>
#include <QBuffer>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>

namespace Ui {
    class MainWindow;
}


class Audio : public QMainWindow
{
    Q_OBJECT

public:
    explicit Audio(QWidget *parent = 0);
    ~Audio();

private:
    void initializeAudio();
    void createAudioInput();
    void createAudioOutput();

    int ApplyVolumeToSample(short iSample);

private slots:
    void readMore();
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QAudioDeviceInfo m_Inputdevice;
    QAudioDeviceInfo m_Outputdevice;
    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;
    QIODevice *m_input;
    QIODevice *m_output;
    QByteArray m_buffer;
    int m_iVolume;

};

#endif // AUDIO_H
