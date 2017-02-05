#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "common.h"
#include "music.h"

class Playlist{
private:
    std::vector<Music> playlist;
    std::vector<Music>::iterator isSelected;
    bool repeat;
    bool random;

public:
    typedef std::vector<Music>::iterator iterator;
    typedef std::vector<Music>::const_iterator const_iterator;

    Playlist();

    void setRandom(bool b);
    void setRepeat(bool b);
    void setIsSelected(iterator it);

    bool getRandom() const;
    bool getRepeat() const;
    iterator getIsSelected() const;

    void addMusic(const Music &m);
    bool addMusic(std::string adress, std::string title, int length);

    std::vector<Music> getPlaylist() const;
    Music getMusic(unsigned int place) const;
    std::vector<int>::size_type getSize() const;

    iterator findThisMusic(int place);
    iterator find(std::string name);
    iterator findPrevious(std::string name);
    iterator findNext(std::string name);
    iterator findFirst();
    iterator findLast();

    void eraseAllMusic();
    void eraseSelected();

    bool readFile(std::string filePath);
    bool printInFile(std::string path, std::string fileName);

};


int skipLine(std::ifstream *file); //skip the entire line
std::string getLineAfterSymbol(std::string sLine, char symbol, int idMusic); //check the id
std::string getLineAfterSymbol(std::string sLine, char symbol);
std::string getLineBeforeSymbol(std::string sLine, char symbol);


#endif
