#include "MusicPlayer.h"
#include <iostream>

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent),m_mode(ORDER_MODE)
{
    ui.setupUi(this);
   
    setBackGround(":/Image/background2.jpg");
    
    setMinimumSize(670, 300);
    audioOutput = new QAudioOutput(this);
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile("C:\\Users\\Eoyll\\Desktop\\大学作业\\Web\\网站\\material\\只因你太美.mp3"));
    /*mediaPlayer->play();*/
    ui.durationLabel->setStyleSheet("QLabel{color:black;font:15px;background-color:rgba(92, 255, 236,0);border-radius:10px;}");
    ui.durationLabel_2->setStyleSheet("QLabel{color:black;font:15px;background-color:rgba(92, 255, 236,0);border-radius:10px;}");
    ui.musicSlider->setValue(0);
    initButton();
    initMusicList();
    audioOutput->setVolume(0.5);
    musicDir = "E:\\QtProject\\MusicPlayer\\MusicPlayer\\music\\";
    loadAppointMusicDir(musicDir);
    qs = QStack<int>();
   
    albumArt = new RotatingAlbumArt(ui.artWidget);
  

    albumArt->startRotation();

}



MusicPlayer::~MusicPlayer()
{}

void MusicPlayer::handlePlaySlot()
{
    ui.volumeWidget->hide();
    if (mediaPlayer->isPlaying()) {
        mediaPlayer->pause();
        ui.playButton->setIcon(QIcon( ":/Icon/play.png"));
    }
    else {
        mediaPlayer->play();
        ui.playButton->setIcon(QIcon(":/Icon/pause.png"));
    }
    albumArt->startRotation();
}

void MusicPlayer::handleModeSlot()
{
    ui.volumeWidget->hide();
    m_mode = (PLAYMODE)((m_mode + 1) % 3);
    if (m_mode == ORDER_MODE) {
        ui.modeButton->setIcon(QIcon(":/Icon/order.png"));
    }
    else if (m_mode == RANDOM_MODE) {
        ui.modeButton->setIcon(QIcon(":/Icon/random.png"));
    }
    else if (m_mode == CIRCLE_MODE) {
        ui.modeButton->setIcon(QIcon(":/Icon/circle.png"));
    }
   
}

void MusicPlayer::handleNextSlot()
{
    ui.volumeWidget->hide();
    int currentRow = ui.musicList->currentRow();
    int nextRow = 0;
    if (m_mode == ORDER_MODE) {
        nextRow = (currentRow + 1) % ui.musicList->count();
    }
    else if (m_mode == RANDOM_MODE) {
        nextRow = QRandomGenerator::global()->bounded(0, ui.musicList->count());
        qs.push(nextRow);
    }
    else if (m_mode == CIRCLE_MODE) {
        nextRow = currentRow;
    }
    
    ui.musicList->setCurrentRow(nextRow);
    startPlayMusic();
}

void MusicPlayer::handlePrevSlot()
{
    ui.volumeWidget->hide();
    int currentRow = ui.musicList->currentRow();
    int prevRow = 0;
    if (m_mode == ORDER_MODE) {
        prevRow = (currentRow - 1 + ui.musicList->count()) % ui.musicList->count();
    }
    else if (m_mode == RANDOM_MODE) {
        if (qs.size() >= 2) {
            qs.pop();
            prevRow = qs.top();
        }
        else {
            prevRow = QRandomGenerator::global()->bounded(0, ui.musicList->count());
        }
    }
    else if (m_mode == CIRCLE_MODE) {
        prevRow = currentRow;
    }
    ui.musicList->setCurrentRow(prevRow);
    startPlayMusic();
}

void MusicPlayer::handleMusicListSlot()
{
    ui.volumeWidget->hide();
    if (isShowFlag) {
        hideAnimation(ui.musicList);
        ui.musicList->hide();

        isShowFlag = false;
    }
    else {
       
        ui.musicList->show();
        showAnimation(ui.musicList);
        isShowFlag = true;
        
    }
}

void MusicPlayer::handlePositionSlot(int position)
{
    int value = ui.musicSlider->value() / 1000;
    ui.durationLabel->setText(QString::number(value / 60) + ":" + QString::number(value % 60));
    //std::cout << "调用了1次" << std::endl;
    ui.musicSlider->setValue(position);
    
}

void MusicPlayer::handleDurationSlot(int duration)
{
    ui.musicSlider->setRange(0, duration);
   
    //ms转为分秒
    int s = duration / 1000;
    ui.durationLabel_2->setText("/" + QString::number(s / 60) + ":" + QString::number(s % 60));
}

void MusicPlayer::handlePlayFnishSlot()
{
    if (mediaPlayer->mediaStatus() == QMediaPlayer::EndOfMedia) {
        handleNextSlot();
    }
}

void MusicPlayer::handleSliderReleasedSlot()
{
    
    mediaPlayer->setPosition(ui.musicSlider->value());
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::handlePositionSlot);
}

void MusicPlayer::handleSliderPressedSlot()
{
    //防止音乐在播放的时候进度条跳动
    disconnect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::handlePositionSlot);


}

void MusicPlayer::handleVolumeWidgetSlot()
{
    ui.volumeWidget->setVisible(!ui.volumeWidget->isVisible());
}

void MusicPlayer::handleVolumeSliderSlot()
{
    ui.volumeSlider->setValue(int(audioOutput->volume() * 100));
}

void MusicPlayer::handleVolumeLabelSlot(int value)
{
    ui.volumeLabel->setText(QString::number(value) + "%");
    if (value != 0) {
        volume = float(value) / 100;
    }
   
}

void MusicPlayer::handleVolumeNumberSlot(int value)
{
    disconnect(audioOutput, &QAudioOutput::volumeChanged, this, &MusicPlayer::handleVolumeSliderSlot);
    audioOutput->setVolume(float(value) / 100);
    connect(audioOutput, &QAudioOutput::volumeChanged, this, &MusicPlayer::handleVolumeSliderSlot);
}

void MusicPlayer::handleVolumeModeSlot(float volume)
{
    if (volume== 0) {
        ui.volumeModeButton->setIcon(QIcon(":/Icon/volume_close.png"));
        ui.volumeButton->setIcon(QIcon(":/Icon/volume_close.png"));
        
    }
    else if(volume<0.5){
        ui.volumeModeButton->setIcon(QIcon(":/Icon/volume_low.png"));
        ui.volumeButton->setIcon(QIcon(":/Icon/volume_low.png"));
    }
    else {
        ui.volumeModeButton->setIcon(QIcon(":/Icon/volume_high.png"));
        ui.volumeButton->setIcon(QIcon(":/Icon/volume_high.png"));
    }
}

void MusicPlayer::handleVolumeModeButtonSlot()
{

    if (audioOutput->volume() == 0) {
        qDebug() << volume;
        audioOutput->setVolume(volume);
    }
    else {
        audioOutput->setVolume(0);
    }
}

void MusicPlayer::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    setBackGround(":/Image/background2.jpg");
    if (ui.buttonWidget) {
        int btnWidth = ui.buttonWidget->width();
        int btnHeight = ui.buttonWidget->height();
        int x = (this->width() - btnWidth) / 2;
        int y = (this->height() - btnHeight) ;
        ui.buttonWidget->move(x, y);
    }
    if (ui.musicList) {
        int listWidth = ui.musicList->width();
        int listHeight = ui.musicList->height();
        int x = this->width() - listWidth;
        int y = 0;
        ui.musicList->setFixedHeight(this->height() - 100);
        ui.musicList->move(x, y);
    }
    if (ui.musicSlider) {
        int sliderWidth = ui.musicSlider->width();
        int sliderHeight = ui.musicSlider->height();
        int x =0;
        int y = this->height()-sliderHeight-70;
        ui.musicSlider->setFixedWidth(this->width());
        ui.musicSlider->move(x, y);
    }
    if (ui.durationLabel) {
        int durationLabelWidth = ui.durationLabel->width();
        int durationLabelHeight = ui.durationLabel->height();
        int x = 20;
        int y =(ui.buttonWidget->y()*2+ui.buttonWidget->height())/2-durationLabelHeight/2;
        
        ui.durationLabel->move(x, y);
    }
    if (ui.durationLabel_2) {
        int durationLabelWidth = ui.durationLabel_2->width();
        int durationLabelHeight = ui.durationLabel_2->height();
        int x = 55;
        int y = (ui.buttonWidget->y() * 2 + ui.buttonWidget->height()) / 2 - durationLabelHeight / 2;

        ui.durationLabel_2->move(x, y);
    }
    if (ui.volumeWidget) {
        QWidget* parent = ui.volumeButton->parentWidget();
       
        if (parent) {
            QWidget* grandparent = parent->parentWidget();
           
            if (grandparent) {
                QWidget* greatGrandparent = grandparent->parentWidget();
                
                if (greatGrandparent) {
                    QPoint posInGrandparent = ui.volumeButton->mapTo(greatGrandparent, QPoint(0, 0));
                   
                    int width = ui.volumeWidget->width();
                    int height = ui.volumeWidget->height();
                    int x = (posInGrandparent.x() + posInGrandparent.x() + ui.volumeButton->width() - width) / 2;
                    int y = posInGrandparent.y() - 20 - height;
                    ui.volumeWidget->move(x,y);
                    
                }
                
            }
        }
    }
   
}

void MusicPlayer::setBackGround(const QString& filename)
{
    QPixmap pixmap(filename);
    QSize windowSize = this->size();

    QPixmap scalePixmap = pixmap.scaled(windowSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(scalePixmap));

    this->setAutoFillBackground(true);  
    this->setPalette(palette);         
}

void MusicPlayer::setButtonStyle(QPushButton* button, const QString& filename, int width, int height)
{
    button->setFixedSize(width, height);
    button->setIcon(QIcon(filename));
    button->setStyleSheet("background:transparent");
    button->setIconSize(QSize(button->size()));
}

void MusicPlayer::initButton()
{
    setButtonStyle(ui.modeButton, ":/Icon/order.png",50,50);
    setButtonStyle(ui.prevButton, ":/Icon/prev.png", 50, 50);
    setButtonStyle(ui.playButton, ":/Icon/play.png",50, 50);
    setButtonStyle(ui.nextButton, ":/Icon/next.png", 50, 50);
    setButtonStyle(ui.listButton, ":/Icon/music.png", 50, 50);
    setButtonStyle(ui.volumeButton, ":/Icon/volume_low.png", 50, 50);
    setButtonStyle(ui.volumeModeButton, ":/Icon/volume_low.png", 30, 30);
   
 
    ui.volumeWidget->setStyleSheet("QWidget{border: none; border-radius:10px;background-color:rgba( 0, 255, 255,0.9);}");
    ui.volumeWidget->hide();
    //ui.volumeWidget->move();
    
    ui.volumeSlider->setStyleSheet("QSlider{border: none; background-color:rgba( 0, 0, 0,0)}");
    ui.volumeLabel->setStyleSheet("QLabel{border: none; background-color:rgba( 0, 0, 0,0)}");




    connect(ui.playButton,&QPushButton::clicked,this,&MusicPlayer::handlePlaySlot);
    connect(ui.modeButton, &QPushButton::clicked, this, &MusicPlayer::handleModeSlot);
    connect(ui.nextButton, &QPushButton::clicked, this, &MusicPlayer::handleNextSlot);
    connect(ui.prevButton, &QPushButton::clicked, this, &MusicPlayer::handlePrevSlot);
    connect(ui.listButton, &QPushButton::clicked, this, &MusicPlayer::handleMusicListSlot);
    connect(ui.volumeButton, &QPushButton::clicked, this, &MusicPlayer::handleVolumeWidgetSlot);

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::handlePositionSlot);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &MusicPlayer::handleDurationSlot);
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MusicPlayer::handlePlayFnishSlot);

    connect(ui.musicSlider, &QSlider::sliderPressed, this, &MusicPlayer::handleSliderPressedSlot);
    connect(ui.musicSlider, &QSlider::sliderReleased, this, &MusicPlayer::handleSliderReleasedSlot);

    connect(ui.volumeSlider, &QSlider::valueChanged, this,&MusicPlayer::handleVolumeLabelSlot);
    connect(ui.volumeSlider, &QSlider::valueChanged, this, &MusicPlayer::handleVolumeNumberSlot);
    connect(ui.volumeModeButton, &QPushButton::clicked,this, &MusicPlayer::handleVolumeModeButtonSlot);

    connect(audioOutput, &QAudioOutput::volumeChanged, this, &MusicPlayer::handleVolumeModeSlot);
    connect(audioOutput, &QAudioOutput::volumeChanged, this, &MusicPlayer::handleVolumeSliderSlot);
    
}

void MusicPlayer::loadAppointMusicDir(const QString& filepath)
{
    QDir dir(filepath);
    if (!dir.exists()) {  
       
        QMessageBox::warning(this, tr("提示"), tr("文件不存在！"), QMessageBox::Ok);
        return;
    }
    QFileInfoList fileList =  dir.entryInfoList(QDir::Files);

    for (auto &element : fileList) {
        if (element.suffix() == "mp3" || element.suffix()=="ogg") {
            QListWidgetItem* Item = new QListWidgetItem(element.baseName(), ui.musicList);
            Item->setData(Qt::UserRole, element.fileName());
           /* ui.musicList->addItem(element.fileName());*/
        }
    }ui.musicList->setCurrentRow(0);

}

void MusicPlayer::initMusicList()
{
    ui.musicList->setStyleSheet(
        // 整个QListWidget的样式
        "QListWidget {"
        "   background-color: #f0f0f0;"  // 背景色
        "   border: 1px solid #cccccc;"   // 边框
        "   border-radius: 5px;"          // 圆角
        "}"
        // 每个项目的正常状态
        "QListWidget::item {"
        "   background-color: #ffffff;"   // 背景色
        "   border: 1px solid #e0e0e0;"   // 边框
        "   border-radius: 3px;"           // 圆角
        "   padding: 0px;"                // 内边距
        "   margin: 2px;"                 // 外边距
        "   color:black;"
        "   height:50px;"
        "}"
        // 项目被选中时的状态
        "QListWidget::item:selected {"
        "   background-color: #2196f3;"   // 背景色
        "   color: white;"                // 文字颜色
        "}"
        // 项目鼠标悬停时的状态
        "QListWidget::item:hover {"
        "   background-color: #bbdefb;"    // 背景色
        "}"
    );
    connect(ui.musicList, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
        playSelectedMusic(item);
        });
    ui.musicList->hide();
    ui.musicList->setFixedWidth(300);
}

void MusicPlayer::startPlayMusic()
{   
    QString musicName = ui.musicList->currentItem()->data(Qt::UserRole).toString();
    QString musicAbsPath = musicDir + musicName;
    mediaPlayer->setSource(QUrl::fromLocalFile(musicAbsPath));
    handlePlaySlot();
}

void MusicPlayer::playSelectedMusic(QListWidgetItem* item)
{
    QString musicName = item->data(Qt::UserRole).toString();
    QString musicAbsPath = musicDir + musicName;
    mediaPlayer->setSource(QUrl::fromLocalFile(musicAbsPath));
    handlePlaySlot();
}



void MusicPlayer::showAnimation(QWidget* window)
{
    QPropertyAnimation animaiton(window,"pos");
    animaiton.setDuration(500);
    animaiton.setStartValue(QPoint(this->width(),0));
    animaiton.setEndValue(QPoint(this->width()-window->width(), 0));
    animaiton.start();
    QEventLoop loop;
    connect(&animaiton, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void MusicPlayer::hideAnimation(QWidget* window)
{
    QPropertyAnimation animaiton(window, "pos");
    animaiton.setDuration(500);
    animaiton.setStartValue(QPoint(this->width() - window->width(), 0));
    animaiton.setEndValue(QPoint(this->width(), 0));
    animaiton.start();
    QEventLoop loop;
    connect(&animaiton, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
    loop.exec();
    
}
