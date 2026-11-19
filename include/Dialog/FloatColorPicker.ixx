module;
#include <QDialog>
#include <wobjectdefs.h>
#include "../Define/DllExportMacro.hpp"

// MSVC 19.44 does not turn class-level dllexport on a module IFC's static
// Q_OBJECT data into dllimport for consumers. Export the data member itself.
#undef Q_OBJECT
#define Q_OBJECT \
public: \
    QT_WARNING_PUSH \
    Q_OBJECT_NO_OVERRIDE_WARNING \
    static WIDGET_LIBRARY_DLL_API const QMetaObject staticMetaObject; \
    virtual const QMetaObject *metaObject() const; \
    virtual void *qt_metacast(const char *); \
    virtual int qt_metacall(QMetaObject::Call, int, void **); \
    QT_TR_FUNCTIONS \
private: \
    QT_OBJECT_GADGET_COMMON \
    QT_DEFINE_TAG_STRUCT(QPrivateSignal); \
    QT_WARNING_POP \
    QT_ANNOTATE_CLASS(qt_qobject, "")

export module FloatColorPickerDialog;

import Widgets.Dialog.Abstract;
import Widgets.ColorWheel;
import Color.Float;

export namespace ArtifactWidgets
{

 class FloatColorPicker : public AbstractDialog
 {
  W_OBJECT(FloatColorPicker)
 private:
  class Impl;
  Impl* impl_;
 public:
  WIDGET_LIBRARY_DLL_API explicit FloatColorPicker(QWidget* parent = nullptr);
  WIDGET_LIBRARY_DLL_API ~FloatColorPicker();

  WIDGET_LIBRARY_DLL_API ArtifactCore::FloatColor getColor() const;
  WIDGET_LIBRARY_DLL_API void setColor(const ArtifactCore::FloatColor& color);
  WIDGET_LIBRARY_DLL_API void setInitialColor(const ArtifactCore::FloatColor& color);

 signals:
  WIDGET_LIBRARY_DLL_API void colorChanged(const ArtifactCore::FloatColor& color) W_SIGNAL(colorChanged, color);
 public slots:
  WIDGET_LIBRARY_DLL_API void accept() override;
  WIDGET_LIBRARY_DLL_API void reject() override;
 };

}
