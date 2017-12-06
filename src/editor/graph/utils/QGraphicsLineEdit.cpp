#include "QGraphicsLineEdit.h"

#include <QPainter>
#include <QKeyEvent>
#include <QApplication>

namespace
{
    QFont TextFont = QFont("Hack", 9, QFont::Weight::Normal);
    //QColor TextColor = QColor::fromRgbF(0.7, 0.7, 0.7, 1.0);
    QFontMetrics TextMetrics = QFontMetrics(TextFont);
}

editor::QGraphicsLineEdit::QGraphicsLineEdit(QRectF rect, QGraphicsItem* parent)
    : QGraphicsEditableItem(parent)
    , _rect(rect)
    , _text_item(this)
{
    _text_item.setPos(-4, -4);
    _text_item.setZValue(6.0);
    _text_item.setTextInteractionFlags(Qt::TextEditorInteraction);
    _text_item.setAcceptedMouseButtons(Qt::NoButton);
    _text_item.setFlags(QGraphicsItem::ItemIsFocusable);
    _text_item.setFont(TextFont);
    _text_item.setTextWidth(_rect.width() + 6);

    connect(&_text_item, &editor::QGraphicsTextEdit::finished, this, &QGraphicsLineEdit::editFinished);
    connect(&_text_item, &editor::QGraphicsTextEdit::typed, this, &QGraphicsLineEdit::editCheck);
}

editor::QGraphicsLineEdit::~QGraphicsLineEdit()
{
}

void editor::QGraphicsLineEdit::initialize()
{
    _text_item.setFocus();
}

QRectF editor::QGraphicsLineEdit::boundingRect() const
{
    return _rect;
}

void editor::QGraphicsLineEdit::setColor(QColor color)
{
    _text_item.setDefaultTextColor(color);
}

void editor::QGraphicsLineEdit::editCheck(int key, bool& accept)
{
    bool is_control = QApplication::keyboardModifiers() == Qt::Modifier::CTRL;
    bool is_acxv = key == Qt::Key_A || key == Qt::Key_C || key == Qt::Key_X || key == Qt::Key_V;

    if (is_control && is_acxv)
    {
        return;
    }

    bool is_backspace = key == Qt::Key_Backspace;
    bool is_delete = key == Qt::Key_Delete;
    bool is_arrow_key = key == Qt::Key_Up || key == Qt::Key_Down || key == Qt::Key_Left || key == Qt::Key_Right;
    bool is_home_end_key = key == Qt::Key_Home || key == Qt::Key_End;

    if (!(is_backspace || is_delete ||  is_arrow_key || is_home_end_key))
    {
        emit changed(QVariant::fromValue(key), accept);
    }
}

void editor::QGraphicsLineEdit::editFinished(QString value)
{
    emit finished(QVariant::fromValue(value));
}

void editor::QGraphicsLineEdit::setValue(QVariant value)
{
    _text_item.setPlainText(value.toString());
}

void editor::QGraphicsLineEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= Q_NULLPTR*/)
{
}
