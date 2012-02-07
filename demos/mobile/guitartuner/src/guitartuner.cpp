/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "guitartuner.h"

#ifdef Q_OS_SYMBIAN
#include <SoundDevice.h>
#endif // Q_OS_SYMBIAN

#if defined(Q_OS_SYMBIAN) && defined(ORIENTATIONLOCK)
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif // Q_OS_SYMBIAN && ORIENTATIONLOCK

GuitarTuner::GuitarTuner(QWidget *parent) :
    QMainWindow(parent)
{

    // Set up the QML.
    m_guitarTunerUI = new QDeclarativeView(QUrl("qrc:/src/application.qml"), this);
    setCentralWidget(m_guitarTunerUI);
    m_guitarTunerUI->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    qmlObject = m_guitarTunerUI->rootObject();

    // Init audio output and input.
    initAudioOutput();
    initAudioInput();

    // Connect the quit signal of m_guitarTunerUI
    // into the close slot of this.
    connect(m_guitarTunerUI->engine(), SIGNAL(quit()), SLOT(close()));

    // Connect the signals from qmlObject into proper slots
    // of this and m_voicegenerator.
    connect(qmlObject, SIGNAL(muteStateChanged(bool)),
            SLOT(muteStateChanged(bool)));
    connect(qmlObject, SIGNAL(volumeChanged(qreal)),
            m_voicegenerator, SLOT(setAmplitude(qreal)));
    connect(qmlObject, SIGNAL(volumeChanged(qreal)),
            SLOT(setMaxVolumeLevel(qreal)));

    // Connect the modeChanged signal from qmlObject
    // into modeChanged slot of this class.
    connect(qmlObject, SIGNAL(modeChanged(bool)),
            SLOT(modeChanged(bool)));

    // Connect the microphoneSensitivityChanged signal from
    // m_guitarTunerUI into setCutOffPercentage slot of m_analyzer class.
    connect(qmlObject, SIGNAL(microphoneSensitivityChanged(qreal)),
            m_analyzer, SLOT(setCutOffPercentage(qreal)));

    // Connect the signals from m_analyzer into slots of qmlObject.
    connect(m_analyzer, SIGNAL(lowVoice()),
            qmlObject, SLOT(lowVoice()));
    connect(m_analyzer, SIGNAL(correctFrequency()),
            qmlObject, SLOT(correctFrequencyObtained()));
    connect(m_analyzer, SIGNAL(voiceDifference(QVariant)),
            qmlObject, SLOT(voiceDifferenceChanged(QVariant)));

    // Initialise the MaximumVoiceDifference
    // value of qmlObject with the value obtained from m_analyzer.
    qmlObject->setProperty("maxVoiceDifference",
                           m_analyzer->getMaximumVoiceDifference());

    // Connect the targetFrequencyChanged signal of qmlObject
    // into targetFrequencyChanged slot of this class.
    connect(qmlObject, SIGNAL(targetFrequencyChanged(qreal)),
            SLOT(targetFrequencyChanged(qreal)));

    // Start voice output or input by using the modeChanged function,
    // depending of the current mode.
    modeChanged(qmlObject->property("isInput").toBool());

}

/**
  * Inits audio output.
  */
void GuitarTuner::initAudioOutput()
{
    // Set up the output format.
    m_format_output.setFrequency(DataFrequencyHzOutput);
    m_format_output.setCodec("audio/pcm");
    m_format_output.setSampleSize(16);
    m_format_output.setChannels(1);
    m_format_output.setByteOrder(QAudioFormat::LittleEndian);
    m_format_output.setSampleType(QAudioFormat::SignedInt);

    // Obtain a default output device, and if the format is not
    // supported, find the nearest format available.
    QAudioDeviceInfo outputDeviceInfo(
                QAudioDeviceInfo::defaultOutputDevice());
    if (!outputDeviceInfo.isFormatSupported(m_format_output)) {
        m_format_output = outputDeviceInfo.nearestFormat(m_format_output);
    }

    // Create new QAudioOutput and VoiceGenerator instances, and store
    // them in m_audioOutput and m_voicegenerator, respectively.
    m_audioOutput = new QAudioOutput(outputDeviceInfo,
                                     m_format_output, this);
    m_voicegenerator = new VoiceGenerator(m_format_output,
                                          qmlObject->property("frequency").toReal(),
                                          qmlObject->property("volume").toReal(),
                                          this);

    // Connect m_audioOutput stateChanged signal to outputStateChanged.
    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)),
            SLOT(outputStateChanged(QAudio::State)));
}

/**
  * Inits audio input.
  */
void GuitarTuner::initAudioInput()
{
    // Set up the input format.
    m_format_input.setFrequency(DataFrequencyHzInput);
    m_format_input.setCodec("audio/pcm");
    m_format_input.setSampleSize(16);
    m_format_input.setChannels(1);
    m_format_input.setByteOrder(QAudioFormat::LittleEndian);
    m_format_input.setSampleType(QAudioFormat::SignedInt);

    // Obtain a default input device, and if the format is not
    // supported, find the nearest format available.
    QAudioDeviceInfo inputDeviceInfo(
                QAudioDeviceInfo::defaultInputDevice());
    if (!inputDeviceInfo.isFormatSupported(m_format_input)) {
        m_format_input = inputDeviceInfo.nearestFormat(m_format_input);
    }

    // Create new QAudioInput and VoiceAnalyzer instances, and store
    // them in m_audioInput and m_analyzer, respectively.
    // Remember to set the cut-off percentage for voice analyzer.
    m_audioInput = new QAudioInput(inputDeviceInfo, m_format_input, this);
    m_analyzer = new VoiceAnalyzer(m_format_input, this);
    m_analyzer->setCutOffPercentage(qmlObject->property("sensitivity").toReal());

}

/**
  * Receives a mode changed signal.
  */
void GuitarTuner::modeChanged(bool isInput)
{



    // If the mode must be changed to input mode:
    if (isInput) {

        // Stop audio output and audio generator.
        m_audioOutput->stop();
        m_voicegenerator->stop();
        // Start the audio analyzer and then the audio input.
        m_analyzer->start(qmlObject->property("frequency").toReal());
        m_audioInput->start(m_analyzer);

    }
    // Else:
    else {

        // Stop audio input and audio analyzer.
        m_audioInput->stop();
        m_analyzer->stop();

        // Set up the audio output.

        // If the current frequency of voice generator
        // is not the same as the target frequency selected in the UI,
        // update voice generator's frequency.
        if (m_voicegenerator->frequency() != qmlObject->property("frequency").toReal()) {
            m_voicegenerator->setFrequency(qmlObject->property("frequency").toReal());
        }

        // Start the voice generator and then the audio output.
        m_voicegenerator->start();
        m_audioOutput->start(m_voicegenerator);
        // Call setMaxVolumeLevel(1) to set the maximum volume louder.
        setMaxVolumeLevel(qmlObject->property("volume").toReal());

        // If the voice is muted, the voice is suspended
        // in the outputStateChanged slot.

    }
}

/**
  * Receives a output state changed signal.
  * Suspends the audio output, if the state is ActiveState
  * and the voice is muted.
  */
void GuitarTuner::outputStateChanged(QAudio::State state)
{
    if (QAudio::ActiveState == state && qmlObject->property("isMuted").toBool()) {
        // If the voice is muted, suspend the voice.
        m_audioOutput->suspend();
    }
}

/**
  * Receives a mute state changed signal.
  * Suspends the audio output or resumes it, depending of the
  * isMuted parameter.
  */
void GuitarTuner::muteStateChanged(bool isMuted)
{
    if (isMuted) {
        m_audioOutput->suspend();
    }
    else {
        m_audioOutput->resume();
    }
}

/**
  * Receives a target frequency signal.
  */
void GuitarTuner::targetFrequencyChanged(qreal targetFrequency)
{
    // If the output mode is active:
    if (!qmlObject->property("isInput").toBool()) {
        // Stop the audio output and voice generator.
        m_audioOutput->stop();
        m_voicegenerator->stop();
        // Set the voice generator's frequency to the target frequency.
        m_voicegenerator->setFrequency(targetFrequency);
        // Start the voice generator and audio output.
        m_voicegenerator->start();
        m_audioOutput->start(m_voicegenerator);
        // Call setMaxVolumeLevel(1) to set the maximum volume louder.
        setMaxVolumeLevel(qmlObject->property("volume").toReal());

        // If the voice is muted, the voice is suspended
        // in the outputStateChanged slot.

    }
    // Else:
    else {

        // Stop the audio input and voice analyzer.
        m_audioInput->stop();
        m_analyzer->stop();
        // Start the voice analyzer with new frequency and audio input.
        m_analyzer->start(targetFrequency);
        m_audioInput->start(m_analyzer);

    }
}

/**
  * This method provides a hack to set the maximum volume level in
  * Symbian.
  */
void GuitarTuner::setMaxVolumeLevel(qreal percent)
{
    if (percent >= 1.0) {
        percent = 1.0;
    }
    else if (percent <= 0.0) {
        percent = 0.0;
    }
    percent = percent*0.5 + 0.5;
    // Warning! This is a hack, which can break when the QtMobility
    // changes. Use at your own risk.
#ifdef Q_OS_SYMBIAN
    unsigned int *pointer_to_abstract_audio
            = (unsigned int*)( (unsigned char*)m_audioOutput + 8 );
    unsigned int *dev_sound_wrapper
            = (unsigned int*)(*pointer_to_abstract_audio) + 16;
    unsigned int *temp
            = ((unsigned int*)(*dev_sound_wrapper) + 6);
    CMMFDevSound *dev_sound = (CMMFDevSound*)(*temp);
    dev_sound->SetVolume((unsigned int)
                         (percent*(float)dev_sound->MaxVolume()));
#endif
}

/**
  * A function used to lock the orientation.
  */
void GuitarTuner::setOrientation(Orientation orientation)
{
    // TODO: Needs a Qt5 implmentation
    Q_UNUSED(orientation)
}
