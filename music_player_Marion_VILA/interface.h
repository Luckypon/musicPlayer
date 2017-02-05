#ifndef INTERFACE_H
#define INTERFACE_H


#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QProgressBar>
#include <QLCDNumber>
#include <QSlider>
#include <QMediaPlayer>
#include <QTextStream>
#include <QVector>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QSlider>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QList>
#include <QStringListModel>
#include <QtWidgets>
#include <QInputDialog>


#include "common.h"
#include "playlist.h"


using namespace std;

class Window : public QWidget{
    Q_OBJECT

private:
    QPushButton *playBtn, *previousBtn, *nextBtn, *firstBtn, *lastBtn,
        *repeatBtn, *randomBtn,
        *addMusicBtn, *deleteMusicBtn, *choosePlaylistBtn, *savePlaylistBtn;
    QMediaPlayer *player;
    QLabel *titleLabel, *playlistLabel, *currentTimeLabel, *volumeLabel;
    QSlider *volumeSlider, *timeSlider;

    QGridLayout *leftLayout;
    QHBoxLayout *mainLayout;
    QVBoxLayout *playlistLayout, *rightLayout;

    QListView *listPlaylistView;
    QStringListModel *listPlaylistModel;
    QStringList *listPlaylistTitle;

    Playlist *myPlaylist;
    Playlist *tmpPlaylist;

    qint64 duration;


public:
    Window();
    Window(int _width,int _height);


public slots:
    bool selectMusic(int action);
    void playMusic();

    void playThisMusic(int nb);
    void playSelectedMusic();
    void playPreviousMusic();
    void playNextMusic();
    void playFirstMusic();
    void playLastMusic();
    void changeVolume(int value);

    void openMessageBoxWarning(std::string s);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void updateDurationInfo(qint64 currentInfo);
    void seek(int seconds);

    void setRepeatBtn();
    void setRandomBtn();

    void changeMusic(qint64 progress);

    void loadPlaylist();
    void addMusicInPlayer();
    void deleteMusicInPlayer();
    void savePlaylist();

    void stopAll();
    void beginPlaylist();

    void createLayouts();
    void createConnectors();
    void createInterface();

    void setIsSelectedInWindow(const std::vector<Music>::iterator &musicPlaying);
    void fillListPlaylistTitle();
    void playIfRandom();


};



#endif // INTERFACE_H
