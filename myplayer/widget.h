#ifndef WIDGET_H
#define WIDGET_H

#include<QProcess>
#include<QtGui>
#include <QIcon>
#include <QPixmap>
#include    "mylist.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <semaphore.h>


class MyPlaylist;

class Widget : public QWidget
{
    Q_OBJECT
public:
   explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    //void stopButton_clicked();
    void    musicmessage();
     void getTimepos();
    void catchOutput();
    void openButton_clicked();
    void playButton_clicked();
   // void stopButton_clicked();
    void SetPlayListShown();
    void    SetLrcShown();
    void    SkipBackward();
    void    SkipForward();
    void    Setexit();
    void TrayIconActivated(QSystemTrayIcon::ActivationReason activation_reason);
    void    TableClicked(int);
    void ClearSources();
    void    statechanged();
    void    sound_off();

   /* void mplayerEnded(int exitCode, QProcess::ExitStatus exitStatus);

    void playButton_clicked();

    void tvButton_clicked();
    void exitButton_clicked();


    void on_m_slider_sliderMoved(int position);

    void on_m_slider_sliderPressed();

    void on_m_slider_sliderReleased();*/


private:
    void    makelist();
    void    set_volume();
    void    get_music_message(QString   string);
    sem_t sem;
    void sleep(unsigned int msec);

    bool stopMPlayer();
    bool    startMPlayer(int pos,int row);
    bool pauseMPlayer();
    QProcess    *   mplayer_process;
    QProcess    *   get_process;
    QVBoxLayout *   mainLay;
    QIcon   *   stopIcon;
    QSlider *   m_slider;
    QSlider *   volume_slider;
    QTimer  *   m_timer;
    QString m_movieFile;
    bool m_isPlaying;
    bool m_isPause;
    bool    m_issound;
    QIcon m_playIcon;
    QIcon m_pauseIcon;
    QIcon   m_openlcon;
    QAction *play_action;
    //QAction *stop_action;
    QAction *skip_backward_action;
    QAction *skip_forward_action;
    QAction *exit_action;
    QAction *sound_action;
    QLabel *top_label;
    QLabel *time_label;
    QSystemTrayIcon *tray_icon;
    int max_time;
     QTime    total_time;
     QTime    current_time;
     MyPlaylist *playlist;
    QString current_filename;
     QStringList    nameList;
     QStringList    timeList;
     QStringList    artistList;
     QStringList    file_name;
     int    current_volume;
protected:
     void paintEvent(QPaintEvent*);
signals:
    void mySignal();
};
#endif // WIDGET_H
