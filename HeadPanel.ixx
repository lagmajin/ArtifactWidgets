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
  explicit HeaderPanel(const QString& title, QWidget* parent = nullptr)
   : QWidget(parent)
  {
   auto layout = new QVBoxLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);

   // ヘッダー部分
   auto header = new QWidget(this);
   header->setObjectName("header");
   header->setStyleSheet("background-color: #333; color: white;");

   auto headerLayout = new QHBoxLayout(header);
   headerLayout->setContentsMargins(5, 2, 5, 2);

   auto label = new QLabel(title, header);
   headerLayout->addWidget(label);

   auto btnCollapse = new QPushButton("-", header);
   btnCollapse->setFixedSize(16, 16);
   headerLayout->addWidget(btnCollapse);

   layout->addWidget(header);

   // コンテンツ部分（ここに後で好きなwidgetをセット）
   m_content = new QWidget(this);
   layout->addWidget(m_content);

   connect(btnCollapse, &QPushButton::clicked, this, [this]() {
	m_content->setVisible(!m_content->isVisible());
	});
  }

  QWidget* contentWidget() const { return m_content; }

 private:
  QWidget* m_content;
 };











};