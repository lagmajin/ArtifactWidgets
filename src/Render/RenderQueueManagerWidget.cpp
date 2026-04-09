module;

#include <QLabel>
#include <QWidget>
#include <QTreeView>
#include <QPushButton>
#include <QProgressBar>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QDataStream>
#include <QIODevice>
#include <QMessageBox>
#include <QDesktopServices>
#include <QElapsedTimer>
#include <QInputDialog>
#include <QUrl>
#include <QFileInfo>
#include <QTimer>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionProgressBar>
#include <algorithm>
#include <wobjectimpl.h>
#include <QDebug>

module Widgets.Render.Queue;

import Render;
import Widgets.Utils.CSS;

namespace ArtifactCore {}

namespace ArtifactWidgets {

 using namespace ArtifactCore;

 W_OBJECT_IMPL(RenderQueueControlPanel)
 W_OBJECT_IMPL(RenderQueueManagerJobPanel)
 W_OBJECT_IMPL(RenderQueueManagerWidget)
 W_OBJECT_IMPL(RenderQueueManagerUnderInfoWidget)

 // ─────────────────────────────────────────────────────────
 // RenderQueueControlPanel
 // ─────────────────────────────────────────────────────────

 class RenderQueueControlPanel::Impl {
 public:
  Impl();
  QProgressBar* progressBar = nullptr;
  QPushButton* renderingStartButton = nullptr;
  QPushButton* clearAllRenderQueueButton = nullptr;
  bool isRendering = false;
 };

 RenderQueueControlPanel::Impl::Impl() {}

 RenderQueueControlPanel::RenderQueueControlPanel(QWidget* parent)
   : QWidget(parent), impl_(new Impl())
 {
  auto pHVoxLayout = new QHBoxLayout();

  impl_->renderingStartButton = new QPushButton("▶ Rendering");
  impl_->renderingStartButton->setStyleSheet(
   "QPushButton { border-radius: 6px; background-color: #27ae60; color: white; font-weight: bold; padding: 6px 16px; }"
   "QPushButton:hover { background-color: #2ecc71; }"
   "QPushButton:pressed { background-color: #1e8449; }"
   "QPushButton:disabled { background-color: #555555; color: #888888; }"
  );

  impl_->clearAllRenderQueueButton = new QPushButton("🗑 Clear All");
  impl_->clearAllRenderQueueButton->setStyleSheet(
   "QPushButton { border-radius: 6px; background-color: #c0392b; color: white; font-weight: bold; padding: 6px 16px; }"
   "QPushButton:hover { background-color: #e74c3c; }"
   "QPushButton:pressed { background-color: #922b21; }"
  );

  impl_->progressBar = new QProgressBar();
  impl_->progressBar->setValue(0);
  impl_->progressBar->setTextVisible(true);
  impl_->progressBar->setStyleSheet(
   "QProgressBar { border: 1px solid #555; border-radius: 4px; background-color: #2a2a2a; text-align: center; color: #ffffff; }"
   "QProgressBar::chunk { background-color: #3498db; border-radius: 3px; }"
  );

  pHVoxLayout->addWidget(impl_->progressBar, 1);
  pHVoxLayout->addWidget(impl_->renderingStartButton);
  pHVoxLayout->addWidget(impl_->clearAllRenderQueueButton);
  setLayout(pHVoxLayout);

  connect(impl_->renderingStartButton, &QPushButton::clicked, this, &RenderQueueControlPanel::startRenderingClicked);
  connect(impl_->clearAllRenderQueueButton, &QPushButton::clicked, this, &RenderQueueControlPanel::clearAllClicked);
 }

 RenderQueueControlPanel::~RenderQueueControlPanel() { delete impl_; }

 QProgressBar* RenderQueueControlPanel::progressBar() const { return impl_->progressBar; }
 QPushButton* RenderQueueControlPanel::renderingStartButton() const { return impl_->renderingStartButton; }
 QPushButton* RenderQueueControlPanel::clearAllButton() const { return impl_->clearAllRenderQueueButton; }

 void RenderQueueControlPanel::setRenderingState(bool rendering) {
  impl_->isRendering = rendering;
  impl_->renderingStartButton->setText(rendering ? "⏸ Pause" : "▶ Rendering");
  impl_->renderingStartButton->setEnabled(true);
  impl_->clearAllRenderQueueButton->setEnabled(!rendering);
 }

 void RenderQueueControlPanel::setTotalProgress(int percent) {
  impl_->progressBar->setValue(std::clamp(percent, 0, 100));
 }

 // ─────────────────────────────────────────────────────────
 // Progress Bar Delegate
 // ─────────────────────────────────────────────────────────

class RenderJobProgressDelegate : public QStyledItemDelegate {
public:
  explicit RenderJobProgressDelegate(QObject* parent = nullptr)
    : QStyledItemDelegate(parent) {}

  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
    if (index.column() == 2) { // Progress column
      int progress = index.data(Qt::DisplayRole).toInt();
      QStyleOptionProgressBar progressBarOption;
      progressBarOption.rect = option.rect.adjusted(4, 4, -4, -4);
      progressBarOption.minimum = 0;
      progressBarOption.maximum = 100;
      progressBarOption.progress = progress;
      progressBarOption.text = QString("%1%").arg(progress);
      progressBarOption.textVisible = true;
      progressBarOption.state = option.state;

      if (progress >= 100) {
        progressBarOption.palette.setColor(QPalette::Highlight, QColor(46, 204, 113));
      } else {
        progressBarOption.palette.setColor(QPalette::Highlight, QColor(52, 152, 219));
      }

      QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    } else {
      QStyledItemDelegate::paint(painter, option, index);
    }
  }
 };

 // ─────────────────────────────────────────────────────────
 // RenderQueueManagerJobPanel
 // ─────────────────────────────────────────────────────────

 class RenderQueueManagerJobPanel::Impl {
 public:
  Impl();
  ~Impl();
  QObject* service_ = nullptr;
  QTimer* updateTimer_ = nullptr;
 };

 RenderQueueManagerJobPanel::Impl::Impl() {
  updateTimer_ = new QTimer();
  updateTimer_->setInterval(500);
 }

 RenderQueueManagerJobPanel::Impl::~Impl() {
  delete updateTimer_;
 }

 RenderQueueManagerJobPanel::RenderQueueManagerJobPanel(QWidget* parent)
   : QTreeView(parent), impl_(new Impl())
 {
  auto style = getDCCStyleSheetPreset(DccStylePreset::StudioStyle);
  setStyleSheet(style);

  auto model = new RenderJobModel();
  setModel(model);

  // Enable drag & drop
  setAcceptDrops(true);
  setDragEnabled(true);
  setDragDropMode(QAbstractItemView::InternalMove);
  setDefaultDropAction(Qt::MoveAction);

  // Enable custom context menu
  setContextMenuPolicy(Qt::CustomContextMenu);

  // Header settings
  header()->setSectionResizeMode(0, QHeaderView::Stretch);
  header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  header()->setSectionResizeMode(3, QHeaderView::Stretch);

  // Progress bar delegate
  setItemDelegateForColumn(2, new RenderJobProgressDelegate(this));

  // Selection
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::ExtendedSelection);

  // Connect context menu
  connect(this, &QTreeView::customContextMenuRequested, this, [this](const QPoint& pos) {
    QModelIndex idx = indexAt(pos);
    if (!idx.isValid()) return;
    emit jobContextMenuRequested(idx.row(), viewport()->mapToGlobal(pos));
  });

  // Auto-refresh timer
  connect(impl_->updateTimer_, &QTimer::timeout, this, [this]() {
    if (impl_->service_) {
      refreshJobList();
    }
  });
  impl_->updateTimer_->start();
 }

 RenderQueueManagerJobPanel::~RenderQueueManagerJobPanel() { delete impl_; }

 void RenderQueueManagerJobPanel::setService(QObject* service) {
  impl_->service_ = service;
 }

 void RenderQueueManagerJobPanel::refreshJobList() {
  if (!impl_->service_) return;

  // Use Qt::QueuedConnection to safely call across module boundaries
  int jobCount = 0;
  QMetaObject::invokeMethod(impl_->service_, "jobCount", Qt::DirectConnection,
    Q_RETURN_ARG(int, jobCount));

  auto* model = static_cast<RenderJobModel*>(this->model());
  if (!model) return;

  model->clearJobs();
  for (int i = 0; i < jobCount; ++i) {
    QString compName, status, outputPath;
    int progress = 0;

    QMetaObject::invokeMethod(impl_->service_, "jobCompositionNameAt", Qt::DirectConnection,
      Q_RETURN_ARG(QString, compName), Q_ARG(int, i));
    QMetaObject::invokeMethod(impl_->service_, "jobStatusAt", Qt::DirectConnection,
      Q_RETURN_ARG(QString, status), Q_ARG(int, i));
    QMetaObject::invokeMethod(impl_->service_, "jobProgressAt", Qt::DirectConnection,
      Q_RETURN_ARG(int, progress), Q_ARG(int, i));
    QMetaObject::invokeMethod(impl_->service_, "jobOutputPathAt", Qt::DirectConnection,
      Q_RETURN_ARG(QString, outputPath), Q_ARG(int, i));

    model->addJob(compName, status, progress, outputPath);
  }
 }

 void RenderQueueManagerJobPanel::contextMenuEvent(QContextMenuEvent* event) {
  QModelIndex idx = indexAt(event->pos());
  if (!idx.isValid()) {
    QTreeView::contextMenuEvent(event);
    return;
  }

  int jobIndex = idx.row();
  emit jobContextMenuRequested(jobIndex, event->globalPos());
 }

 void RenderQueueManagerJobPanel::dragEnterEvent(QDragEnterEvent* event) {
  if (event->mimeData()->hasFormat("application/x-artifact-composition-id")) {
    event->acceptProposedAction();
    return;
  }
  if (event->source() == this && event->mimeData()->hasFormat("application/x-render-queue-job")) {
    event->acceptProposedAction();
    return;
  }
  QTreeView::dragEnterEvent(event);
 }

 void RenderQueueManagerJobPanel::dragMoveEvent(QDragMoveEvent* event) {
  if (event->mimeData()->hasFormat("application/x-artifact-composition-id") ||
      event->mimeData()->hasFormat("application/x-render-queue-job")) {
    event->acceptProposedAction();
    return;
  }
  QTreeView::dragMoveEvent(event);
 }

 void RenderQueueManagerJobPanel::dropEvent(QDropEvent* event) {
  if (event->mimeData()->hasFormat("application/x-artifact-composition-id")) {
    QString compId = event->mimeData()->data("application/x-artifact-composition-id");
    emit compositionDropped(compId);
    event->acceptProposedAction();
    return;
  }
  if (event->mimeData()->hasFormat("application/x-render-queue-job")) {
    QByteArray data = event->mimeData()->data("application/x-render-queue-job");
    QDataStream stream(&data, QIODevice::ReadOnly);
    int fromIndex;
    stream >> fromIndex;

    QModelIndex dropIdx = indexAt(event->position().toPoint());
    if (dropIdx.isValid()) {
      int toIndex = dropIdx.row();
      if (fromIndex != toIndex) {
        emit jobDropped(fromIndex, toIndex);
      }
    }
    event->acceptProposedAction();
    return;
  }
  QTreeView::dropEvent(event);
 }

 void RenderQueueManagerJobPanel::startDrag(Qt::DropActions supportedActions) {
  QModelIndexList selected = selectionModel()->selectedRows();
  if (selected.isEmpty()) {
    QTreeView::startDrag(supportedActions);
    return;
  }

  QMimeData* mimeData = new QMimeData();
  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);
  stream << selected.first().row();
  mimeData->setData("application/x-render-queue-job", data);

  QDrag* drag = new QDrag(this);
  drag->setMimeData(mimeData);
  drag->exec(Qt::MoveAction);
 }

 // ─────────────────────────────────────────────────────────
 // RenderQueueManagerWidget
 // ─────────────────────────────────────────────────────────

 class RenderQueueManagerWidget::Impl {
 public:
  Impl();
  ~Impl();
  RenderQueueControlPanel* controlPanel_ = nullptr;
  RenderQueueManagerJobPanel* jobPanel_ = nullptr;
  RenderQueueManagerUnderInfoWidget* infoWidget_ = nullptr;
  QObject* service_ = nullptr;
  QTimer* infoUpdateTimer_ = nullptr;
  QElapsedTimer renderTimer_;
  bool isRendering_ = false;
  int64_t renderStartTime_ = 0;
 };

 RenderQueueManagerWidget::Impl::Impl() {
  infoUpdateTimer_ = new QTimer();
  infoUpdateTimer_->setInterval(1000);
 }

 RenderQueueManagerWidget::Impl::~Impl() {
  delete infoUpdateTimer_;
 }

 RenderQueueManagerWidget::RenderQueueManagerWidget(QWidget* parent)
   : QWidget(parent), impl_(new Impl())
 {
  auto style = getDCCStyleSheetPreset(DccStylePreset::StudioStyle);
  setStyleSheet(style);

  impl_->controlPanel_ = new RenderQueueControlPanel();
  impl_->jobPanel_ = new RenderQueueManagerJobPanel();
  impl_->jobPanel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  impl_->infoWidget_ = new RenderQueueManagerUnderInfoWidget();

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(1);
  layout->addWidget(impl_->controlPanel_);
  layout->addWidget(impl_->jobPanel_, 1);
  layout->addWidget(impl_->infoWidget_);
  setLayout(layout);

  // Connect control panel signals
  connect(impl_->controlPanel_, &RenderQueueControlPanel::startRenderingClicked, this, [this]() {
    if (!impl_->service_) return;
    QMetaObject::invokeMethod(impl_->service_, "startAllJobs", Qt::QueuedConnection);
    impl_->isRendering_ = true;
    impl_->controlPanel_->setRenderingState(true);
    impl_->renderTimer_.start();
  });

  connect(impl_->controlPanel_, &RenderQueueControlPanel::clearAllClicked, this, [this]() {
    if (!impl_->service_) return;
    int count = 0;
    QMetaObject::invokeMethod(impl_->service_, "jobCount", Qt::DirectConnection, Q_RETURN_ARG(int, count));
    if (count == 0) return;

    if (QMessageBox::question(this, "Clear All",
        QString("Remove all %1 job(s) from the queue?").arg(count),
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
      QMetaObject::invokeMethod(impl_->service_, "removeAllRenderQueues", Qt::QueuedConnection);
      impl_->jobPanel_->refreshJobList();
    }
  });

  // Connect job panel signals
  connect(impl_->jobPanel_, &RenderQueueManagerJobPanel::jobContextMenuRequested, this, [this](int jobIndex, const QPoint& pos) {
    showJobContextMenu(jobIndex, pos);
  });

  connect(impl_->jobPanel_, &RenderQueueManagerJobPanel::jobDropped, this, [this](int from, int to) {
    if (!impl_->service_) return;
    QMetaObject::invokeMethod(impl_->service_, "moveRenderQueue", Qt::QueuedConnection,
      Q_ARG(int, from), Q_ARG(int, to));
    impl_->jobPanel_->refreshJobList();
  });

  connect(impl_->jobPanel_, &RenderQueueManagerJobPanel::compositionDropped, this, [this](const QString& compId) {
    if (!impl_->service_) return;
    QMetaObject::invokeMethod(impl_->service_, "addRenderQueueForComposition", Qt::QueuedConnection,
      Q_ARG(QString, compId));
    impl_->jobPanel_->refreshJobList();
  });

  // Info update timer
  connect(impl_->infoUpdateTimer_, &QTimer::timeout, this, [this]() {
    updateInfoPanel();
  });
  impl_->infoUpdateTimer_->start();
 }

 RenderQueueManagerWidget::~RenderQueueManagerWidget() { delete impl_; }

 QSize RenderQueueManagerWidget::sizeHint() const {
  return QSize(500, QWidget::sizeHint().height());
 }

void RenderQueueManagerWidget::setFloatingMode(bool isFloating) {
    if (impl_->controlPanel_) {
        if (auto* progressBar = impl_->controlPanel_->progressBar()) {
            progressBar->setVisible(!isFloating);
        }
        if (auto* startButton = impl_->controlPanel_->renderingStartButton()) {
            startButton->setVisible(!isFloating);
        }
        if (auto* clearButton = impl_->controlPanel_->clearAllButton()) {
            clearButton->setVisible(!isFloating);
        }
    }
}

 void RenderQueueManagerWidget::setService(QObject* service) {
  impl_->service_ = service;
  impl_->jobPanel_->setService(service);
  impl_->jobPanel_->refreshJobList();
 }

 void RenderQueueManagerWidget::showJobContextMenu(int jobIndex, const QPoint& pos) {
  if (!impl_->service_) return;

  QMenu menu(this);
  menu.setStyleSheet(
   "QMenu { background-color: #2a2a2a; color: #ffffff; border: 1px solid #555; }"
   "QMenu::item { padding: 6px 20px; }"
   "QMenu::item:selected { background-color: #3498db; }"
  );

  // Get job status
  QString status;
  QMetaObject::invokeMethod(impl_->service_, "jobStatusAt", Qt::DirectConnection,
    Q_RETURN_ARG(QString, status), Q_ARG(int, jobIndex));

  QAction* renameAction = menu.addAction("✏️ Rename...");
  QAction* dupAction = menu.addAction("📋 Duplicate");
  menu.addSeparator();

  QAction* outputAction = menu.addAction("📁 Open Output Folder");
  QAction* settingsAction = menu.addAction("⚙️ Output Settings...");
  menu.addSeparator();

  QAction* resetAction = nullptr;
  if (status == "Failed" || status == "Completed") {
    resetAction = menu.addAction("🔄 Reset & Re-render");
  }

  QAction* removeAction = menu.addAction("🗑 Remove");

  QAction* chosen = menu.exec(pos);
  if (!chosen) return;

  if (chosen == renameAction) {
    // Inline rename via input dialog
    QString currentName;
    QMetaObject::invokeMethod(impl_->service_, "jobNameAt", Qt::DirectConnection,
      Q_RETURN_ARG(QString, currentName), Q_ARG(int, jobIndex));
    bool ok = false;
    QString newName = QInputDialog::getText(this, "Rename Job", "Job name:", QLineEdit::Normal,
      currentName.isEmpty() ? QString("Job %1").arg(jobIndex + 1) : currentName, &ok);
    if (ok && !newName.trimmed().isEmpty()) {
      QMetaObject::invokeMethod(impl_->service_, "setJobNameAt", Qt::QueuedConnection,
        Q_ARG(int, jobIndex), Q_ARG(QString, newName.trimmed()));
      impl_->jobPanel_->refreshJobList();
    }
  } else if (chosen == dupAction) {
    QMetaObject::invokeMethod(impl_->service_, "duplicateRenderQueueAt", Qt::QueuedConnection,
      Q_ARG(int, jobIndex));
    impl_->jobPanel_->refreshJobList();
  } else if (chosen == outputAction) {
    QString outputPath;
    QMetaObject::invokeMethod(impl_->service_, "jobOutputPathAt", Qt::DirectConnection,
      Q_RETURN_ARG(QString, outputPath), Q_ARG(int, jobIndex));
    if (!outputPath.isEmpty()) {
      QFileInfo fi(outputPath);
      QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absolutePath()));
    }
  } else if (chosen == settingsAction) {
    QMessageBox::information(this, "Output Settings", "Output settings dialog coming soon.");
  } else if (chosen == resetAction) {
    QMetaObject::invokeMethod(impl_->service_, "resetJobForRerun", Qt::QueuedConnection,
      Q_ARG(int, jobIndex));
    impl_->jobPanel_->refreshJobList();
  } else if (chosen == removeAction) {
    QMetaObject::invokeMethod(impl_->service_, "removeRenderQueueAt", Qt::QueuedConnection,
      Q_ARG(int, jobIndex));
    impl_->jobPanel_->refreshJobList();
  }
 }

 void RenderQueueManagerWidget::updateInfoPanel() {
  if (!impl_->service_) return;

  // RAM usage
  int ramMB = 0;
  QMetaObject::invokeMethod(impl_->service_, "getCurrentRamUsageMB", Qt::DirectConnection,
    Q_RETURN_ARG(int, ramMB));

  // Elapsed time
  QString elapsed = "--:--:--";
  if (impl_->isRendering_ && impl_->renderTimer_.isValid()) {
    qint64 secs = impl_->renderTimer_.elapsed() / 1000;
    int h = secs / 3600;
    int m = (secs % 3600) / 60;
    int s = secs % 60;
    elapsed = QString("%1:%2:%3").arg(h, 2, 10, QChar('0')).arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
  }

  // Log (last status message)
  QString log = "Idle";
  QMetaObject::invokeMethod(impl_->service_, "getLastStatusMessage", Qt::DirectConnection,
    Q_RETURN_ARG(QString, log));

  impl_->infoWidget_->updateInfo(log, ramMB, elapsed);
 }

 // ─────────────────────────────────────────────────────────
 // RenderQueueManagerUnderInfoWidget
 // ─────────────────────────────────────────────────────────

 class RenderQueueManagerUnderInfoWidget::Impl {
 public:
  Impl();
  ~Impl();
  QLabel* logLabel = nullptr;
  QLabel* ramUsageLabel = nullptr;
  QLabel* elapsedTimeLabel = nullptr;
 };

 RenderQueueManagerUnderInfoWidget::Impl::Impl() {}
 RenderQueueManagerUnderInfoWidget::Impl::~Impl() {}

 RenderQueueManagerUnderInfoWidget::RenderQueueManagerUnderInfoWidget(QWidget* parent)
   : QWidget(parent), impl_(new Impl())
 {
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(8, 4, 8, 4);

  impl_->logLabel = new QLabel("Log: Idle");
  impl_->logLabel->setStyleSheet("color: #aaaaaa; font-size: 11px;");

  impl_->ramUsageLabel = new QLabel("RAM: -- MB");
  impl_->ramUsageLabel->setStyleSheet("color: #aaaaaa; font-size: 11px;");

  impl_->elapsedTimeLabel = new QLabel("Elapsed: --:--:--");
  impl_->elapsedTimeLabel->setStyleSheet("color: #aaaaaa; font-size: 11px;");

  layout->addWidget(impl_->logLabel, 1);
  layout->addWidget(impl_->ramUsageLabel);
  layout->addWidget(impl_->elapsedTimeLabel);
  setLayout(layout);
 }

 RenderQueueManagerUnderInfoWidget::~RenderQueueManagerUnderInfoWidget() { delete impl_; }

 void RenderQueueManagerUnderInfoWidget::updateInfo(const QString& log, int ramMB, const QString& elapsedTime) {
  impl_->logLabel->setText(QString("Log: %1").arg(log.isEmpty() ? "Idle" : log));
  impl_->ramUsageLabel->setText(QString("RAM: %1 MB").arg(ramMB));
  impl_->elapsedTimeLabel->setText(QString("Elapsed: %1").arg(elapsedTime));
 }

};
