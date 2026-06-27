module;

#include <qboxlayout.h>
#include <QLabel>
#include <QPushButton>
#include <wobjectimpl.h>
#include <QWidget>


module HeadPanel;

import Widgets.Utils.CSS;

namespace ArtifactWidgets
{
 using namespace ArtifactCore;

 W_OBJECT_IMPL(HeaderPanel)









  HeaderPanel::HeaderPanel(const QString& title, QWidget* parent /*= nullptr*/) : QWidget(parent)
 {
  // Global palette + CommonStyle がテーマを担当するため、
  // 個別の DCC preset QSS は設定しない


  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  // ヘッダー部分
  auto header = new QWidget(this);
  header->setObjectName("header");

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

}
