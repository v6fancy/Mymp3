#include"widget.h"


//#include <QWSServer>

#define MPLAYER_PATH    "/usr/bin/mplayer"
#define MUSIC_PATH  "loadfile   /home/train/Music/"
//#define MPLAYER_PATH    "/bin/mplayer"
//#define MUSIC_PATH  "loadfile   /mnt/mplayer/"


Widget::Widget(QWidget *parent) :
    QWidget(parent)
{

   mplayer_process =   new QProcess(this);
   connect(mplayer_process,SIGNAL(readyReadStandardOutput()),this,SLOT(musicmessage()));


   // connect(mplayer_process,SIGNAL(finished(int,QProcess::ExitStatus)),
    //        this,SLOT(mplayerEnded(int,QProcess::ExitStatus)));

    m_timer =   new QTimer(this);
    m_isPlaying = false;

    //设置窗口基本属性
    setWindowTitle(tr("MyPlayer音乐播放器"));
    setWindowIcon(QIcon(":/images/icon.ico.png"));//从资源文件中招图标
    setMinimumSize(320, 440);
    setMaximumSize(320, 440);//最大最小设置为一样，代表不改变播放器窗口的大小

    //创建顶部标签
    top_label = new QLabel(tr("<a href=\"http://www.cnblogs.com/tornadomeet/\">http://www.cnblogs.com/tornadomeet/</a>"));
    top_label->setTextFormat(Qt::RichText);
    top_label->setOpenExternalLinks(true);//运行点击进入外部链接
    top_label->setAlignment(Qt::AlignCenter);

    //创建控制播放进度的滑块
    m_slider    =   new QSlider(Qt::Horizontal,this);

    //设置显示时间的标签
    QToolBar *widget_bar = new QToolBar(this);
    time_label = new QLabel(tr("00:00/00:00"), this);
    time_label->setToolTip(tr("当前时间/总时间"));
    time_label->setAlignment(Qt::AlignCenter);
    //QSizePolicy类是描述水平和垂直修改大小策略的一种属性
    time_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);//水平方向上尺寸可扩展，水平方向已固定

    //播放列表开启控制图标
    QAction *PLAction = new QAction(tr("PL"), this);
    PLAction->setShortcut(QKeySequence("F4"));//设置开启播放列表的快捷键为F4
    PLAction->setToolTip(tr("播放列表(F4)"));
    connect(PLAction, SIGNAL(triggered()), this, SLOT(SetPlayListShown()));//链接触发信号

    //桌面歌词显示开启控制图标
    QAction *LRCAction = new QAction(tr("LRC"), this);
    LRCAction->setShortcut(QKeySequence("F2"));//设置开启桌面歌词的播放列表快捷键为F2
    LRCAction->setToolTip(tr("桌面歌词(F2)"));
    connect(LRCAction, SIGNAL(triggered()), this, SLOT(SetLrcShown()));

    //将上面2个action和1个widget添加到工具栏，默认的添加方式为水平方向添加
    widget_bar->addAction(PLAction);
    widget_bar->addSeparator();
    widget_bar->addWidget(time_label);
    widget_bar->addSeparator();
    widget_bar->addAction(LRCAction);
    widget_bar->addSeparator();

    //设置播放动作
    QToolBar *tool_bar = new QToolBar(this);//该构造函数没有写入文字
    play_action = new QAction(this);
    play_action->setIcon(QIcon(":/images/play.png"));
    play_action->setText(tr("播放(F5)"));
    play_action->setShortcut(QKeySequence("F5"));//播放的快捷键位F5
    connect(play_action, SIGNAL(triggered()), this, SLOT(playButton_clicked()));

    //设置上一首动作
    skip_backward_action = new QAction(this);
    skip_backward_action->setIcon(QIcon(":/images/previous.png"));
    skip_backward_action->setText(tr("上一首(Ctrl+Left)"));
    skip_backward_action->setShortcut(QKeySequence("Ctrl+Left"));
    connect(skip_backward_action, SIGNAL(triggered()), this, SLOT(SkipBackward()));

    //设置下一首动作
    skip_forward_action = new QAction(this);
    skip_forward_action->setIcon(QIcon(":/images/next.png"));
    skip_forward_action->setText(tr("上一首(Ctrl+Right)"));
    skip_forward_action->setShortcut(QKeySequence("Ctrl+Right"));
    connect(skip_forward_action, SIGNAL(triggered()), this, SLOT(SkipForward()));

    //设置打开文件动作
    QAction *open_action = new QAction(this);
    open_action->setIcon(QIcon(":/images/gtk-open.png"));
    open_action->setText(tr("播放文件(Ctrl+O)"));
    open_action->setShortcut(QKeySequence("Ctrl+O"));
    open_action->setEnabled(true);
    connect(open_action, SIGNAL(triggered()), this, SLOT(openButton_clicked()));

    //静音图标
    sound_action    =   new QAction(this);
    sound_action->setIcon(QIcon(":/images/sound.png"));
    sound_action->setEnabled(true);
    connect(sound_action, SIGNAL(triggered()), this, SLOT(sound_off()));
    //音乐控制部件
    current_volume  =   50;
    volume_slider    =   new QSlider(Qt::Horizontal,this);
    volume_slider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    volume_slider->setMaximum(static_cast<int>(100));
    volume_slider->setMinimum(0);
    volume_slider->setSingleStep(3);
    volume_slider->setValue(50);


    //connect(volume_slider,SIGNAL(valueChanged(int)),this,SLOT(set_volume(int)));

    //将以上部件添加到工具栏
    tool_bar->addAction(play_action);
    tool_bar->addSeparator();
    tool_bar->addAction(skip_backward_action);
    tool_bar->addSeparator();
    tool_bar->addAction(skip_forward_action);
    tool_bar->addSeparator();
    tool_bar->addAction(sound_action);
    tool_bar->addSeparator();
    tool_bar->addWidget(volume_slider);
    tool_bar->addSeparator();
    tool_bar->addAction(open_action);

    //创建主界面管理器
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(top_label);
    main_layout->addWidget(m_slider);
    main_layout->addWidget(widget_bar);
    main_layout->addWidget(tool_bar);
    setLayout(main_layout);

    exit_action = new QAction(this);
    exit_action->setIcon(QIcon(":/images/exit.png"));
    exit_action->setText(tr("退出"));
    exit_action->setEnabled(true);
    connect(exit_action, SIGNAL(triggered()), this, SLOT(Setexit()));
    // 创建系统托盘图标
   tray_icon = new QSystemTrayIcon(QIcon(":/images/VMP2.png"), this);
   tray_icon->setToolTip(tr("简易音乐播放器"));
   // 创建菜单,系统托盘图标后右击出现的菜单
   QMenu *  menu = new QMenu;
   QList<QAction *> actions;
   actions << play_action  << skip_backward_action << skip_forward_action;
   menu->addActions(actions);
   menu->addSeparator();
   menu->addAction(PLAction);
   menu->addAction(LRCAction);
   menu->addSeparator();
   menu->addAction(exit_action);
   tray_icon->setContextMenu(menu);
   // 托盘图标被激活后进行处理
   connect(tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
           this, SLOT(TrayIconActivated(QSystemTrayIcon::ActivationReason)));

   // 显示托盘图标
   tray_icon->show();

   playlist = new MyPlaylist(this);
   //cellClicked()信号是当表格中的一个cell单元被单击时发出的。
   connect(playlist, SIGNAL(cellClicked(int,int)), this, SLOT(TableClicked(int)));
   connect(playlist, SIGNAL(play_list_clean()), this, SLOT(ClearSources()));
   connect(m_timer,SIGNAL(timeout()),this,SLOT(getTimepos()));
 //  m_timer->start(2000);
}

void    Widget::statechanged()
{

}

//系统托盘图标被激活
void Widget::TrayIconActivated(QSystemTrayIcon::ActivationReason activation_reason)
{
    if(activation_reason == QSystemTrayIcon::Trigger)
        show();
}

void    Widget::Setexit()
{
    this->hide();
    this->close();
}

void    Widget::SkipBackward()
{
    int index = nameList.indexOf(current_filename);
    startMPlayer(0,index-1);
    qDebug()<<index;
}

void    Widget::SkipForward()
{
    int index = nameList.indexOf(current_filename);
    startMPlayer(0,index+1);
    qDebug()<<index;
}

void    Widget::SetLrcShown()
{

}

void    Widget::SetPlayListShown()
{
    if(playlist->isHidden()) {
        //playlist->move(frameGeometry().bottomLeft());//显示在主界面的下方
        playlist->setGeometry(330,10,400,400);
        playlist->show();
    }
    else {
        playlist->hide();
    }
}

void    Widget::playButton_clicked()
{
    if(m_isPlaying)
    {
        pauseMPlayer();
    }
    else
    {
        if (nameList.isEmpty())
        {
            openButton_clicked();
        }
        else
        {
             pauseMPlayer();
        }
    }
}

bool Widget::pauseMPlayer()
{
    if(!m_isPlaying)
        return true;
    mplayer_process->write("pause\n");
    m_isPause = !m_isPause;
    if (m_isPause) {
        play_action->setIcon(QIcon(":/images/play.png"));
        m_timer->stop();
        m_slider->setEnabled(false);
    } else {
        play_action->setIcon(QIcon(":/images/pause.png"));
        m_timer->start(1000);
        m_slider->setEnabled(true);
    }

    return true;
}
bool    Widget::startMPlayer(int pos,int    row)
{
   QString string  =   file_name.at(row);
    mplayer_process->write(QString( MUSIC_PATH + string.replace("\ ","\\ ")  +   "\n").toAscii());
   if (pos > 0) {
         mplayer_process->write(QString("seek " + QString::number(pos) + " 2\n").toUtf8());
    }
    mplayer_process->write("get_time_length\n");
    m_isPlaying = true;
    m_isPause = false;
    m_timer->start(1000);
    m_slider->setEnabled(true);
    play_action->setIcon(QIcon(":/images/pause.png"));
    return true;
}

//void    Widget::stopButton_clicked()
//{
 //   stopMPlayer();
//}

void Widget::catchOutput()
{


}

bool    Widget::stopMPlayer()
{
    if(!m_isPlaying)
        return true;
    mplayer_process->write("quit\n");
    m_timer->stop();
    if(!mplayer_process->waitForFinished(3000)) {
        return false;
    }
    m_slider->setValue(static_cast<int>(0));
    return true;
}

void    Widget::getTimepos()
{
   mplayer_process->write("get_time_pos\n");
   mplayer_process->write("get_file_name\n");
   set_volume();
}

void Widget::openButton_clicked()
{
    //可以同时打开多个音频文件
    QStringList list = QFileDialog::getOpenFileNames(this, tr("打开音乐文件"),
                                                     QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
    if(list.isEmpty())
        return;
    //获取当前媒体源列表的大小
    QStringList args;//开机默认音量为50%，阶跃量为3%；
    args<<"-af";
    args<< "volume=0";
    args<<"-softvol";
    args<<"-softvol-max";
    args<<"200";
    args << "-slave";
    args << "-quiet";
    args << "/home/train/Music/111.mp3";


    //args<<"/mnt/mplayer/111.mp3";
    mplayer_process->setProcessChannelMode(QProcess::MergedChannels);
    mplayer_process->start(MPLAYER_PATH,args);
    if(!mplayer_process->waitForStarted(3000))
    {
        qDebug()<<"startMplayer    failed";
    }
    foreach(QString string  ,  list)
    {  
            qDebug()<<string;
            get_music_message(string);
    }
    mplayer_process->write("pause\n");
    sleep(5000);
    m_isPlaying =   true;
    m_isPause   =   false;
    makelist();

}


void    Widget::get_music_message(QString   string)
{
    mplayer_process->write(QString("loadfile  "   +   string.replace("\ ","\\ ")  +   "\n").toAscii());

    mplayer_process->write("get_time_length\n");
    mplayer_process->write("get_meta_artist\n");
    mplayer_process->write("get_file_name\n");
}
void    Widget::makelist()
{
    int size    =   artistList.size();
    qDebug()<<"listsize="<<size;
    for (int i = 0; i < artistList.size(); ++i)
    {
            //qDebug()<<artistList.at(i);
            QTableWidgetItem *artist_item = new QTableWidgetItem(artistList.at(i));
            artist_item->setFlags(artist_item->flags() ^ Qt::ItemIsEditable);

            QTableWidgetItem *title_item = new QTableWidgetItem(nameList.at(i));
            title_item->setFlags(title_item->flags() ^ Qt::ItemIsEditable);

            //获取总时间信息
            QString    totaltime    =   timeList.at(i);
            //qDebug()<<totaltime;

            float totaltime1  =   totaltime.toFloat();
            int totaltime2  =   (int)(totaltime1);
            //qDebug()<<totaltime1;
            QTime time(0, totaltime2/60, totaltime2%60);
            QTableWidgetItem *time_item = new QTableWidgetItem(time.toString("mm:ss"));
            //插入播放列表
            int current_rows = playlist->rowCount();//返回列表中的行数
            playlist->insertRow(current_rows);
            playlist->setItem(current_rows, 0, title_item);
            playlist->setItem(current_rows, 1, artist_item);
            playlist->setItem(current_rows, 2, time_item);
    }

}

void    Widget::musicmessage()
{
    while(mplayer_process->canReadLine()) {
        QByteArray buffer(mplayer_process->readLine());
        // get_time_length : ANS_LENGTH=xx.yy
        if(buffer.startsWith("ANS_LENGTH"))
        {
            buffer.remove(0, 11); // vire ANS_LENGTH=
            buffer.replace(QByteArray("'"), QByteArray(""));
            buffer.replace(QByteArray(" "), QByteArray(""));
            buffer.replace(QByteArray("\n"), QByteArray(""));
            buffer.replace(QByteArray("\r"), QByteArray(""));
            QString timeStr;
            timeStr.prepend(buffer);
            qDebug()<<"time is"<<timeStr;
            timeList<<timeStr;

            float maxTime = buffer.toFloat();
            max_time    =  (int)( maxTime);
            m_slider->setMaximum(static_cast<int>(maxTime+1));
        }
        // get_time_pos : ANS_TIME_POSITION=xx.y
        else if(buffer.startsWith("ANS_FILENAME"))
        {
            buffer.remove(0, 13); // vire ANS_TIME_POSITION=
            buffer.replace(QByteArray("'"), QByteArray(""));
            QString str;
            str.prepend(buffer);
            file_name<<str;
            buffer.replace(QByteArray(" "), QByteArray(""));
            buffer.replace(QByteArray("\n"), QByteArray(""));
            buffer.replace(QByteArray("\r"), QByteArray(""));
            QString nameStr;

            nameStr.prepend(buffer);
            if(nameStr  ==  "")
            {
                nameStr =   "unknown    name";
            }
            nameList<<nameStr;
            //qDebug()<<"name is "<<nameStr;

            current_filename    =   nameStr;
            //qDebug()<<"current_file="<<current_filename;

        }
        else if(buffer.startsWith("ANS_META_ARTIST"))
        {
            buffer.remove(0, 16); // vire ANS_TIME_POSITION=
            buffer.replace(QByteArray("'"), QByteArray(""));
            buffer.replace(QByteArray(" "), QByteArray(""));
            buffer.replace(QByteArray("\n"), QByteArray(""));
            buffer.replace(QByteArray("\r"), QByteArray(""));
            QString artistStr;
            artistStr.prepend(buffer);
            if(artistStr    ==  "")
            {
                artistStr   =   "unkown artist";
            }
            artistList<<artistStr;
           // qDebug()<<"artist   is:"<<artistStr;
        }
        else if(buffer.startsWith("ANS_TIME_POSITION"))
        {
            buffer.remove(0, 18); // vire ANS_TIME_POSITION=
            buffer.replace(QByteArray("'"), QByteArray(""));
            buffer.replace(QByteArray(" "), QByteArray(""));
            buffer.replace(QByteArray("\n"), QByteArray(""));
            buffer.replace(QByteArray("\r"), QByteArray(""));
            float currTime = buffer.toFloat();
            m_slider->setValue(static_cast<int>(currTime+1));

            int time    =   (int)(currTime);
            QTime     current_time(0, time/60, time%60);//传进来的time参数代表了当前的时间
            QTime  total_time(0,max_time/60,max_time%60);
            QString str = current_time.toString("mm:ss") + "/" + total_time.toString("mm:ss");
            //QString str = total_time.toString("mm:ss");
            time_label->setText(str);
        }

    }
}

void Widget::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void    Widget::ClearSources()
{

}

void    Widget::TableClicked(int    row )
{
    mplayer_process->write("pause\n");      //停止当前播放的歌曲
    qDebug()<<"cliched  is  :   "<<row;
     //清楚播放队列
    //如果单就的播放列表行号比媒体源中列表行号还打，则直接返回
    if(row >= artistList.size())
          return;
    if(!QString::compare(nameList.at(row),current_filename,Qt::CaseInsensitive))
            mplayer_process->write("pause\n");      //停止当前播放的歌曲
     else
            startMPlayer(0,row);
}

void    Widget::sound_off()
{
    if(m_issound)
    {
        mplayer_process->write("mute 1\n");
        m_issound   =   false;
    }
    else
    {
        mplayer_process->write("mute 0\n");
        m_issound   =   true;
    }
}

void    Widget::set_volume()
{
    int pos =   volume_slider->value();
    //qDebug()<<"setvolume="<<pos;
    if(pos  ==  current_volume)
        return;
    int offset  =   (pos -   current_volume) /   3;
    if( offset  >   0)
    {
       //qDebug()<<"offset="<<offset;
        while(offset--)
        {
            mplayer_process->write("volume  +9\n");
        }
    }
    else
    {
        //qDebug()<<"offset="<<-offset;
       offset   =   -offset;
        while(offset--  )
       {
           mplayer_process->write("volume  +0\n");
        }
    }
    current_volume  =   pos;
}

void    Widget::paintEvent(QPaintEvent *)
{
    QPainter    painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);
    painter.save();
    painter.setPen(Qt::NoPen);
    QLinearGradient BgGradient(QPoint(0,0),QPoint(0,height()));
    BgGradient.setColorAt(0.0,Qt::red);
    BgGradient.setColorAt(0.3,Qt::yellow);
    BgGradient.setColorAt(0.6,Qt::green);
    BgGradient.setColorAt(1.0,Qt::blue);
    painter.setBrush(BgGradient);
    QRectF   ThisRect(0,0,320,440);
    painter.drawRoundedRect(ThisRect,14,14);
}

Widget::~Widget()
{

}
