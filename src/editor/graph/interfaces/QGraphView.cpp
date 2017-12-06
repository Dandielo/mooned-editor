#include "QGraphView.h"
#include <QtMath>

#include <QGLWidget>

editor::QGraphView::QGraphView(QWidget* parent /*= nullptr*/) : QGraphicsView(parent)
{
    _scale = 1.0;
}

void editor::QGraphView::enterEvent(QEvent* event)
{
    QGraphicsView::enterEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}

void editor::QGraphView::mouseMoveEvent(QMouseEvent* event)
{
    _target_viewport_pos = event->pos();
    _target_scene_pos = mapToScene(event->pos());
    QGraphicsView::mouseMoveEvent(event);
}

void editor::QGraphView::mousePressEvent(QMouseEvent* event)
{
    if (event->modifiers() == Qt::ControlModifier && event->button() == Qt::LeftButton)
    {
        auto item = itemAt(event->pos());
        if (item == nullptr)
            setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else if (event->modifiers() != Qt::ControlModifier && event->button() == Qt::LeftButton)
    {
        auto item = itemAt(event->pos());
        if (item == nullptr)
            setDragMode(QGraphicsView::RubberBandDrag);
    }
    QGraphicsView::mousePressEvent(event);
}

void editor::QGraphView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
    setDragMode(QGraphicsView::NoDrag);
}

void editor::QGraphView::wheelEvent(QWheelEvent* event)
{
    double angle = event->angleDelta().y();
    if (angle < 0)
        angle = -60;
    else
        angle = 60;

    double factor = qPow(1.0015, angle);

    if (_scale < 0.5)
    {
        double new_scale = _scale * factor;
        if (new_scale < 0.5)
            return;
    }
    if (_scale >= 1.0)
    {
        double new_scale = _scale * factor;
        if (new_scale > 1.0)
            return;
    }

    QPointF scenePos = _target_scene_pos;
    _scale *= factor;

    scale(factor, factor);
    centerOn(scenePos);
    
    QPointF delta_viewport_pos = _target_viewport_pos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
    QPointF viewport_center = mapFromScene(scenePos) - delta_viewport_pos;
    centerOn(mapToScene(viewport_center.toPoint()));
}

void editor::QGraphView::contextMenuEvent(QContextMenuEvent *event)
{
    //auto event_pos = event->pos();
    //auto scene_pos = mapToScene(event_pos);
    //auto item = itemAt(event->pos());

    //// Menu building 
    //QMenu menu(this);

    //if (item == nullptr)
    //{
    //    initSceneContextMenu(menu, scene_pos, reinterpret_cast<QWorkspaceScene*>(scene()));
    //}
    //else
    //{
    //    QNodePin* nodePin;
    //    QNode* node;

    //    nodePin = dynamic_cast<QNodePin*>(item);
    //    if (nodePin)
    //    {
    //        node = dynamic_cast<QNode*>(nodePin->parentItem());
    //    }
    //    else
    //    {
    //        node = dynamic_cast<QNode*>(item);
    //        if (!node)
    //            node = dynamic_cast<QNode*>(item->parentItem());
    //    }

    //    // Build the context menu
    //    initNodeContextMenu(menu, node);
    //    initNodePinContextMenu(menu, nodePin);
    //}

    //// Show the menu
    //menu.exec(event->globalPos());
}

void editor::QGraphView::drawBackground(QPainter* painter, const QRectF& rect)
{
    drawGrid(painter, rect);
}

void editor::QGraphView::drawGrid(QPainter* painter, const QRectF& rect)
{
    // Workspace colors
    static int BackgroundColorValue = 0x35;
    static int BackgroundColorLineValue = 0x00;
    static int LineColorDarkValue = 0x45;
    static int LineColorLightValue = 0x75;

    static auto BackgroundColor = QColor::fromRgb(BackgroundColorValue, BackgroundColorValue, BackgroundColorValue, 0xff);
    //BackgroundLineColor = QColor::fromRgb(BackgroundColorLineValue, BackgroundColorLineValue, BackgroundColorLineValue, 0xff);

    static auto LineColorDark = QColor::fromRgb(LineColorDarkValue, LineColorDarkValue, LineColorDarkValue, 0xff);
    static auto LineColorLight = QColor::fromRgb(LineColorLightValue, LineColorLightValue, LineColorLightValue, 0xff);

    //int boardSpacing = 100 * (1.0 / _scale);
    //int boardSpacingS = boardSpacing / 10;

    //QSize sceneSize = QSize{ (int)rect.width(), (int)rect.height() };

    //int boardWidth = sceneSize.width();
    //int boardHeight = sceneSize.height();

    //int boardLeft = rect.left();
    //int boardRight = rect.right();
    //int boardTop = rect.top();
    //int boardBottom = rect.bottom();

    //QVector<QPoint> lines;
    //QVector<QPoint> smallLines;

    //{// Vertical
    //    int xOffset = boardLeft % boardSpacing;
    //    int endDraw = boardRight + xOffset;

    //    for (int linePos = rect.left() - xOffset; linePos <= endDraw; linePos += boardSpacing)
    //    {
    //        lines.push_back({ linePos, -boardTop });
    //        lines.push_back({ linePos, boardBottom });

    //        for (int i = 1, sLinePos = linePos + boardSpacingS; i < 10 && sLinePos <= endDraw; ++i, sLinePos += boardSpacingS)
    //        {
    //            smallLines.push_back({ sLinePos, -boardTop });
    //            smallLines.push_back({ sLinePos, boardBottom });
    //        }
    //    }
    //}
    //{// Horizontal
    //    int yOffset = boardTop % boardSpacing;
    //    int endDraw = boardBottom + yOffset;

    //    for (int linePos = boardTop - yOffset; linePos <= endDraw; linePos += boardSpacing)
    //    {
    //        lines.push_back({ boardLeft, linePos });
    //        lines.push_back({ boardRight, linePos });

    //        for (int i = 1, sLinePos = linePos + boardSpacingS; i < 10 && sLinePos <= endDraw; ++i, sLinePos += boardSpacingS)
    //        {
    //            smallLines.push_back({ boardLeft, sLinePos });
    //            smallLines.push_back({ boardRight, sLinePos });
    //        }
    //    }
    //}

    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->fillRect(rect, BackgroundColor);
    //painter->drawLines(smallLines);
    //painter->setPen(QPen(LineColorLight, 1));
    //painter->drawLines(lines);
    int gridSize = 10;
    if (_scale < 1.0)
        gridSize = 25;

    auto draw_grid = [&](int gridSize, QPen pen) 
    {
        qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
        qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

        QVarLengthArray<QLineF, 100> lines;

        for (qreal x = left; x < rect.right(); x += gridSize)
            lines.append(QLineF(x, rect.top(), x, rect.bottom()));
        for (qreal y = top; y < rect.bottom(); y += gridSize)
            lines.append(QLineF(rect.left(), y, rect.right(), y));

        //qDebug() << lines.size();

        painter->setPen(pen);
        painter->drawLines(lines.data(), lines.size());
    };

    draw_grid(gridSize, QPen(LineColorDark, gridSize / 10));
    draw_grid(gridSize * 5, QPen(LineColorLight, gridSize / 10));
}
