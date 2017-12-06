#include "QBasicGraphView.h"

editor::QBasicGraphView::QBasicGraphView(QWidget* parent /*= nullptr*/) : QGraphView(parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    setRubberBandSelectionMode(Qt::ContainsItemShape);
}


