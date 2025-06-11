#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MusicPlayer.h"
#include<qevent.h>
#include<qmediaplayer.h>
#include<qaudiooutput.h>
#include<qfile.h>
#include<qdir.h>
#include<qfileinfo.h>
#include<qmessagebox.h>
#include<qrandom.h>
#include<qstack.h>
#include<qpropertyanimation.h>
#include"RotatingAlbumArt.h"
enum PLAYMODE
{
       ORDER_MODE,
       RANDOM_MODE,
       CIRCLE_MODE
};
class MusicPlayer : public QMainWindow
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = nullptr);
   
    ~MusicPlayer();

public slots:
    void handlePlaySlot();
    void handleModeSlot();
    void handleNextSlot();
    void handlePrevSlot();
    void handleMusicListSlot();
    void handlePositionSlot(int position);
    void handleDurationSlot(int duration);
    void handlePlayFnishSlot();

    void handleSliderReleasedSlot();
    void handleSliderPressedSlot();
    void handleVolumeWidgetSlot();

    void handleVolumeSliderSlot();
    void handleVolumeLabelSlot(int value);
    void handleVolumeNumberSlot(int value);
    void handleVolumeModeSlot(float volume);
    void handleVolumeModeButtonSlot();
protected:
    void resizeEvent(QResizeEvent* event);
private:
    void setBackGround(const QString & filename);
    void setButtonStyle(QPushButton* button,const QString & filename,int width,int height);
    void initButton();
    void loadAppointMusicDir(const QString & filepath);
    void initMusicList();
    void startPlayMusic();
    void playSelectedMusic(QListWidgetItem* item);
   
    void showAnimation(QWidget* window);
    void hideAnimation(QWidget* window);


private:
    Ui::MusicPlayerClass ui;
    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;
    PLAYMODE m_mode;
    QStack<int>qs;
    QString musicDir;
    bool isShowFlag;
    float volume;

    RotatingAlbumArt * albumArt;
};
