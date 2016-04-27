#ifndef BATEAU_H
#define BATEAU_H

typedef unsigned char uchar;

class Bateau
{
public:
    Bateau(uchar taille=0, uchar posx=0, uchar posy=0, bool hori=false);
    bool isDestroyed();
    uchar getPositionX();
    uchar getPositionY();
    bool isHorizontal();
    bool hit(uchar x, uchar y);
    uchar getPV();
    void destroy();
    char getStatusAt(uchar x, uchar y);

    static const char SAFE = 'B';
    static const char NONE = ' ';
    static const char DESTROYED = '*';

private:
    uchar bTaille;
    uchar bX;
    uchar bY;
    bool bHorizontal;
    bool * statut;
};

#endif // BATEAU_H
