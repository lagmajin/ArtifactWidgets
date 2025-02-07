module;

#include <QtCore/QFileInfo>

export module IViewer;




export namespace ArtifactWidgets
{
 class QFileInfo;

 class IViewer {

 public:
  virtual void loadFromFile(const QFileInfo& info)=0;
  virtual QFileInfo activeFileInfo() const=0;
  virtual bool isActive() const=0;
 };








}