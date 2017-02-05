#include "interface.h"


int main(int argc, char * argv[]){
    QApplication app(argc, argv);

    Window window;

    window.show();

    return app.exec();
}

/* The length isn't valid when I try to load a music alone.
 * It works without it, because my program doesn't need it,
 * So if I save this music into a new playlist file, a false length will be given. */
