#include "music.h"
#include "playlist.h"

using namespace std;


Music::Music(){
    length = -1;
};

void Music::setTitle(string s){
    title = s;
};
void Music::setLength(int n){
    length = n;
};
void Music::setAdress(string s){
    adress = s;
};
void Music::setId(int n){
    id = n;
};


string Music::getTitle() const{
    return title;
};
int Music::getLength() const{
    return length;
};
string Music::getAdress() const{
    return adress;
};
int Music::getId() const{
    return id;
};


bool Music::operator==(const Music & a){
  return title == a.title;
}


int Music::fillMusicPLS(ifstream *file, int nbId){
    string sLine = "", newData = "";
    char symbol = '=';

    for(unsigned int i = 0; i < 3; i++){
        getline(*file, sLine);
        if(sLine[0] == 'F' && getAdress().empty()){
            newData = getLineAfterSymbol(sLine,symbol,nbId);
            if(!(newData.empty()))
                setAdress(newData);
            else
                return -1;
        }
        else if(sLine[0] == 'T' && getTitle().empty()){
            newData = getLineAfterSymbol(sLine,symbol, nbId);
            if(!(newData.empty()))
                setTitle(newData);
            else
                return -1;
        }
        else if(sLine[0] == 'L' && getLength() <= 0){
            newData = getLineAfterSymbol(sLine,symbol, nbId);
            if(!(newData.empty())){
                try{
                    setLength(std::stoi(newData));
                }
                catch(std::invalid_argument&){
                    return -1;
                }
            }
            else
                return -1;
        }else if(sLine[0] == 'N'){
            newData = getLineAfterSymbol(sLine,symbol);
            if(!(newData.empty())){
                int testNumber = -1;
                try{
                    testNumber = std::stoi(newData);
                }
                catch(std::invalid_argument&){
                    return -1;
                }
                return testNumber;
            }

            else
                return -1;
        }else{
            if(getAdress().empty() || getTitle().empty() || getLength() <= 0){
                return -1;
            }
            return -2;
        }
     }

    if(getAdress().empty() || getTitle().empty() || getLength() <= 0){
        return -1;
    }
    return -2;


}

bool Music::fillMusicM3U(ifstream *file){
    string sLine = "", length = "", name = "", newData = "";
    char colonSymbol = ':', commaSymbol = ',';

    getline(*file, sLine);
    if(sLine[0] == '#'){
        newData = getLineAfterSymbol(sLine, colonSymbol);
        if(!(newData.empty())){
            length = getLineBeforeSymbol(newData, commaSymbol);
            name = getLineAfterSymbol(newData, commaSymbol);
            try{
                setLength(std::stoi(length));
            }
            catch(std::invalid_argument&){
                return false;
            }
            if(!(name.empty())){
                setTitle(name);
            }else{
                return false;
            }
        }else{
            return false;
        }
        getline(*file, sLine);
        if(!(sLine.empty())){
            setAdress(sLine);
            return true;
        }
    }
    return false;


}



