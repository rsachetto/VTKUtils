#include "volume.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void verificarVolume(Volume *p, int cor) {
    if(p->getCor() != -1) return;
    p->setCor(cor);
    if(p->getVol().at(3) == 'c') {
        if(p->getLeft()->getCor() == -1) {
            verificarVolume(p->getLeft(), cor);
        }
    }
    if(p->getVol().at(1) == 'c') {
        if(p->getRight()->getCor() == -1) {
            verificarVolume(p->getRight(), cor);
        }
    }
    if(p->getVol().at(2) == 'c') {
        if(p->getBottom()->getCor() == -1) {
            verificarVolume(p->getBottom(), cor);
        }
    }
}

int main() {
    ifstream entrada("malha.txt");
    string linha, vol;
    Volume *inicio, *p, *anterior, *cima;
    int i, j, contX = 0, contY, numGaps = 0;
    while(getline(entrada, linha)) {
        if(contX == 0) inicio = new Volume("", NULL, NULL, NULL, NULL, -1);
        p = inicio;
        while(p->getBottom() != NULL) p = p->getBottom();
        p->setBottom(new Volume("", NULL, NULL, NULL, NULL, -1));
        cima = p;
        p = p->getBottom();
        p->setTop(cima);
        istringstream split(linha);
        contY = 0;
        while(getline(split, vol, '\t')) {
            p->setVol(vol);
            for(i = 0; i < 4; i++) {
                if(vol.at(i) == '1' || vol.at(i) == '2' || vol.at(i) == '3') numGaps++;
            }
            if(contX != 0) {
                cima->setBottom(p);
                p->setTop(cima);
                cima = cima->getRight();
            }
            p->setRight(new Volume("", NULL, NULL, NULL, NULL, -1));
            anterior = p;
            p = p->getRight();
            p->setLeft(anterior);
            contY++;
        }
        anterior->setRight(NULL);
        contX++;
    }
    numGaps /= 2;
    entrada.close();
    ofstream saida("malha.vtk");
    saida << "# vtk DataFile Version 3.0\nMalha\nASCII\nDATASET POLYDATA\nPOINTS " << 12 * contX * contY << " float\n";
    double acrescimoX = 0, acrescimoY;
    for(i = 0; i < contX; i++) {
        acrescimoY = 0;
        for(j = 0; j < contY; j++) {
            saida << j + acrescimoY << " " << i + acrescimoX << " 0\n";
            saida << j + 1 + acrescimoY << " " << i + acrescimoX << " 0\n";
            acrescimoY += 0.1;
        }
        acrescimoY = 0;
        for(j = 0; j < contY; j++) {
            saida << j + acrescimoY << " " << i + 1 + acrescimoX << " 0\n";
            saida << j + 1 + acrescimoY << " " << i + 1 + acrescimoX << " 0\n";
            acrescimoY += 0.1;
        }
        acrescimoX += 0.1;
    }
    acrescimoX = 0;
    for(i = 0; i < contX; i++) {
        acrescimoY = 0;
        for(j = 0; j < contY; j++) {
            saida << j + 0.4 + acrescimoY << " " << i + 0.2 + acrescimoX << " 0.1\n";
            saida << j + 0.6 + acrescimoY << " " << i + 0.2 + acrescimoX << " 0.1\n";
            acrescimoY += 0.1;
        }
        acrescimoX += 0.1;
    }
    acrescimoX = 0;
    for(i = 0; i < contX; i++) {
        acrescimoY = 0;
        for(j = 0; j < contY; j++) {
            saida << j + 0.8 + acrescimoY << " " << i + 0.4 + acrescimoX << " 0.1\n";
            saida << j + 0.8 + acrescimoY << " " << i + 0.6 + acrescimoX << " 0.1\n";
            acrescimoY += 0.1;
        }
        acrescimoX += 0.1;
    }
    acrescimoX = 0;
    for(i = 0; i < contX; i++) {
        acrescimoY = 0;
        for(j = 0; j < contY; j++) {
            saida << j + 0.4 + acrescimoY << " " << i + 0.8 + acrescimoX << " 0.1\n";
            saida << j + 0.6 + acrescimoY << " " << i + 0.8 + acrescimoX << " 0.1\n";
            acrescimoY += 0.1;
        }
        acrescimoX += 0.1;
    }
    acrescimoX = 0;
    for(i = 0; i < contX; i++) {
        acrescimoY = 0;
        for(j = 0; j < contY; j++) {
            saida << j + 0.2 + acrescimoY << " " << i + 0.4 + acrescimoX << " 0.1\n";
            saida << j + 0.2 + acrescimoY << " " << i + 0.6 + acrescimoX << " 0.1\n";
            acrescimoY += 0.1;
        }
        acrescimoX += 0.1;
    }
    int poligonos = contX * contY + numGaps;
    saida << "POLYGONS " << poligonos << " " << 5 * poligonos << "\n";
    vector<int> cor;
    int contCor = 0, cont = 0;
    for(i = 0; i < contX; i++) {
        for(j = 0; j < contY; j++) {
            saida << "4 " << 2 * (i + cont) * contY + 2 * j << " " << 2 * (i + cont) * contY + (2 * j + 1) << " " << 2 * (i + 1 + cont) * contY + (2 * j + 1) << " " << 2 * (i + 1 + cont) * contY + 2 * j << "\n";
        }
        cont++;
    }
    Volume *q;
    vector<int> corGap;
    int numVolX, numVolY;
    p = inicio->getBottom();
    numVolX = 0;
    while(p != NULL) {
        q = p;
        numVolY = 0;
        while(q != NULL) {
            if(q->getVol().at(1) != 'm' && q->getVol().at(1) != 'c') {
                saida << "4 " << 6 * contX * contY + 2 * numVolX * contY + 2 * numVolY << " " << 6 * contX * contY + 2 * numVolX * contY + 2 * numVolY + 1 << " " << 10 * contX * contY + 2 * numVolX * contY + 2 * numVolY + 3 << " " << 10 * contX * contY + 2 * numVolX * contY + 2 * numVolY + 2 << "\n";
                corGap.push_back(21);
            }
            q = q->getRight();
            numVolY++;
        }
        p = p->getBottom();
        numVolX++;
    }
    p = inicio->getBottom();
    numVolX = 0;
    while(p != NULL) {
        q = p;
        numVolY = 0;
        while(q != NULL) {
            if(q->getVol().at(2) != 'm' && q->getVol().at(2) != 'c') {
                saida << "4 " << 8 * contX * contY + 2 * numVolX * contY + 2 * numVolY << " " << 8 * contX * contY + 2 * numVolX * contY + (2 * numVolY + 1) << " " << 4 * contX * contY + 2 * (numVolX + 1) * contY + (2 * numVolY + 1) << " " << 4 * contX * contY + 2 * (numVolX + 1) * contY + 2 * numVolY << "\n";
                if(q->getVol().at(2) == '2') corGap.push_back(22);
                else corGap.push_back(23);
            }
            q = q->getRight();
            numVolY++;
        }
        p = p->getBottom();
        numVolX++;
    }
    srand(time(NULL));
    p = inicio->getBottom();
    while(p != NULL) {
        q = p;
        while(q != NULL) {
            verificarVolume(q, rand() % 20);
            q = q->getRight();
        }
        p = p->getBottom();
    }
    saida << "CELL_DATA " << poligonos << "\nSCALARS cell_scalars int 1\nLOOKUP_TABLE default\n";
    p = inicio->getBottom();
    while(p != NULL) {
        q = p;
        while(q != NULL) {
            saida << q->getCor() << " ";
            q = q->getRight();
        }
        p = p->getBottom();
    }
    for(i = 0; i < corGap.size(); i++) {
        saida << corGap.at(i) << " ";
    }
    saida.close();
    return 0;
}
