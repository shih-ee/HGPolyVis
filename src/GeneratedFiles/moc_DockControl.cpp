/****************************************************************************
** Meta object code from reading C++ file 'DockControl.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../DockControl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DockControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DockControl_t {
    QByteArrayData data[21];
    char stringdata0[523];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DockControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DockControl_t qt_meta_stringdata_DockControl = {
    {
QT_MOC_LITERAL(0, 0, 11), // "DockControl"
QT_MOC_LITERAL(1, 12, 30), // "HLBFGSLinesearchITERMAXChanged"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 3), // "val"
QT_MOC_LITERAL(4, 48, 22), // "HLBFGSUpdateNumChanged"
QT_MOC_LITERAL(5, 71, 55), // "on_spinBox_MaximumPolygonArea..."
QT_MOC_LITERAL(6, 127, 4), // "arg1"
QT_MOC_LITERAL(7, 132, 30), // "energyDivisionThresholdChanged"
QT_MOC_LITERAL(8, 163, 18), // "digonWeightChanged"
QT_MOC_LITERAL(9, 182, 17), // "digonScaleChanged"
QT_MOC_LITERAL(10, 200, 45), // "on_doubleSpinBox_PS_EXPECT_GA..."
QT_MOC_LITERAL(11, 246, 47), // "on_doubleSpinBox_CircleRadius..."
QT_MOC_LITERAL(12, 294, 3), // "arg"
QT_MOC_LITERAL(13, 298, 44), // "on_doubleSpinBox_ExpectedLeng..."
QT_MOC_LITERAL(14, 343, 43), // "on_doubleSpinBox_OCBufferScal..."
QT_MOC_LITERAL(15, 387, 20), // "nsBufferScaleChanged"
QT_MOC_LITERAL(16, 408, 31), // "multipleDigonAngleOffsetChanged"
QT_MOC_LITERAL(17, 440, 25), // "toggleOptimizationButtons"
QT_MOC_LITERAL(18, 466, 3), // "res"
QT_MOC_LITERAL(19, 470, 46), // "on_comboBox_LayoutSelector_cu..."
QT_MOC_LITERAL(20, 517, 5) // "index"

    },
    "DockControl\0HLBFGSLinesearchITERMAXChanged\0"
    "\0val\0HLBFGSUpdateNumChanged\0"
    "on_spinBox_MaximumPolygonAreaVerticesCount_valueChanged\0"
    "arg1\0energyDivisionThresholdChanged\0"
    "digonWeightChanged\0digonScaleChanged\0"
    "on_doubleSpinBox_PS_EXPECT_GAMMA_valueChanged\0"
    "on_doubleSpinBox_CircleRadiusRatio_valueChanged\0"
    "arg\0on_doubleSpinBox_ExpectedLength_valueChanged\0"
    "on_doubleSpinBox_OCBufferScale_valueChanged\0"
    "nsBufferScaleChanged\0"
    "multipleDigonAngleOffsetChanged\0"
    "toggleOptimizationButtons\0res\0"
    "on_comboBox_LayoutSelector_currentIndexChanged\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DockControl[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x0a /* Public */,
       4,    1,   87,    2, 0x0a /* Public */,
       5,    1,   90,    2, 0x0a /* Public */,
       7,    1,   93,    2, 0x0a /* Public */,
       8,    1,   96,    2, 0x0a /* Public */,
       9,    1,   99,    2, 0x0a /* Public */,
      10,    1,  102,    2, 0x0a /* Public */,
      11,    1,  105,    2, 0x0a /* Public */,
      13,    1,  108,    2, 0x0a /* Public */,
      14,    1,  111,    2, 0x0a /* Public */,
      15,    1,  114,    2, 0x0a /* Public */,
      16,    1,  117,    2, 0x0a /* Public */,
      17,    1,  120,    2, 0x0a /* Public */,
      19,    1,  123,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,   12,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::Int,   20,

       0        // eod
};

void DockControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DockControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->HLBFGSLinesearchITERMAXChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->HLBFGSUpdateNumChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_spinBox_MaximumPolygonAreaVerticesCount_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->energyDivisionThresholdChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->digonWeightChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->digonScaleChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->on_doubleSpinBox_PS_EXPECT_GAMMA_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->on_doubleSpinBox_CircleRadiusRatio_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->on_doubleSpinBox_ExpectedLength_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->on_doubleSpinBox_OCBufferScale_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->nsBufferScaleChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->multipleDigonAngleOffsetChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->toggleOptimizationButtons((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->on_comboBox_LayoutSelector_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DockControl::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_DockControl.data,
    qt_meta_data_DockControl,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DockControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DockControl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DockControl.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DockControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
