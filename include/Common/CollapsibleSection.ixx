module;
#include <wobjectdefs.h>
#include <QWidget>

export module CollapsibleSection;

export namespace ArtifactWidgets
{

 class CollapsibleSection : public QWidget {
 	W_OBJECT(CollapsibleSection)
 private:
  class Impl;
  Impl* impl_;

 public:
  explicit CollapsibleSection(const QString& title,
   QWidget* contentWidget = nullptr,
   QWidget* parent = nullptr);
  virtual ~CollapsibleSection();

  void setContent(QWidget* widget);
  void setExpanded(bool expand);
  bool isExpanded() const;


 };







};