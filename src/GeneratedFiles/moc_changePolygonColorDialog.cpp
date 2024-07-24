/****************************************************************************
** Meta object code from reading C++ file 'changePolygonColorDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../changePolygonColorDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'changePolygonColorDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_changePolygonColorDialog_t {
    QByteArrayData data[15];
    char stringdata0[400];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_changePolygonColorDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_changePolygonColorDialog_t qt_meta_stringdata_changePolygonColorDialog = {
    {
QT_MOC_LITERAL(0, 0, 24), // "changePolygonColorDialog"
QT_MOC_LITERAL(1, 25, 21), // "on_buttonBox_accepted"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 21), // "on_buttonBox_rejected"
QT_MOC_LITERAL(4, 70, 48), // "on_pushButton_SetPolygonColor..."
QT_MOC_LITERAL(5, 119, 34), // "on_pushButton_UpdateColors_cl..."
QT_MOC_LITERAL(6, 154, 42), // "on_pushButton_resetToDefaultC..."
QT_MOC_LITERAL(7, 197, 40), // "on_pushButton_SwapSelectedCol..."
QT_MOC_LITERAL(8, 238, 37), // "on_pushButton_OutputColorInfo..."
QT_MOC_LITERAL(9, 276, 39), // "on_pushButton_addToColorPalet..."
QT_MOC_LITERAL(10, 316, 22), // "pushButtonColorClicked"
QT_MOC_LITERAL(11, 339, 19), // "spinBoxColorChanged"
QT_MOC_LITERAL(12, 359, 4), // "arg1"
QT_MOC_LITERAL(13, 364, 17), // "load_Color_Scheme"
QT_MOC_LITERAL(14, 382, 17) // "save_Color_Scheme"

    },
    "changePolygonColorDialog\0on_buttonBox_accepted\0"
    "\0on_buttonBox_rejected\0"
    "on_pushButton_SetPolygonColorsAsSelected_clicked\0"
    "on_pushButton_UpdateColors_clicked\0"
    "on_pushButton_resetToDefaultColors_clicked\0"
    "on_pushButton_SwapSelectedColors_clicked\0"
    "on_pushButton_OutputColorInfo_clicked\0"
    "on_pushButton_addToColorPalette_clicked\0"
    "pushButtonColorClicked\0spinBoxColorChanged\0"
    "arg1\0load_Color_Scheme\0save_Color_Scheme"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_changePolygonColorDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    0,   79,    2, 0x08 /* Private */,
       8,    0,   80,    2, 0x08 /* Private */,
       9,    0,   81,    2, 0x08 /* Private */,
      10,    0,   82,    2, 0x08 /* Private */,
      11,    1,   83,    2, 0x08 /* Private */,
      13,    0,   86,    2, 0x08 /* Private */,
      14,    0,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void changePolygonColorDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<changePolygonColorDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_buttonBox_accepted(); break;
        case 1: _t->on_buttonBox_rejected(); break;
        case 2: _t->on_pushButton_SetPolygonColorsAsSelected_clicked(); break;
        case 3: _t->on_pushButton_UpdateColors_clicked(); break;
        case 4: _t->on_pushButton_resetToDefaultColors_clicked(); break;
        case 5: _t->on_pushButton_SwapSelectedColors_clicked(); break;
        case 6: _t->on_pushButton_OutputColorInfo_clicked(); break;
        case 7: _t->on_pushButton_addToColorPalette_clicked(); break;
        case 8: _t->pushButtonColorClicked(); break;
        case 9: _t->spinBoxColorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->load_Color_Scheme(); break;
        case 11: _t->save_Color_Scheme(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject changePolygonColorDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_changePolygonColorDialog.data,
    qt_meta_data_changePolygonColorDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *changePolygonColorDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *changePolygonColorDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_changePolygonColorDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int changePolygonColorDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
