#include "QScriptedNodePinValue.h"
#include "QScriptedNodeProperty.h"
#include "QScriptedNodePin.h"
#include "QScriptedNodeEvent.h"

#include "graph/utils/QGraphicsLineEdit.h"
#include "scripts/CScriptManager.h"

#include <QPainter>
#include <QCoreApplication>
#include <QGraphicsSceneMouseEvent>

namespace
{
    QFont TextFont = QFont("Hack", 10, QFont::Weight::Normal);
    //QColor TextColor = QColor::fromRgbF(0.7, 0.7, 0.7, 1.0);
    QFontMetrics TextMetrics = QFontMetrics(TextFont);
}

static QColor getColorForType(int type_id);
static editor::QScriptedNodePinValue* createValueForProperty(editor::QScriptedNodeProperty* property);

template<class T, int TYPEID>
class QScriptedNodePinAsNativeValue : public editor::QScriptedNodePinValue
{
public:
    QScriptedNodePinAsNativeValue(editor::QScriptedNodeProperty* property, const T& initial)
        : QScriptedNodePinValue()
        , _property(property)
        , _value(initial)
    { }
    virtual ~QScriptedNodePinAsNativeValue() override = default;

    virtual QColor color() const override { return getColorForType(TYPEID); }
    virtual uint typeId() const override { return TYPEID; }

    virtual QVariant value() const override
    {
        return { _value };
    }

    virtual QVariant resolvedValue() const override
    {
        return { _connected_value ? _connected_value->value() : _value };
    }

    virtual void setValue(QVariant val) override final
    {
        _value = qvariant_cast<T>(val);

        if (_property->nodePin()->pinType() == editor::PinType::Input)
        {
            // Send and update event to the parent node object
            QCoreApplication::postEvent(_property->nodePin()->parent(), new editor::QScriptedNodeEvent{});
        }
    }

    virtual void beginEdit() override
    {
        _edit_item = new editor::QGraphicsLineEdit({ 0, 0, boundingRect().width(), boundingRect().height() }, this);
        _edit_item->initialize();
        _edit_item->setPos(boundingRect().topLeft());
        _edit_item->setValue(value());
        _edit_item->setColor(color());

        QCoreApplication::connect(_edit_item, &editor::QGraphicsEditableItem::finished, [&](QVariant value)
        {
            setValue(value);
            endEdit();
        });

        QCoreApplication::connect(_edit_item, &editor::QGraphicsEditableItem::changed, [&](QVariant value, bool& accept)
        {
            int key = value.toInt();

            bool is_number = key >= Qt::Key_0 && key <= Qt::Key_9;
            bool is_minus = key == Qt::Key_Minus;
            bool is_period = key == Qt::Key_Period;

            accept = is_number || is_minus || is_period;
        });
    }

    virtual void endEdit() override
    {
        _edit_item->deleteLater();
        _edit_item = nullptr;
    }

    virtual QString toString() const override
    {
        return QString("%1").arg(_value);
    }

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        if (_edit_item == nullptr)
        {
            painter->setFont(TextFont);
            painter->setPen(QPen(color()));

            if (_property->nodePin()->pinType() == editor::PinType::Input)
            {
                QScriptedNodePinValue* pin_value = _connected_value != nullptr ? _connected_value : this;
                QString text = pin_value->toString();

                painter->drawText(boundingRect(), text);

                if (_property->nodePin()->editable())
                {
                    QRectF line_rect = boundingRect().adjusted(0, -2, 0, -2);
                    line_rect.setWidth(TextMetrics.width(text) + 3);

                    painter->setPen(QPen(color(), 1, Qt::DashLine));
                    painter->drawLine(line_rect.bottomLeft(), line_rect.bottomRight());
                }
            }
            else // pin->pinType() == PinType::Output
            {
                painter->drawText(boundingRect(), toString(), QTextOption(Qt::AlignRight));

                if (_property->nodePin()->editable())
                {
                    auto box_width = boundingRect().width();
                    auto text_width = TextMetrics.width(toString());
                    box_width -= text_width;

                    QRectF line_rect = boundingRect().adjusted(box_width, -2, box_width, -2);
                    line_rect.setWidth(text_width + 3);

                    painter->setPen(QPen(color(), 1, Qt::DashLine));
                    painter->drawLine(line_rect.bottomLeft(), line_rect.bottomRight());
                }
            }
        }
    }

protected:
    editor::QScriptedNodeProperty* _property;
    T _value;
};

class QScriptedNodePinAsNativeBoolValue : public QScriptedNodePinAsNativeValue<bool, asTYPEID_BOOL>
{
public:
    QScriptedNodePinAsNativeBoolValue(editor::QScriptedNodeProperty* prop, bool initial)
        : QScriptedNodePinAsNativeValue(prop, initial)
    {
    }

    virtual ~QScriptedNodePinAsNativeBoolValue() override = default;

    virtual void beginEdit() override
    {
        setValue(QVariant{ !_value });
        update();
    }

    virtual void endEdit() override
    {
        /* empty */
    }

    virtual QString toString() const override
    {
        return _value ? "true" : "false";
    }
};

template<class T, int TYPEID>
class QScriptedNodePinAsNativeNumericValue : public QScriptedNodePinAsNativeValue<T, TYPEID>
{
public:
    QScriptedNodePinAsNativeNumericValue(editor::QScriptedNodeProperty* property, const T& initial)
        : QScriptedNodePinAsNativeValue{ property, initial }
        , _last_value{ }
        , _capturing{ false }
        , _initial{ 0.0f, 0.0f }
    {
    }
    virtual ~QScriptedNodePinAsNativeNumericValue() override = default;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* ev) override
    {
        _last_value = _value;
        _capturing = true;
        _initial = ev->pos();
    }

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* ev) override
    {
        if (_capturing)
        {
            setValue(static_cast<T>(_last_value + (_initial.y() - ev->pos().y()) * 1.0));
            update();
        }
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* ev) override
    {
        _capturing = false;
        setValue(static_cast<T>(_last_value + (_initial.y() - ev->pos().y()) * 1.0));
        update();
    }

private:
    T _last_value;
    bool _capturing;
    QPointF _initial;
};


editor::QScriptedNodePinValue* editor::QScriptedNodePinValue::create(editor::QScriptedNodeProperty* prop)
{
    return createValueForProperty(prop);
}

editor::QScriptedNodePinValue* editor::QScriptedNodePinValue::connectedValue() const
{
    return _connected_value;
}

void editor::QScriptedNodePinValue::setConnectedValue(QScriptedNodePinValue* val)
{
    _connected_value = val;
}

QRectF editor::QScriptedNodePinValue::boundingRect() const
{
    return _rect;
}

void editor::QScriptedNodePinValue::setBoundingRect(QRectF rect)
{
    prepareGeometryChange();
    _rect = rect;
}

//////////////////////////////////////////////////////////////////////////
// Returns a color for the given TypeId
QColor getColorForType(int type_id)
{
    switch (type_id)
    {
    case asTYPEID_BOOL:
        return { 195, 89, 95 };
    case asTYPEID_INT8:
    case asTYPEID_INT16:
    case asTYPEID_INT32:
    case asTYPEID_INT64:
    case asTYPEID_UINT8:
    case asTYPEID_UINT16:
    case asTYPEID_UINT32:
    case asTYPEID_UINT64:
        return { 57, 167, 181 };
    case asTYPEID_FLOAT:
    case asTYPEID_DOUBLE:
        return { 117, 227, 215 };
    case 10001: // string
        return { 204, 157, 64 };
    default:
        break;
    }
    return Qt::gray;
}

//////////////////////////////////////////////////////////////////////////
// Returns a new PinValue object for the given TypeId
editor::QScriptedNodePinValue* createValueForProperty(editor::QScriptedNodeProperty* property)
{
    auto type_id = property->subTypeId();
    switch (type_id)
    {
    case asTYPEID_BOOL:
        return new QScriptedNodePinAsNativeBoolValue(property, false);
    case asTYPEID_INT8:
        return new QScriptedNodePinAsNativeNumericValue<int8_t, asTYPEID_INT8>(property, 0);
    case asTYPEID_INT16:
        return new QScriptedNodePinAsNativeNumericValue<int16_t, asTYPEID_INT16>(property, 0);
    case asTYPEID_INT32:
        return new QScriptedNodePinAsNativeNumericValue<int32_t, asTYPEID_INT32>(property, 0);
    case asTYPEID_INT64:
        return new QScriptedNodePinAsNativeNumericValue<int64_t, asTYPEID_INT64>(property, 0);
    case asTYPEID_UINT8:
        return new QScriptedNodePinAsNativeNumericValue<uint8_t, asTYPEID_UINT8>(property, 0);
    case asTYPEID_UINT16:
        return new QScriptedNodePinAsNativeNumericValue<uint16_t, asTYPEID_UINT16>(property, 0);
    case asTYPEID_UINT32:
        return new QScriptedNodePinAsNativeNumericValue<uint32_t, asTYPEID_UINT32>(property, 0);
    case asTYPEID_UINT64:
        return new QScriptedNodePinAsNativeNumericValue<uint64_t, asTYPEID_UINT64>(property, 0);
    case asTYPEID_FLOAT:
        return new QScriptedNodePinAsNativeNumericValue<float, asTYPEID_FLOAT>(property, 0.0f);
    case asTYPEID_DOUBLE:
        return new QScriptedNodePinAsNativeNumericValue<double, asTYPEID_DOUBLE>(property, 0.0);
    case 10001: // string
        return nullptr;
    default:
        break;
    }
    return nullptr;
}