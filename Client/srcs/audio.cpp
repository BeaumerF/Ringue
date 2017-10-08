#include "audio.h"
#include "ui_audio.h"
#include <QDebug>

#include <iostream>

const int BufferSize = 14096;

Audio::Audio(QWidget *parent) :
    QMainWindow(parent),
       m_Inputdevice(QAudioDeviceInfo::defaultInputDevice())
    ,   m_Outputdevice(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioInput(0)
    ,   m_audioOutput(0)
    ,   m_input(0)
    ,   m_iVolume(50)
    ,   m_buffer(BufferSize, 0)
    ,   ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeAudio();
    m_output = m_audioOutput->start(); // start(socket);
    //https://stackoverflow.com/questions/25713243/udp-voice-chat-on-qt
    m_input = m_audioInput->start();
    //std::cout << m_audioOutput->start() << std::endl;
    //connect(m_input, SIGNAL(readyRead()), SLOT(readMore()));
}

Audio::~Audio()
{
    delete ui;
}

void Audio::initializeAudio()
{
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/opus");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt );

    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());
    if (!infoIn.isFormatSupported(m_format))
    {
        m_format = infoIn.nearestFormat(m_format);
    }

    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());

    if (!infoOut.isFormatSupported(m_format))
    {
        m_format = infoOut.nearestFormat(m_format);
    }
    createAudioInput();
    createAudioOutput();
}

void Audio::createAudioOutput()
{
    m_audioOutput = new QAudioOutput(m_Outputdevice, m_format, this);
}
void Audio::createAudioInput()
{
    if (m_input != 0) {
        disconnect(m_input, 0, this, 0);
        m_input = 0;
    }

    m_audioInput = new QAudioInput(m_Inputdevice, m_format, this);

}
void Audio::readMore()
{
    if(!m_audioInput)
        return;

    qint64 len = m_audioInput->bytesReady();

    if(len > 4096)
        len = 4096;
    qint64 l = m_input->read(m_buffer.data(), len);
    if(l > 0)
    {
        short* resultingData = (short*)m_buffer.data();


        short *outdata=resultingData;
        outdata[ 0 ] = resultingData [ 0 ];

         int iIndex;
         if(ui->chkRemoveNoise->checkState() == Qt::Checked)
         {
                for ( iIndex=1; iIndex < len; iIndex++ )
                {
                    outdata[ iIndex ] = 0.333 * resultingData[iIndex ] + ( 1.0 - 0.333 ) * outdata[ iIndex-1 ];
                }
          }

         for ( iIndex=0; iIndex < len; iIndex++ )
         {
             outdata[ iIndex ] = ApplyVolumeToSample( outdata[ iIndex ]);
         }

          m_output->write((char*)outdata, len);

    }

}

int Audio::ApplyVolumeToSample(short iSample)
{
    return std::max(std::min(((iSample * m_iVolume) / 50) ,35535), -35535);

}

void Audio::on_horizontalSlider_valueChanged(int value)
{
    m_iVolume = value;
}
