#ifndef MYLIST_H
#define MYLIST_H

#include <QTableWidget>

class MyPlaylist : public QTableWidget
{
    Q_OBJECT
public:
    explicit MyPlaylist(QWidget *parent = 0);

signals:
    void play_list_clean();

public slots:

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void clear_play_list();

};

#endif // MYLIST_H
