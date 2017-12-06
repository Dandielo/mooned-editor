#include "QGraphicsTextEdit.h"

#include <QKeyEvent>

editor::QGraphicsTextEdit::QGraphicsTextEdit(QGraphicsItem* parent /*= nullptr*/)
    : QGraphicsTextItem(parent)
{
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setAcceptedMouseButtons(Qt::NoButton);
    setFlags(QGraphicsItem::ItemIsFocusable);
}

void editor::QGraphicsTextEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key::Key_Return)
    {
        clearFocus();
        event->ignore();
    }
    else
    {
        bool accepted = true;
        emit typed(event->key(), accepted);

        if (accepted)
        {
            QGraphicsTextItem::keyPressEvent(event);
        }
    }
}

void editor::QGraphicsTextEdit::focusOutEvent(QFocusEvent *event)
{
    QGraphicsTextItem::focusOutEvent(event);
    emit finished(toPlainText());
}
