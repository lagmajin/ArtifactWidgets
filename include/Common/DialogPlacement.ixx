module;

#include <QtWidgets/QWidget>
#include "../Define/DllExportMacro.hpp"

export module Widgets.Common.DialogPlacement;

namespace ArtifactWidgets {

export WIDGET_LIBRARY_DLL_API void installDialogCentering(QWidget* mainWindow);

}
