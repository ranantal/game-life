#ifndef DRAWPANELWIDJET_H
#define DRAWPANELWIDJET_H

#include <QWidget>
#include <QImage>
#include <QPoint>

#include <QMouseEvent>
#include <QTimer>

#include <utility>
#include <thread>

#include "gamemodel.h"

class DrawPanelWidget : public QWidget
{
    Q_OBJECT
private:
    QImage image;
    GameModel *model;

    std::pair <int, int> lastHexCoords;

    QTimer *timer;

    void drawImpacts(QPainter & painter);
    void drawLine(QPoint point1, QPoint point2);
    void drawHex(QPoint center);
    void drawField();
    void fillHex(int x, int y, uint newColor, uint oldColor);
    void spanFill(int x, int y, uint newColor, uint oldColor);
    std::pair<int, int> calculateHexCoords(int x, int y);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;

public:
    QString actualFileName;

    explicit DrawPanelWidget(QWidget *parent = 0);
    ~DrawPanelWidget();

    void resizeImage();
    void repaintField();

    void nextStep();

    void clear();

    void runGame();
    void stopGame();

    bool checkModel();
    bool openModel(QString fileName);
    bool saveModel(QString fileName);

public slots:
    void runGameSlot();
};

#endif // DRAWPANELWIDJET_H
