#include "volume.h"
#include <string>

using namespace std;

Volume::Volume(string vol, Volume *top, Volume *right, Volume *bottom, Volume *left, int cor) {
    this->vol = vol;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
    this->left = left;
    this->cor = cor;
}

void Volume::setVol(string vol) {
    this->vol = vol;
}

string Volume::getVol() {
    return vol;
}

void Volume::setTop(Volume *top) {
    this->top = top;
}

Volume *Volume::getTop() {
    return top;
}

void Volume::setRight(Volume *right) {
    this->right = right;
}

Volume *Volume::getRight() {
    return right;
}

void Volume::setBottom(Volume *bottom) {
    this->bottom = bottom;
}

Volume *Volume::getBottom() {
    return bottom;
}

void Volume::setLeft(Volume *left) {
    this->left = left;
}

Volume *Volume::getLeft() {
    return left;
}

void Volume::setCor(int cor) {
    this->cor = cor;
}

int Volume::getCor() {
    return cor;
}
