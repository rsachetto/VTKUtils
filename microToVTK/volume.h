#include <string>

using namespace std;

class Volume {
private:

    string vol;
    Volume *top;
    Volume *right;
    Volume *bottom;
    Volume *left;
    int cor;

public:

    Volume(string vol, Volume *top, Volume *right, Volume *bottom, Volume *left, int cor);

    void setVol(string vol);
    string getVol();
    void setTop(Volume *top);
    Volume *getTop();
    void setRight(Volume *right);
    Volume *getRight();
    void setBottom(Volume *bottom);
    Volume *getBottom();
    void setLeft(Volume *left);
    Volume *getLeft();
    void setCor(int cor);
    int getCor();
};
