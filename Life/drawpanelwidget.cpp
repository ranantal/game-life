#include "drawpanelwidget.h"
#include "settings.h"
#include "gamemodel.h"

#include <QtCore>
#include <QtGui>

#include <fstream>
#include <cmath>
#include <stack>

#include <unistd.h>

#define ALIVE_COLOR 0xff44ff44
#define DEAD_COLOR 0xd7d7d7d7
#define BORDER_COLOR 0x00000000

#define MAX(x, y) (x) > (y) ? x : y

DrawPanelWidget::DrawPanelWidget(QWidget *parent) : QWidget(parent){
    image = QImage(Settings::xSize * sqrt(3) * Settings::cellSize,
                   Settings::ySize * Settings::cellSize * 3 / 2 + Settings::cellSize,
                   QImage::Format_RGB32);

    setMinimumHeight(Settings::ySize * Settings::cellSize * 3 / 2 + Settings::cellSize);
    setMinimumWidth(Settings::xSize * sqrt(3) * Settings::cellSize);

    model = new GameModel();
    model->init(Settings::xSize, Settings::ySize);

    repaintField();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(runGameSlot()));

    actualFileName = "Untitled";

}

DrawPanelWidget::~DrawPanelWidget() {
    delete timer;
    delete model;
}

void DrawPanelWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.drawImage(0,\
                      0,\
                      image);

    if (Settings::impact) {
        model->calculateImpacts();
        drawImpacts(painter);
    }

}

void DrawPanelWidget::resizeEvent(QResizeEvent *) {

}

void DrawPanelWidget::drawLine(QPoint point1, QPoint point2) {
    int width = image.width();

    uchar* bits = image.bits();

    int x1 = point1.x();
    int x2 = point2.x();
    int y1 = point1.y();
    int y2 = point2.y();

    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;

    int error = deltaX - deltaY;

    ((uint*)bits)[y2 * width + x2] = BORDER_COLOR;

    while(x1 != x2 || y1 != y2) {
        ((uint*)bits)[y1 * width + x1] = BORDER_COLOR;

        const int error2 = error * 2;

        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}

void DrawPanelWidget::drawHex(QPoint center) {
    int size = Settings::cellSize;

    int halfWidth = sqrt(3) * size * 0.5;

    QPoint point1(center.x() + halfWidth, center.y() + size / 2);
    QPoint point2(center.x() + halfWidth, center.y() - size / 2);
    QPoint point3(center.x(), center.y() - size);
    QPoint point4(center.x() - halfWidth, center.y() - size / 2);
    QPoint point5(center.x() - halfWidth, center.y() + size / 2);
    QPoint point6(center.x(), center.y() + size);

    drawLine(point1, point2);
    drawLine(point2, point3);
    drawLine(point3, point4);
    drawLine(point4, point5);
    drawLine(point5, point6);
    drawLine(point6, point1);
}

void DrawPanelWidget::drawField() {
    int size = Settings::cellSize;

    int halfWidth = sqrt(3) * size * 0.5;
    int halfSize = size / 2;

    int xCenter = halfWidth;
    int yCenter = size;

    for (int j = 0; j < Settings::ySize; ++j) {
        for (int i = 0; i < Settings::xSize - (j % 2); ++i) {
            drawHex(QPoint(xCenter, yCenter));
            xCenter += 2 * halfWidth;
        }
        if (j % 2 == 1)
            xCenter = halfWidth;
        else
            xCenter = 2 * halfWidth;

        yCenter += size + halfSize;
    }
}

void DrawPanelWidget::fillHex(int x, int y, uint newColor, uint oldColor) {
    int size = Settings::cellSize;

    int halfWidth = sqrt(3) * size * 0.5;
    int halfSize = size / 2;

    int xCenter = halfWidth;
    int yCenter = size;

    if (y % 2 == 1)
        xCenter += halfWidth;

    xCenter += x * 2 * halfWidth;

    yCenter += y * (size + halfSize);

    spanFill(xCenter, yCenter, newColor, oldColor);
}

void DrawPanelWidget::spanFill(int x, int y, uint newColor, uint oldColor) {
    std::stack<QPoint> myStack;
    uint* bits = (uint*)image.bits();

    myStack.push(QPoint(x ,y));

    while (!myStack.empty()) {
        QPoint point = myStack.top();
        myStack.pop();

        int curX = point.x();
        int curY = point.y();

        int leftBord, rightBord;

        if (bits[curY * image.width() + curX] == oldColor)
            bits[curY * image.width() + curX] = newColor;

        else
            continue;

        ++curX;

        while (bits[curY * image.width() + curX] == oldColor && curX < image.width()) {
            bits[curY * image.width() + curX] = newColor;

            ++curX;
        }

        rightBord = curX - 1;

        curX = point.x();
        --curX;

        while (bits[curY * image.width() + curX] == oldColor && curX > -1) {
            bits[curY * image.width() + curX] = newColor;

            --curX;
        }

        leftBord = curX + 1;

        bool span = false;
        if (curY + 1 < image.height())
            for (int i = leftBord; i <= rightBord; ++i) {
                if (bits[(curY + 1) * image.width() + i] == oldColor) {
                    if (span == false) {
                        span = true;
                        myStack.push(QPoint(i, curY + 1));
                    }
                } else
                    span = false;
            }

        span = false;
        if (curY > 0)
            for (int i = leftBord; i <= rightBord; ++i) {
                if (bits[(curY - 1) * image.width() + i] == oldColor) {
                    if (span == false) {
                        span = true;
                        myStack.push(QPoint(i, curY - 1));
                    }
                } else
                    span = false;
            }
    }
}

void DrawPanelWidget::resizeImage() {
    int width = Settings::xSize * sqrt(3) * Settings::cellSize;
    int height = Settings::ySize * Settings::cellSize * 3 / 2 + Settings::cellSize;

    setMinimumHeight(height);
    setMinimumWidth(width);

    model->init(Settings::xSize, Settings::ySize);

    if (width == image.width() && height == image.height())
        return;

    image = QImage(width,\
                           height,\
                           QImage::Format_RGB32);

    repaintField();
    repaint();
}

void DrawPanelWidget::repaintField() {
    memset(image.bits(),\
               DEAD_COLOR,\
               image.byteCount());

    drawField();

    for (int y = 0; y < Settings::ySize; ++y) {
        for (int x = 0; x < Settings::xSize - y % 2; ++x) {
            if (model->getHex(y, x).state == ALIVE) {
                fillHex(x, y, ALIVE_COLOR, DEAD_COLOR);
            }
            else {
                fillHex(x, y, DEAD_COLOR, ALIVE_COLOR);
            }
        }
    }
}

void DrawPanelWidget::nextStep() {
    model->calculateImpacts();
    model->nextStep();
    repaintField();
    repaint();
}

void DrawPanelWidget::mouseReleaseEvent(QMouseEvent* event) {
    lastHexCoords.first = -1;
}

void DrawPanelWidget::mousePressEvent(QMouseEvent* event) {
    int hexX;
    int hexY;

    std::pair<int, int> coords;

    try {
        coords = calculateHexCoords(event->x(), event->y());
    } catch (uint color) {
        return;
    }

    lastHexCoords = coords;

    hexX = coords.first;
    hexY = coords.second;

    if (hexY > -1 && hexY < Settings::ySize && hexX > -1 && (hexX - hexY % 2 < Settings::xSize)) {
        if (Settings::replaceMode) {
            model->setHexState(hexY, hexX, ALIVE);
            fillHex(hexX, hexY, ALIVE_COLOR, DEAD_COLOR);
        }

        else if (Settings::xorMode) {
            State previousState = model->getHex(hexY, hexX).state;
            model->setHexState(hexY, hexX, previousState == ALIVE ? DEAD : ALIVE);
            fillHex(
                        hexX,
                        hexY,
                        previousState == ALIVE ? DEAD_COLOR : ALIVE_COLOR,
                        previousState == ALIVE ? ALIVE_COLOR : DEAD_COLOR
                                                 );
        }
        repaint();
    }

}

void DrawPanelWidget::mouseMoveEvent(QMouseEvent* event) {
    int hexX;
    int hexY;

    std::pair<int, int> coords;

    try {
        coords = calculateHexCoords(event->x(), event->y());
    } catch (uint color) {
        return;
    }

    if (coords == lastHexCoords)
        return;

    hexX = coords.first;
    hexY = coords.second;

    lastHexCoords = coords;

    if (hexY > -1 && hexY < Settings::ySize && hexX > -1 && (hexX < Settings::xSize - hexY % 2)) {
        if (Settings::replaceMode) {
            model->setHexState(hexY, hexX, ALIVE);
            fillHex(hexX, hexY, ALIVE_COLOR, DEAD_COLOR);
        }

        else if (Settings::xorMode) {
            State previousState = model->getHex(hexY, hexX).state;
            model->setHexState(hexY, hexX, previousState == ALIVE ? DEAD : ALIVE);
            fillHex(
                        hexX,
                        hexY,
                        previousState == ALIVE ? DEAD_COLOR : ALIVE_COLOR,
                        previousState == ALIVE ? ALIVE_COLOR : DEAD_COLOR
                                                 );
        }
        repaint();
    }
}

std::pair<int, int> DrawPanelWidget::calculateHexCoords(int x, int y) {
    if (x > image.width() || y > image.height() || x < 0 || y < 0) {
        throw (uint)DEAD_COLOR;
    }

    uint* bits = (uint *)image.bits();

    if (bits[y * image.width() + x] == BORDER_COLOR) {
        throw (uint)BORDER_COLOR;
    }

    int halfHexWidth = sqrt(3) * Settings::cellSize * 0.5;
    int halfHexSize = Settings::cellSize / 2;

    int rectHeight = Settings::cellSize + halfHexSize;
    int rectWidth = halfHexWidth * 2;

    int hexY = y / rectHeight;
    int hexX = ((x  - (hexY % 2) * rectWidth / 2 + rectWidth) / rectWidth) - 1;

    if ((y - (rectHeight * hexY)) <= halfHexSize) {
        int localY = y - rectHeight * hexY;
        int localX = x - rectWidth * hexX - halfHexWidth * (hexY % 2);

        if ((-1 * halfHexSize * localX) > (localY - halfHexSize) * halfHexWidth) {
            if (hexY % 2 == 0)
                --hexX;

            --hexY;
        }

        if ((halfHexSize * localX) > (localY + halfHexSize) * halfHexWidth) {
            if (hexY % 2 == 1)
                ++hexX;

            --hexY;
        }
    }

    return std::pair<int, int> (hexX, hexY);
}

void DrawPanelWidget::clear() {
    for (int y = 0; y < Settings::ySize; ++y) {
        for (int x = 0; x < Settings::xSize - y % 2; ++x) {
            model->setHexState(y, x, DEAD);
        }
    }

    repaintField();

    repaint();
}

void DrawPanelWidget::drawImpacts(QPainter & painter) {
    if (Settings::cellSize < 15)
        return;

    painter.setPen(Qt::SolidLine);
    painter.setFont(QFont("Arial", 12));

    int size = Settings::cellSize;

    int halfWidth = sqrt(3) * size * 0.5;
    int halfSize = size / 2;

    int xCenter = halfWidth;
    int yCenter = size;

    for (int j = 0; j < Settings::ySize; ++j) {
        for (int i = 0; i < Settings::xSize - (j % 2); ++i) {
            QPoint point(xCenter, yCenter + 6);

            if (model->getHex(j, i).impact % 10 == 0)
                point.setX(point.x() - 4);
            else
                point.setX(point.x() - 10);

            painter.drawText(point, QString::number((float)model->getHex(j, i).impact / 10));
            xCenter += 2 * halfWidth;
        }
        if (j % 2 == 1)
            xCenter = halfWidth;
        else
            xCenter = 2 * halfWidth;

        yCenter += size + halfSize;
    }
}

void DrawPanelWidget::runGame() {
    timer->start(1000 / Settings::speed);
}

void DrawPanelWidget::stopGame() {
    timer->stop();
}

void DrawPanelWidget::runGameSlot() {
    nextStep();
}

bool DrawPanelWidget::checkModel() {
    return model->isActual;
}

bool DrawPanelWidget::openModel(QString fileName) {
    std::ifstream ifs;
    ifs.open(fileName.toStdString());

    if (!ifs.is_open())
        throw std::string("Can't open this file");

    try {
        ifs >> *model;
    } catch(...) {
        throw;
    }

    ifs.close();

    model->isActual = true;
    actualFileName = fileName;

    resizeImage();
    repaintField();
    repaint();

    return true;
}

bool DrawPanelWidget::saveModel(QString fileName) {
    std::ofstream ofs;
    ofs.open(fileName.toStdString());

    if (!ofs.is_open())
        return false;

    ofs << *model;
    ofs.close();

    model->isActual = true;
    actualFileName = fileName;

    return true;
}
