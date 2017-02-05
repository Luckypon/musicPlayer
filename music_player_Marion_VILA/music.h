#ifndef MUSIC_H
#define MUSIC_H

#include <string>

class Music{
private:
    std::string title;
    int length;
    std::string adress;
    int id;

public:
    Music();

    void setTitle(std::string s);
    void setLength(int n);
    void setAdress(std::string s);
    void setId(int n);

    std::string getTitle() const;
    int getLength() const;
    std::string getAdress() const;
    int getId() const;

    bool operator==(const Music & a);

    int fillMusicPLS(std::ifstream *file, int nbId); // -2 = everything is good, -1 = error, int > 0 = nbOfEntries
    bool fillMusicM3U(std::ifstream *file);

};

#endif
