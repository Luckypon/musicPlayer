#include "interface.h"


void Window::openMessageBoxWarning(string s){
    QMessageBox::warning(this, "Warning", QString::fromStdString(s));
}

bool Window::selectMusic(int action){
    QVariant elementSelectionne;
    QModelIndex indexElementSelectionne, newIndex;
    std::vector<Music>::iterator musicPlaying;
    QItemSelectionModel *selection = listPlaylistView->selectionModel();

    if(selection->hasSelection()){
        indexElementSelectionne = selection->currentIndex();
        elementSelectionne = listPlaylistModel->data(indexElementSelectionne, Qt::DisplayRole);

        if(0 == action){
            musicPlaying = myPlaylist->find(elementSelectionne.toString().toStdString());
        }else{
            if (-1 == action){
                if(indexElementSelectionne.row() != 0){
                    musicPlaying = myPlaylist->findPrevious(elementSelectionne.toString().toStdString());
                    listPlaylistView->selectionModel()->select(indexElementSelectionne, QItemSelectionModel::Deselect);
                    newIndex = listPlaylistModel->index(indexElementSelectionne.row() - 1, 0);
                }else{
                    return false;
                }
            }else if(1 == action){
                musicPlaying = myPlaylist->findNext(elementSelectionne.toString().toStdString());
                if(myPlaylist->getIsSelected() != musicPlaying){
                    listPlaylistView->selectionModel()->select(indexElementSelectionne, QItemSelectionModel::Deselect);
                    newIndex = listPlaylistModel->index(indexElementSelectionne.row() + 1, 0);
                }else{
                    return false;
                }
            }else if(-2 == action){
                if(indexElementSelectionne.row() != 0){
                    musicPlaying = myPlaylist->findFirst();
                    listPlaylistView->selectionModel()->select(indexElementSelectionne, QItemSelectionModel::Deselect);
                    newIndex = listPlaylistModel->index(0, 0);
                }else{
                    return false;
                }
            }else if(2 == action){
                musicPlaying = myPlaylist->findLast();
                if(myPlaylist->getIsSelected() != musicPlaying){
                    listPlaylistView->selectionModel()->select(indexElementSelectionne, QItemSelectionModel::Deselect);
                    newIndex = listPlaylistModel->index(listPlaylistModel->rowCount() - 1, 0);
                }else{
                    return false;
                }
            }else{
                return false;
            }
            listPlaylistView->selectionModel()->setCurrentIndex(newIndex, QItemSelectionModel::Select);
        }
        setIsSelectedInWindow(musicPlaying);
        return true;
    }else{
        openMessageBoxWarning("Please select a music");
        return false;
    }
}

void Window::setIsSelectedInWindow(const std::vector<Music>::iterator &musicPlaying){
    if(musicPlaying != myPlaylist->getPlaylist().end()){
        if(myPlaylist->getIsSelected() != musicPlaying){
            player->setMedia(QUrl::fromLocalFile(QString::fromStdString(musicPlaying->getAdress())));
            titleLabel->setText(QString::fromStdString(musicPlaying->getTitle())),
            myPlaylist->setIsSelected(musicPlaying);
        }
    }
}


void Window::playMusic(){
    switch(player->state()){
        case 0 :
            playBtn->setIcon(QIcon(":/img/media_pause.png"));
            playBtn->setToolTip("Pause the music");
            player->play();
            break;
        case 1 :
            playBtn->setIcon(QIcon(":/img/media_play.png"));
            playBtn->setToolTip("Play the music");
            player->pause();
            break;
        case  2 :
            playBtn->setIcon(QIcon(":/img/media_pause.png"));
            playBtn->setToolTip("Pause the music");
            player->play();
            break;
        default:
            openMessageBoxWarning("There is a problem with the player");
            break;
    }

}

void Window::playSelectedMusic(){
    if(selectMusic(0)){
        playMusic();
    }
}

void Window::playPreviousMusic(){
    if(myPlaylist->getRandom()){
        playIfRandom();
    }else{
        if(selectMusic(-1)){
            playMusic();
        }
    }
}

void Window::playNextMusic(){
    if(myPlaylist->getRandom()){
        playIfRandom();
    }else{
        if(selectMusic(1)){
            playMusic();
        }
    }
}

void Window::playFirstMusic(){
    if(selectMusic(-2)){
        playMusic();
    }
}

void Window::playLastMusic(){
    if(selectMusic(2)){
        playMusic();
    }
}

void Window::playThisMusic(int nb){
    QVariant elementSelectionne;
    QModelIndex indexElementSelectionne;
    QModelIndex newIndex;

    std::vector<Music>::iterator musicPlaying;
    QItemSelectionModel *selection = listPlaylistView->selectionModel();

    if(selection->hasSelection()){
        indexElementSelectionne = selection->currentIndex();
        elementSelectionne = listPlaylistModel->data(indexElementSelectionne, Qt::DisplayRole);
        musicPlaying = myPlaylist->findThisMusic(nb);
        listPlaylistView->selectionModel()->select(indexElementSelectionne, QItemSelectionModel::Deselect);
        newIndex = listPlaylistModel->index(nb, 0);
        listPlaylistView->selectionModel()->setCurrentIndex(newIndex, QItemSelectionModel::Select);
        setIsSelectedInWindow(musicPlaying);
        playMusic();
    }else{
        openMessageBoxWarning("Please select a music");
    }
}

void Window::changeVolume(int value){
    player->setVolume(value);
}

void Window::durationChanged(qint64 duration){
    this->duration = duration/1000;
    timeSlider->setMaximum(duration / 1000);
}

void Window::positionChanged(qint64 progress){
    if (!timeSlider->isSliderDown()) {
        timeSlider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}

void Window::updateDurationInfo(qint64 currentInfo){
    QString tStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    currentTimeLabel->setText(tStr);
}

void Window::seek(int seconds){
    player->setPosition(seconds * 1000);
}

void Window::playIfRandom(){
    int nbMusic = myPlaylist->getSize();
    int randomMusic = rand() % nbMusic;
    if(randomMusic < nbMusic){
        if(listPlaylistView->selectionModel()->currentIndex().row() != randomMusic){
            playThisMusic(randomMusic);
        }else{
            if(randomMusic == 0){
                playLastMusic();
            }else{
                playFirstMusic();
            }
        }
    }
}

void Window::changeMusic(qint64 progress){
    srand (time(NULL));
    if (progress || duration){
        if((progress / 1000) >= duration){
            if(myPlaylist->getRepeat()){
                player->stop();
                player->play();
            }else if(myPlaylist->getRandom()){
                playIfRandom();
            }else{
                playNextMusic();
            }

        }
    }
}

void Window::setRepeatBtn(){
    if(myPlaylist->getRepeat() == true){
        myPlaylist->setRepeat(false);
        repeatBtn->setAutoFillBackground(true);
        repeatBtn->setStyleSheet("background-color : #e1e1e1; border:1px solid #adadad; padding: 2px 2.8px;");
    }else{
        myPlaylist->setRepeat(true);
        repeatBtn->setAutoFillBackground(true);
        repeatBtn->setStyleSheet("background-color : #909090; border:1px solid #adadad; padding: 2px 2.8px;");
    }
}

void Window::setRandomBtn(){
    if(myPlaylist->getRandom() == true){
        myPlaylist->setRandom(false);
        randomBtn->setAutoFillBackground(true);
        randomBtn->setStyleSheet("background-color : #e1e1e1; border:1px solid #adadad; padding: 2px 2.8px;");
     }else{
        myPlaylist->setRandom(true);
        randomBtn->setAutoFillBackground(true);
        randomBtn->setStyleSheet("background-color : #909090; border:1px solid #adadad; padding: 2px 2.8px;");
    }
}

void Window::loadPlaylist(){
    bool ok = false;
    QDir outputDir = QDir::current();
    outputDir.cd("../music_player_Marion_VILA/playlist");
    QString path = outputDir.path();
    QString fileName = QInputDialog::getText(this, "Playlist file", "Please enter the full name of your playlist file, with its filename extension. <br>"
                                                                    "It must be located in the playlist folder : <br> " + path +
                                                                    "<br> Please be aware that this program only read .pls or .m3u <br>"
                                                                "exemple : myPlaylist.pls", QLineEdit::Normal, QString(), &ok);
    if (ok && !path.isEmpty()){
        QString fileOut = path+ "/" +fileName;
        if(outputDir.exists(fileName)){
            tmpPlaylist = myPlaylist;
            myPlaylist->eraseAllMusic();
            if(!(myPlaylist->readFile(fileOut.toStdString()))){
                openMessageBoxWarning("The playlist wasn't well read");
                myPlaylist = tmpPlaylist;
            }else{
                beginPlaylist();
            }
        }else{
            openMessageBoxWarning("This file doesn't exist");
        }

    }
}

void Window::fillListPlaylistTitle(){
    listPlaylistTitle->clear();
    for(unsigned int i = 0; i < myPlaylist->getSize(); i++){
        *listPlaylistTitle << QString::fromStdString(myPlaylist->getMusic(i).getTitle());
    }
    listPlaylistModel->setStringList(*listPlaylistTitle);
}

void Window::stopAll(){
    myPlaylist->eraseAllMusic();
    listPlaylistTitle->clear();
    listPlaylistModel->setStringList(*listPlaylistTitle);
    player->setMedia(QMediaContent());
    timeSlider->setValue(0);
    titleLabel->setText("Please select a music and press play");
    currentTimeLabel->setText("Length :");
}

void Window::beginPlaylist(){
    fillListPlaylistTitle();
    listPlaylistView->selectionModel()->setCurrentIndex(listPlaylistModel->index(0, 0), QItemSelectionModel::Select);
    playSelectedMusic();
    myPlaylist->setIsSelected(myPlaylist->findFirst());
    player->setMedia(QUrl::fromLocalFile(QString::fromStdString(myPlaylist->getIsSelected()->getAdress())));
    titleLabel->setText(QString::fromStdString(myPlaylist->getIsSelected()->getTitle()));
    playBtn->setIcon(QIcon(":/img/media_pause.png"));
    playBtn->setToolTip("Pause the music");
    player->play();
}

void Window::addMusicInPlayer(){
    bool ok = false;
    QDir outputDir = QDir::current();
    outputDir.cd("../music_player_Marion_VILA/music");
    QString path = outputDir.path();
    QString fileName = QInputDialog::getText(this, "Playlist file", "Please enter the full name of the music you want to load with its extension<br>"
                                                                    "It must be located in the music folder : <br> " + path +
                                                                "<br> exemple : QUEEN - We will rock you.mp3", QLineEdit::Normal, QString(), &ok);
    if (ok && !path.isEmpty()){
        QString fileOut = path+ "/" +fileName;
        if(outputDir.exists(fileName)){
            tmpPlaylist = myPlaylist;
            if(!(myPlaylist->addMusic(fileOut.toStdString(), fileName.toStdString(), 1))){
                openMessageBoxWarning("The file wasn't well read");
                myPlaylist = tmpPlaylist;
            }else{
                if((int)myPlaylist->getSize() == 1){
                    beginPlaylist();
                }else{
                    fillListPlaylistTitle();
                    listPlaylistView->selectionModel()->setCurrentIndex(listPlaylistModel->index(myPlaylist->getSize() - 1, 0), QItemSelectionModel::Select);
                    playSelectedMusic();
                }
            }
        }else{
            openMessageBoxWarning("This file doesn't exist");
        }

    }
}

void Window::deleteMusicInPlayer(){
    QVariant elementSelectionne;
    QModelIndex indexElementSelectionne;
    QModelIndex newIndex;

    std::vector<Music>::iterator musicPlaying;
    QItemSelectionModel *selection = listPlaylistView->selectionModel();

    if(selection->hasSelection()){
        indexElementSelectionne = selection->currentIndex();
        elementSelectionne = listPlaylistModel->data(indexElementSelectionne, Qt::DisplayRole);
        if(indexElementSelectionne.row() > 0){
            newIndex = listPlaylistModel->index(indexElementSelectionne.row() - 1, 0);
        }else{
            musicPlaying = myPlaylist->findNext(elementSelectionne.toString().toStdString());
            if(myPlaylist->getIsSelected() != musicPlaying){
                newIndex = listPlaylistModel->index(0, 0);
            }else{
                listPlaylistView->selectionModel()->select(indexElementSelectionne, QItemSelectionModel::Deselect);
                stopAll();
                return;
            }
        }
        listPlaylistView->selectionModel()->select(indexElementSelectionne, QItemSelectionModel::Deselect);
        myPlaylist->eraseSelected();

        fillListPlaylistTitle();
        listPlaylistView->selectionModel()->setCurrentIndex(newIndex, QItemSelectionModel::Select);
        if(newIndex.row() == indexElementSelectionne.row()){
            myPlaylist->setIsSelected(myPlaylist->getIsSelected() + 1);
        }
        playSelectedMusic();
    }else{
        openMessageBoxWarning("Please select a music");
    }



}

void Window::savePlaylist(){
    bool ok = false;
    QDir outputDir = QDir::current();
    outputDir.cd("../music_player_Marion_VILA/playlist");
    QString path = outputDir.path();
    QString fileName = QInputDialog::getText(this, "Playlist file", "Please enter the name you want to pick for your playlist<br>"
                                                                    "It will be a .m3u located in the playlist folder : <br> " + path +
                                                                "<br> exemple : newPlaylist", QLineEdit::Normal, QString(), &ok);
     if (ok && !path.isEmpty()){
        if(myPlaylist->printInFile(path.toStdString(), fileName.toStdString())){
            QMessageBox::information(this, "Playlist file", "The playlist was saved.");
        }else{
            openMessageBoxWarning("The playlist wasn't saved");
        }

     }else{
            openMessageBoxWarning("Error when saving the playlist");
     }

}

void Window::createConnectors(){
    QObject::connect(playBtn, SIGNAL(clicked()), this, SLOT(playSelectedMusic()));
    QObject::connect(previousBtn, SIGNAL(clicked()), this, SLOT(playPreviousMusic()));
    QObject::connect(nextBtn, SIGNAL(clicked()), this, SLOT(playNextMusic()));
    QObject::connect(firstBtn, SIGNAL(clicked()), this, SLOT(playFirstMusic()));
    QObject::connect(lastBtn, SIGNAL(clicked()), this, SLOT(playLastMusic()));

    QObject::connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeVolume(int)));

    QObject::connect(repeatBtn, SIGNAL(clicked()), this, SLOT(setRepeatBtn()));
    QObject::connect(randomBtn, SIGNAL(clicked()), this, SLOT(setRandomBtn()));


    QObject::connect(timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));
    QObject::connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));

    QObject::connect(player, SIGNAL(positionChanged(qint64)), SLOT(changeMusic(qint64)));

    QObject::connect(addMusicBtn, SIGNAL(clicked()), SLOT(addMusicInPlayer()));
    QObject::connect(deleteMusicBtn, SIGNAL(clicked()), SLOT(deleteMusicInPlayer()));
    QObject::connect(choosePlaylistBtn, SIGNAL(clicked()), SLOT(loadPlaylist()));
    QObject::connect(savePlaylistBtn, SIGNAL(clicked()), SLOT(savePlaylist()));

    QObject::connect(listPlaylistView, SIGNAL(doubleClicked(QModelIndex)),this,SLOT(playSelectedMusic()));

}

void Window::createLayouts(){
    leftLayout->addWidget(titleLabel,0,0,1,8);

    leftLayout->addWidget(currentTimeLabel,1,0,1,2);
    leftLayout->addWidget(timeSlider,1,2,1,6);

    leftLayout->addWidget(firstBtn,2,0,1,1);
    leftLayout->addWidget(previousBtn,2,1,1,1);
    leftLayout->addWidget(playBtn,2,2,1,1);
    leftLayout->addWidget(nextBtn,2,3,1,1);
    leftLayout->addWidget(lastBtn,2,4,1,1);
    leftLayout->addWidget(volumeLabel,2,5,1,1);
    leftLayout->addWidget(volumeSlider,2,6,1,1);
    leftLayout->addWidget(repeatBtn,2,7,1,1);
    leftLayout->addWidget(randomBtn,2,8,1,1);

    leftLayout->addWidget(addMusicBtn,3,0,1,2);
    leftLayout->addWidget(deleteMusicBtn,3,2,1,2);
    leftLayout->addWidget(choosePlaylistBtn,3,4,1,3);
    leftLayout->addWidget(savePlaylistBtn,3,7,1,2);

    rightLayout->addWidget(playlistLabel);
    rightLayout->addWidget(listPlaylistView);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setMinimumSize(500,200);
    setLayout(mainLayout);
}

void Window::createInterface(){
    playBtn->setIcon(QIcon(":/img/media_play.png"));
    playBtn->setIconSize(QSize(40,30));
    playBtn->setToolTip("Play the music");

    previousBtn->setIcon(QIcon(":/img/media_previous.png"));
    previousBtn->setIconSize(QSize(40,30));
    previousBtn->setToolTip("Play previous music");

    nextBtn->setIcon(QIcon(":/img/media_next.png"));
    nextBtn->setIconSize(QSize(40,30));
    nextBtn->setToolTip("Play the next music");

    firstBtn->setIcon(QIcon(":/img/media_first.png"));
    firstBtn->setIconSize(QSize(40,30));
    firstBtn->setToolTip("Play the first music");

    lastBtn->setIcon(QIcon(":/img/media_last.png"));
    lastBtn->setIconSize(QSize(40,30));
    lastBtn->setToolTip("Play the last music");

    randomBtn->setIcon(QIcon(":/img/media_random.png"));
    randomBtn->setIconSize(QSize(40,30));
    randomBtn->setToolTip("The next music will be randomly chosen if you press this button");

    repeatBtn->setIcon(QIcon(":/img/media_repeat.png"));
    repeatBtn->setIconSize(QSize(40,30));
    repeatBtn->setToolTip("The music will be repeated if you press this button");

    addMusicBtn->setToolTip("You can add a music to the playlist");

    deleteMusicBtn->setToolTip("You can delete the selected music");

    choosePlaylistBtn->setToolTip("You can load a playlist to replace the current one");

    savePlaylistBtn->setToolTip("You can save the current playlist");

    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);

    timeSlider->setRange(0, player->duration() / 1000);

    listPlaylistView->setModel(listPlaylistModel);
    listPlaylistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listPlaylistView->selectionModel()->setCurrentIndex(listPlaylistModel->index(0, 0), QItemSelectionModel::Select);

    createLayouts();
    createConnectors();
}


Window::Window() : QWidget() {
    setFixedSize(1000,250);

    myPlaylist = new Playlist;

    player = new QMediaPlayer;

    leftLayout = new QGridLayout;
    rightLayout = new QVBoxLayout;
    mainLayout = new QHBoxLayout;

    playBtn = new QPushButton();
    previousBtn = new QPushButton();
    nextBtn = new QPushButton();
    firstBtn = new QPushButton();
    lastBtn = new QPushButton();
    randomBtn = new QPushButton();
    repeatBtn = new QPushButton();
    addMusicBtn = new QPushButton("Add music", this);
    deleteMusicBtn = new QPushButton("Delete music", this);
    choosePlaylistBtn = new QPushButton("Load playlist (m3u or pls)", this);
    savePlaylistBtn = new QPushButton("Save playlist (save m3u)", this);

    titleLabel = new QLabel("Please select a music and press play");
    playlistLabel = new QLabel("Playlist :");
    currentTimeLabel = new QLabel("Length :");

    volumeLabel = new QLabel("Volume :");
    volumeSlider = new QSlider(Qt::Horizontal, this);
    timeSlider = new QSlider(Qt::Horizontal, this);

    listPlaylistTitle = new QStringList();
    listPlaylistModel = new QStringListModel();
    listPlaylistView = new QListView ;

    createInterface();

}
