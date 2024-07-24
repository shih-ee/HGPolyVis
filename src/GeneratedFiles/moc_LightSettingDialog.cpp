/****************************************************************************
** Meta object code from reading C++ file 'LightSettingDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../LightSettingDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LightSettingDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LightSettingDialog_t {
    QByteArrayData data[12];
    char stringdata0[315];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LightSettingDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LightSettingDialog_t qt_meta_stringdata_LightSettingDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "LightSettingDialog"
QT_MOC_LITERAL(1, 19, 11), // "updateLight"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 29), // "on_checkBox_UseLIght0_clicked"
QT_MOC_LITERAL(4, 62, 7), // "checked"
QT_MOC_LITERAL(5, 70, 29), // "on_checkBox_UseLIght1_clicked"
QT_MOC_LITERAL(6, 100, 31), // "on_checkBox_Draw_Lights_clicked"
QT_MOC_LITERAL(7, 132, 44), // "on_doubleSpinBox_lightGlyphSi..."
QT_MOC_LITERAL(8, 177, 3), // "arg"
QT_MOC_LITERAL(9, 181, 47), // "on_doubleSpinBox_CylinderShin..."
QT_MOC_LITERAL(10, 229, 45), // "on_doubleSpinBox_SphereShinin..."
QT_MOC_LITERAL(11, 275, 39) // "on_pushButton_ResetLightSourc..."

    },
    "LightSettingDialog\0updateLight\0\0"
    "on_checkBox_UseLIght0_clicked\0checked\0"
    "on_checkBox_UseLIght1_clicked\0"
    "on_checkBox_Draw_Lights_clicked\0"
    "on_doubleSpinBox_lightGlyphSize_valueChanged\0"
    "arg\0on_doubleSpinBox_CylinderShininess_valueChanged\0"
    "on_doubleSpinBox_SphereShininess_valueChanged\0"
    "on_pushButton_ResetLightSources_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LightSettingDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       3,    1,   57,    2, 0x0a /* Public */,
       5,    1,   60,    2, 0x0a /* Public */,
       6,    1,   63,    2, 0x0a /* Public */,
       7,    1,   66,    2, 0x0a /* Public */,
       9,    1,   69,    2, 0x0a /* Public */,
      10,    1,   72,    2, 0x0a /* Public */,
      11,    0,   75,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void,

       0        // eod
};

void LightSettingDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LightSettingDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateLight((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->on_checkBox_UseLIght0_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_checkBox_UseLIght1_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_checkBox_Draw_Lights_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_doubleSpinBox_lightGlyphSize_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->on_doubleSpinBox_CylinderShininess_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->on_doubleSpinBox_SphereShininess_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->on_pushButton_ResetLightSources_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LightSettingDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_LightSettingDialog.data,
    qt_meta_data_LightSettingDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LightSettingDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LightSettingDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LightSettingDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int LightSettingDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
