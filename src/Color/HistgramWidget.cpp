module;
#include <QtGui/QPainter>

//#include "../../../include/Color/HistgramWidget.hpp"

module HistgramWidget;

namespace ArtifactWidgets {
 class HistgramWidget::Impl {
 private:

 public:
  Impl();
  ~Impl();
 };

 HistgramWidget::Impl::Impl()
 {
 }

 HistgramWidget::Impl::~Impl()
 {
 }

 HistgramWidget::HistgramWidget(QWidget* parent /*= nullptr*/):QWidget(parent)
 {

 }

 HistgramWidget::~HistgramWidget()
 {

 }

 void HistgramWidget::paintEvent(QPaintEvent* event)
 {

 }



};