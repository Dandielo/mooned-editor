#pragma once 
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>

namespace editor
{
    class QGraphView : public QGraphicsView
    {
        Q_OBJECT;

    public:
        QGraphView(QWidget* parent = nullptr);
        virtual ~QGraphView() override = default;

    protected:
        virtual void enterEvent(QEvent* event) override;
        virtual void mouseMoveEvent(QMouseEvent* event) override;
        virtual void mousePressEvent(QMouseEvent* event) override;
        virtual void mouseReleaseEvent(QMouseEvent* event) override;
        virtual void wheelEvent(QWheelEvent* event) override;
        virtual void contextMenuEvent(QContextMenuEvent *event) override;

        virtual void drawBackground(QPainter* painter, const QRectF& rect) override;
        virtual void drawGrid(QPainter* painter, const QRectF& rect);

    private:
        //IWorkspace* m_Workspace;
        QPointF _target_scene_pos;
        QPointF _target_viewport_pos;

        double _scale;

        // Dialogs
        //QListDialog* m_NodeListDialog;
    };
}
