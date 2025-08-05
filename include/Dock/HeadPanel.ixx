module;
#include <qboxlayout.h>
#include <QLabel>
#include <QPushButton>
#include <wobjectimpl.h>
#include <QWidget>

export module HeadPanel;


export namespace ArtifactWidgets
{

 class HeaderPanel : public QWidget {
  W_OBJECT(HeaderPanel)
 public:
  explicit HeaderPanel(const QString& title, QWidget* parent = nullptr);

  QWidget* contentWidget() const { return m_content; }

 private:
  QWidget* m_content;
 };











};