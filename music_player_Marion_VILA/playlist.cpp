#include "playlist.h"

using namespace std;


Playlist::Playlist(){
    random = false;
    repeat = false;
};


void Playlist::addMusic(const Music &m){
    playlist.push_back(m);
};

bool Playlist::addMusic(string adress, string title, int length){
    Music newMusic;

    if(!(adress.empty()))
        newMusic.setAdress(adress);
    else
        return false;
    if(!(title.empty()))
        newMusic.setTitle(title);
    else
        return false;
    if(length > 0)
        newMusic.setLength(length);
    else
        return false;

    addMusic(newMusic);
    return true;

}


void Playlist::setRandom(bool b){
    random = b;
}

void Playlist::setRepeat(bool b){
    repeat = b;

}

bool Playlist::getRandom() const{
    return random;
}

bool Playlist::getRepeat() const{
    return repeat;
}

void Playlist::eraseAllMusic(){
    playlist.clear();
}

void Playlist::eraseSelected(){
    playlist.erase(getIsSelected());

}


vector<Music> Playlist::getPlaylist() const{
    return playlist;
}

vector<int>::size_type Playlist::getSize() const{
    return playlist.size();
}



Music Playlist::getMusic(unsigned int place) const{
    if(place <= getSize()){
       return playlist.at(place) ;
    }
    exit(EXIT_FAILURE);
}

Playlist::iterator Playlist::findThisMusic(int place){
   Music tmp = getMusic(place);
   tmp.setTitle(tmp.getTitle());
   Playlist::iterator it = std::find(playlist.begin(), playlist.end(), tmp);
   if ((it == playlist.end())){
        return getIsSelected();
   }else{
        return it;
   }
}


Playlist::iterator Playlist::find(std::string name){
   Music tmp;
   tmp.setTitle(name);
   return std::find(playlist.begin(), playlist.end(), tmp);
}

Playlist::iterator Playlist::findPrevious(std::string name){
    Playlist::iterator it = find(name);
    if (it == playlist.begin()){
        return getIsSelected();
    }else{
        return std::prev(it);
    }
}

Playlist::iterator next(Playlist::iterator it){
    return ++it;
}

Playlist::iterator Playlist::findNext(std::string name){
   Playlist::iterator it = find(name);
   if ((it != playlist.end()) && (next(it) == playlist.end())){
        return getIsSelected();
   }else{
        return std::next(it);
   }
}

Playlist::iterator Playlist::findFirst(){
   return playlist.begin();
}

Playlist::iterator Playlist::findLast(){
   return --(playlist.end());
}

void Playlist::setIsSelected(iterator it){
   isSelected = it;
}
Playlist::iterator Playlist::getIsSelected() const{
    return isSelected;
}

int skipLine(ifstream *file){
    string sLine = "";
    getline(*file, sLine);
    return 0;
}

string getLineAfterSymbol(string sLine, char symbol){
    unsigned int i = 0;
    while (sLine[i] != symbol){
        i++;
    }
    sLine = sLine.substr(i+1);
    return sLine;
}

string getLineBeforeSymbol(string sLine, char symbol){
    unsigned int i = 0;
    string newLine;
    while(sLine[i] != symbol){
        i++;
    }
    newLine = sLine.substr(0,i);
    return newLine;
}

string getLineAfterSymbol(string sLine, char symbol, int idMusic){
    unsigned int i = 0;
    while (sLine[i] != symbol){
        i++;
    }
    char testId[2];

    sprintf(testId,"%c",sLine[i-1]);
    if(atoi(testId) == idMusic){
        sLine = sLine.substr(i+1);
        return sLine;
    }else{
        return "";
    }
}




bool Playlist::readFile(string filePath){

    ifstream file(filePath, ios::in);


    if(file.fail()){
        return false;
    }else{
        int nbId = 1, testNbOfEntries = -5;
        bool testFillMusic = false;
        string firstLine;

        getline(file, firstLine);

        if(firstLine == "[playlist]"){ // FORMAT PLS
            while (!file.eof() && testNbOfEntries < 0){
                Music newMusic;
                newMusic.setId(nbId);
                testNbOfEntries = newMusic.fillMusicPLS(&file,nbId);
                if(testNbOfEntries == -2){
                    addMusic(newMusic);
                    nbId++;
                }
            }
            file.close();
            vector<int>::size_type sz = playlist.size();
            if(sz == (unsigned int) testNbOfEntries){
                return true;
            }
            else{
                playlist.clear();
                return false;
            }
        }else if(firstLine == "#EXTM3U"){ // FORMAT m3u
            while (!file.eof()){
                Music newMusic;
                testFillMusic = newMusic.fillMusicM3U(&file);
                if(testFillMusic){
                    addMusic(newMusic);
                }
            }
            file.close();
            if(getSize() < 1) return false;
            else return true;
        }else{
            playlist.clear();
            return false;
        }



    }
    return false;
}

bool Playlist::printInFile(string path, string fileName){
    string fullPath = path + "/" + fileName + ".m3u";
    ofstream file(fullPath, ios::out | ios::trunc);

    if(file){
        file << "#EXTM3U" << endl;
        for(unsigned int i = 0; i < getSize(); i++){
            file << "#EXTINF:" << getMusic(i).getLength() << ","
            << getMusic(i).getTitle() << endl;
            file << getMusic(i).getAdress() << endl;
        }
        file.close();
        return true;
    }else
        return false;
}












