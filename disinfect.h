#ifndef DISINFECT_H
#define DISINFECT_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QObject>
#include <QGraphicsItem>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>
#include <QTime>
#include <QTimer>
#include <QMovie>
#include <QMediaPlayer>

#include "corona.h"



class Disinfect: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Disinfect(int width_, int height_);

    //QImage * get_background() {return background_;};

    std::vector<Corona *> get_cells() {return cells_;};

    virtual void populate(int width, int height); //This will be virtual

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    QPixmap background_;
    std::vector<Corona *> cells_;

signals:
    void LostTheMiniGame(Disinfect * d);


//private:
//    QPixmap background_;
//    std::vector<Corona *> cells_;
};

#endif // DISINFECT_H
