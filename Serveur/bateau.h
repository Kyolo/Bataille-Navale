#ifndef BATEAU_H
#define BATEAU_H

typedef unsigned char uchar;

class Bateau
{
public:
    Bateau(uchar taille, uchar posx, uchar posy, bool hori);
    Bateau();
    bool isDestroyed();
    uchar getPositionX();
    uchar getPositionY();
    bool isHorizontal();
    bool hit(uchar x, uchar y);
    uchar getPV();
    void destroy();
private:
    uchar bTaille;
    uchar bX;
    uchar bY;
    bool bHorizontal;
    bool * statut;
};

#endif // BATEAU_H
