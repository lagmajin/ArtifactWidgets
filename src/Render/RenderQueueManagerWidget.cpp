module;

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QDataStream>
#include <QDebug>
#include <QDesktopServices>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIODevice>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QIcon>
#include <QCheckBox>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QPalette>
#include <QProgressBar>
#include <QPixmap>
#include <QPushButton>
#include <QStyleOptionProgressBar>
#include <QStyledItemDelegate>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QTreeView>
#include <QStyle>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include <algorithm>
#include <wobjectimpl.h>


module Widgets.Render.Queue;

import Render;
import AppProgress;
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
  QProgressBar *progressBar = nullptr;
  QPushButton *renderingStartButton = nullptr;
  QPushButton *clearAllRenderQueueButton = nullptr;
  QPushButton *addAllCompositionsButton = nullptr;
  QCheckBox *autoOpenOutputFolderCheckBox = nullptr;
  bool isRendering = false;
};

RenderQueueControlPanel::Impl::Impl() {}

RenderQueueControlPanel::RenderQueueControlPanel(QWidget *parent)
    : QWidget(parent), impl_(new Impl()) {
  auto pHVoxLayout = new QHBoxLayout();

  impl_->renderingStartButton = new QPushButton("▶ Rendering");
  impl_->renderingStartButton->setStyleSheet(
      "QPushButton { border-radius: 6px; background-color: #27ae60; color: "
      "white; font-weight: bold; padding: 6px 16px; }"
      "QPushButton:hover { background-color: #2ecc71; }"
      "QPushButton:pressed { background-color: #1e8449; }"
      "QPushButton:disabled { background-color: #555555; color: #888888; }");

  impl_->clearAllRenderQueueButton = new QPushButton("🗑 Clear All");
  impl_->clearAllRenderQueueButton->setStyleSheet(
      "QPushButton { border-radius: 6px; background-color: #c0392b; color: "
      "white; font-weight: bold; padding: 6px 16px; }"
      "QPushButton:hover { background-color: #e74c3c; }"
      "QPushButton:pressed { background-color: #922b21; }");

  impl_->addAllCompositionsButton = new QPushButton("＋ Add All Comps");
  impl_->addAllCompositionsButton->setStyleSheet(
      "QPushButton { border-radius: 6px; background-color: #3c3c3c; color: "
      "white; font-weight: bold; padding: 6px 16px; }"
      "QPushButton:hover { background-color: #4a4a4a; }"
      "QPushButton:pressed { background-color: #2b2b2b; }");

  impl_->progressBar = new QProgressBar();
  impl_->progressBar->setValue(0);
  impl_->progressBar->setTextVisible(true);
  impl_->progressBar->setStyleSheet(
      "QProgressBar { border: 1px solid #555; border-radius: 4px; "
      "background-color: #2a2a2a; text-align: center; color: #ffffff; }"
      "QProgressBar::chunk { background-color: #3498db; border-radius: 3px; }");

  pHVoxLayout->addWidget(impl_->progressBar, 1);
  impl_->autoOpenOutputFolderCheckBox = new QCheckBox("Open folder when done");
  impl_->autoOpenOutputFolderCheckBox->setChecked(false);
  pHVoxLayout->addWidget(impl_->autoOpenOutputFolderCheckBox);
  pHVoxLayout->addWidget(impl_->addAllCompositionsButton);
  pHVoxLayout->addWidget(impl_->renderingStartButton);
  pHVoxLayout->addWidget(impl_->clearAllRenderQueueButton);
  setLayout(pHVoxLayout);

  connect(impl_->renderingStartButton, &QPushButton::clicked, this,
          &RenderQueueControlPanel::startRenderingClicked);
  connect(impl_->clearAllRenderQueueButton, &QPushButton::clicked, this,
          &RenderQueueControlPanel::clearAllClicked);
  connect(impl_->addAllCompositionsButton, &QPushButton::clicked, this,
          &RenderQueueControlPanel::addAllCompositionsClicked);
}

RenderQueueControlPanel::~RenderQueueControlPanel() { delete impl_; }

QProgressBar *RenderQueueControlPanel::progressBar() const {
  return impl_->progressBar;
}
QPushButton *RenderQueueControlPanel::renderingStartButton() const {
  return impl_->renderingStartButton;
}
QPushButton *RenderQueueControlPanel::clearAllButton() const {
  return impl_->clearAllRenderQueueButton;
}
QPushButton *RenderQueueControlPanel::addAllCompositionsButton() const {
  return impl_->addAllCompositionsButton;
}
QCheckBox *RenderQueueControlPanel::autoOpenOutputFolderCheckBox() const {
  return impl_->autoOpenOutputFolderCheckBox;
}

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
  explicit RenderJobProgressDelegate(QObject *parent = nullptr)
      : QStyledItemDelegate(parent) {}

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override {
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
        progressBarOption.palette.setColor(QPalette::Highlight,
                                           QColor(46, 204, 113));
      } else {
        progressBarOption.palette.setColor(QPalette::Highlight,
                                           QColor(52, 152, 219));
      }

      QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                         &progressBarOption, painter);
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
  QObject *service_ = nullptr;
  QTimer *updateTimer_ = nullptr;
  QHash<int, QPixmap> previewIcons_;
  QMetaObject::Connection previewFrameReadyConnection_;
};

RenderQueueManagerJobPanel::Impl::Impl() {
  updateTimer_ = new QTimer();
  updateTimer_->setInterval(500);
}

RenderQueueManagerJobPanel::Impl::~Impl() { delete updateTimer_; }

RenderQueueManagerJobPanel::RenderQueueManagerJobPanel(QWidget *parent)
    : QTreeView(parent), impl_(new Impl()) {
  auto style = getDCCStyleSheetPreset(DccStylePreset::StudioStyle);
  setStyleSheet(style);

  auto model = new RenderJobModel();
  setModel(model);

  // Enable drag & drop
  setAcceptDrops(true);
  setDragEnabled(true);
  setDragDropMode(QAbstractItemView::InternalMove);
  setDefaultDropAction(Qt::MoveAction);
  setDropIndicatorShown(true);
  setDragDropOverwriteMode(false);

  // Enable custom context menu
  setContextMenuPolicy(Qt::CustomContextMenu);

  // Header settings
  header()->setSectionResizeMode(0, QHeaderView::Stretch);
  header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  header()->setSectionResizeMode(3, QHeaderView::Stretch);

  // Progress bar delegate
  setItemDelegateForColumn(2, new RenderJobProgressDelegate(this));
  setIconSize(QSize(64, 36));

  // Enable inline editing for job name column
  setEditTriggers(QAbstractItemView::DoubleClicked |
                  QAbstractItemView::SelectedClicked);

  // Selection
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::ExtendedSelection);

  // Connect context menu
  connect(this, &QTreeView::customContextMenuRequested, this,
          [this](const QPoint &pos) {
            QModelIndex idx = indexAt(pos);
            if (!idx.isValid())
              return;
            emit jobContextMenuRequested(idx.row(),
                                         viewport()->mapToGlobal(pos));
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

void RenderQueueManagerJobPanel::setService(QObject *service) {
  if (impl_->previewFrameReadyConnection_) {
    QObject::disconnect(impl_->previewFrameReadyConnection_);
    impl_->previewFrameReadyConnection_ = QMetaObject::Connection();
  }
  impl_->service_ = service;

  if (service) {
    impl_->previewFrameReadyConnection_ = QObject::connect(
        service, SIGNAL(previewFrameReady(int,int)), this,
        SLOT(handlePreviewFrameReady(int,int)));
  }
}

void RenderQueueManagerJobPanel::handlePreviewFrameReady(int jobIndex,
                                                         int /*frameNumber*/) {
  if (!impl_->service_) {
    return;
  }
  QImage preview;
  QMetaObject::invokeMethod(impl_->service_, "lastRenderedFrame",
                            Qt::DirectConnection,
                            Q_RETURN_ARG(QImage, preview));
  if (preview.isNull()) {
    return;
  }
  impl_->previewIcons_[jobIndex] = QPixmap::fromImage(preview);
  if (auto *model = this->model(); model && jobIndex >= 0) {
    const QModelIndex itemIndex = model->index(jobIndex, 0);
    if (itemIndex.isValid()) {
      model->setData(itemIndex, QIcon(impl_->previewIcons_.value(jobIndex)),
                     Qt::DecorationRole);
    }
  }
}

void RenderQueueManagerJobPanel::refreshJobList() {
  if (!impl_->service_)
    return;

  // Use Qt::QueuedConnection to safely call across module boundaries
  int jobCount = 0;
  QMetaObject::invokeMethod(impl_->service_, "jobCount", Qt::DirectConnection,
                            Q_RETURN_ARG(int, jobCount));

  auto *model = static_cast<RenderJobModel *>(this->model());
  if (!model)
    return;

  model->clearJobs();
  impl_->previewIcons_.clear();
  for (int i = 0; i < jobCount; ++i) {
    QString compName, status, outputPath;
    int progress = 0;

    QMetaObject::invokeMethod(impl_->service_, "jobCompositionNameAt",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(QString, compName), Q_ARG(int, i));
    QMetaObject::invokeMethod(impl_->service_, "jobStatusAt",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(QString, status), Q_ARG(int, i));
    QMetaObject::invokeMethod(impl_->service_, "jobProgressAt",
                              Qt::DirectConnection, Q_RETURN_ARG(int, progress),
                              Q_ARG(int, i));
    QMetaObject::invokeMethod(impl_->service_, "jobOutputPathAt",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(QString, outputPath), Q_ARG(int, i));

    model->addJob(compName, status, progress, outputPath);
    if (impl_->previewIcons_.contains(i)) {
      const QModelIndex itemIndex = model->index(i, 0);
      if (itemIndex.isValid()) {
        model->setData(itemIndex, QIcon(impl_->previewIcons_.value(i)),
                       Qt::DecorationRole);
      }
    }
  }
}

void RenderQueueManagerJobPanel::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex idx = indexAt(event->pos());
  if (!idx.isValid()) {
    QTreeView::contextMenuEvent(event);
    return;
  }

  int jobIndex = idx.row();
  emit jobContextMenuRequested(jobIndex, event->globalPos());
}

void RenderQueueManagerJobPanel::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasFormat("application/x-artifact-composition-id")) {
    event->acceptProposedAction();
    return;
  }
  if (event->source() == this &&
      event->mimeData()->hasFormat("application/x-render-queue-job")) {
    event->acceptProposedAction();
    return;
  }
  QTreeView::dragEnterEvent(event);
}

void RenderQueueManagerJobPanel::dragMoveEvent(QDragMoveEvent *event) {
  if (event->mimeData()->hasFormat("application/x-artifact-composition-id") ||
      event->mimeData()->hasFormat("application/x-render-queue-job")) {
    event->acceptProposedAction();
    return;
  }
  QTreeView::dragMoveEvent(event);
}

void RenderQueueManagerJobPanel::dropEvent(QDropEvent *event) {
  if (event->mimeData()->hasFormat("application/x-artifact-composition-id")) {
    QString compId =
        event->mimeData()->data("application/x-artifact-composition-id");
    emit compositionDropped(compId);
    event->acceptProposedAction();
    return;
  }
  if (event->mimeData()->hasFormat("application/x-render-queue-job")) {
    QByteArray data = event->mimeData()->data("application/x-render-queue-job");
    QDataStream stream(&data, QIODevice::ReadOnly);
    int fromIndex;
    stream >> fromIndex;

    const QPoint pos = event->position().toPoint();
    const int rowCount = model() ? model()->rowCount() : 0;
    int toIndex = rowCount;
    const QModelIndex hoverIndex = indexAt(pos);
    const int hoverRow = hoverIndex.isValid() ? hoverIndex.row() : -1;
    if (hoverRow >= 0) {
      const QRect hoverRect = visualRect(hoverIndex);
      toIndex = pos.y() > hoverRect.center().y() ? hoverRow + 1 : hoverRow;
    } else if (pos.y() < viewport()->height() / 2) {
      toIndex = 0;
    }
    toIndex = std::clamp(toIndex, 0, rowCount);
    if (fromIndex != toIndex) {
      emit jobDropped(fromIndex, toIndex);
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

  QMimeData *mimeData = new QMimeData();
  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);
  stream << selected.first().row();
  mimeData->setData("application/x-render-queue-job", data);

  QDrag *drag = new QDrag(this);
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
  RenderQueueControlPanel *controlPanel_ = nullptr;
  RenderQueueManagerJobPanel *jobPanel_ = nullptr;
  RenderQueueManagerUnderInfoWidget *infoWidget_ = nullptr;
  QObject *service_ = nullptr;
  QTimer *infoUpdateTimer_ = nullptr;
  QElapsedTimer renderTimer_;
  bool isRendering_ = false;
  int64_t renderStartTime_ = 0;
  QSystemTrayIcon *trayIcon_ = nullptr;
  int lastProgressBucket_ = -1;
  QMetaObject::Connection allJobsCompletedConnection_;
};

RenderQueueManagerWidget::Impl::Impl() {
  infoUpdateTimer_ = new QTimer();
  infoUpdateTimer_->setInterval(1000);
}

RenderQueueManagerWidget::Impl::~Impl() {
  delete trayIcon_;
  delete infoUpdateTimer_;
}

namespace {

QString formatHmsFromSeconds(qint64 seconds) {
  seconds = std::max<qint64>(0, seconds);
  const qint64 hours = seconds / 3600;
  const qint64 minutes = (seconds % 3600) / 60;
  const qint64 secs = seconds % 60;
  return QStringLiteral("%1:%2:%3")
      .arg(hours, 2, 10, QChar('0'))
      .arg(minutes, 2, 10, QChar('0'))
      .arg(secs, 2, 10, QChar('0'));
}

} // namespace

RenderQueueManagerWidget::RenderQueueManagerWidget(QWidget *parent)
    : QWidget(parent), impl_(new Impl()) {
  auto style = getDCCStyleSheetPreset(DccStylePreset::StudioStyle);
  setStyleSheet(style);

  impl_->controlPanel_ = new RenderQueueControlPanel();
  impl_->jobPanel_ = new RenderQueueManagerJobPanel();
  impl_->jobPanel_->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::Expanding);
  impl_->infoWidget_ = new RenderQueueManagerUnderInfoWidget();

  QVBoxLayout *layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(1);
  layout->addWidget(impl_->controlPanel_);
  layout->addWidget(impl_->jobPanel_, 1);
  layout->addWidget(impl_->infoWidget_);
  setLayout(layout);

  // Connect control panel signals
  connect(impl_->controlPanel_, &RenderQueueControlPanel::startRenderingClicked,
          this, [this]() {
            if (!impl_->service_)
              return;
            QMetaObject::invokeMethod(impl_->service_, "startAllJobs",
                                      Qt::QueuedConnection);
            impl_->isRendering_ = true;
            impl_->lastProgressBucket_ = -1;
            impl_->controlPanel_->setRenderingState(true);
            ::ArtifactWidgets::AppProgress::setProgress(this, 0, 100);
            notifyBackgroundRender("Render Queue", "Rendering started.");
            impl_->renderTimer_.start();
          });

  connect(impl_->controlPanel_, &RenderQueueControlPanel::clearAllClicked, this,
          [this]() {
            if (!impl_->service_)
              return;
            int count = 0;
            QMetaObject::invokeMethod(impl_->service_, "jobCount",
                                      Qt::DirectConnection,
                                      Q_RETURN_ARG(int, count));
            if (count == 0)
              return;

            if (QMessageBox::question(
                    this, "Clear All",
                    QString("Remove all %1 job(s) from the queue?").arg(count),
                    QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
              QMetaObject::invokeMethod(impl_->service_,
                                        "removeAllRenderQueues",
                                        Qt::QueuedConnection);
              impl_->jobPanel_->refreshJobList();
            }
          });

  connect(impl_->controlPanel_,
          &RenderQueueControlPanel::addAllCompositionsClicked, this,
          [this]() {
            if (!impl_->service_)
              return;
            if (QMessageBox::question(
                    this, "Add All Compositions",
                    QStringLiteral("Add every composition in the project to the render queue?"),
                    QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
              return;
            }
            int added = 0;
            QMetaObject::invokeMethod(impl_->service_, "addAllCompositions",
                                      Qt::DirectConnection,
                                      Q_RETURN_ARG(int, added));
            if (added > 0) {
              impl_->jobPanel_->refreshJobList();
            }
          });

  // Connect job panel signals
  connect(impl_->jobPanel_,
          &RenderQueueManagerJobPanel::jobContextMenuRequested, this,
          [this](int jobIndex, const QPoint &pos) {
            showJobContextMenu(jobIndex, pos);
          });

  connect(impl_->jobPanel_, &RenderQueueManagerJobPanel::jobDropped, this,
          [this](int from, int to) {
            if (!impl_->service_)
              return;
            QMetaObject::invokeMethod(impl_->service_, "moveRenderQueue",
                                      Qt::QueuedConnection, Q_ARG(int, from),
                                      Q_ARG(int, to));
            impl_->jobPanel_->refreshJobList();
          });

  connect(impl_->jobPanel_, &RenderQueueManagerJobPanel::compositionDropped,
          this, [this](const QString &compId) {
            if (!impl_->service_)
              return;
            QMetaObject::invokeMethod(
                impl_->service_, "addRenderQueueForComposition",
                Qt::QueuedConnection, Q_ARG(QString, compId));
            impl_->jobPanel_->refreshJobList();
          });

  // Info update timer
  connect(impl_->infoUpdateTimer_, &QTimer::timeout, this,
          [this]() { updateInfoPanel(); });
  impl_->infoUpdateTimer_->start();
}

RenderQueueManagerWidget::~RenderQueueManagerWidget() { delete impl_; }

QSize RenderQueueManagerWidget::sizeHint() const {
  return QSize(500, QWidget::sizeHint().height());
}

void RenderQueueManagerWidget::notifyBackgroundRender(const QString &title,
                                                      const QString &message) {
  if (!impl_) {
    return;
  }
  if (!impl_->trayIcon_) {
    auto *tray = new QSystemTrayIcon(this);
    QIcon icon = QApplication::windowIcon();
    if (icon.isNull()) {
      icon = QApplication::style()->standardIcon(QStyle::SP_ComputerIcon);
    }
    tray->setIcon(icon);
    tray->setVisible(true);
    impl_->trayIcon_ = tray;
  }
  if (impl_->trayIcon_ && QSystemTrayIcon::isSystemTrayAvailable()) {
    impl_->trayIcon_->showMessage(title, message, QSystemTrayIcon::Information,
                                  3000);
  } else {
    QApplication::alert(this, 3000);
  }
}

void RenderQueueManagerWidget::setFloatingMode(bool isFloating) {
  if (impl_->controlPanel_) {
    if (auto *progressBar = impl_->controlPanel_->progressBar()) {
      progressBar->setVisible(!isFloating);
    }
    if (auto *addAllButton = impl_->controlPanel_->addAllCompositionsButton()) {
      addAllButton->setVisible(!isFloating);
    }
    if (auto *startButton = impl_->controlPanel_->renderingStartButton()) {
      startButton->setVisible(!isFloating);
    }
    if (auto *clearButton = impl_->controlPanel_->clearAllButton()) {
      clearButton->setVisible(!isFloating);
    }
  }
}

void RenderQueueManagerWidget::setService(QObject *service) {
  if (impl_->allJobsCompletedConnection_) {
    QObject::disconnect(impl_->allJobsCompletedConnection_);
    impl_->allJobsCompletedConnection_ = QMetaObject::Connection();
  }
  impl_->service_ = service;
  impl_->jobPanel_->setService(service);
  impl_->jobPanel_->refreshJobList();

  if (service) {
    impl_->allJobsCompletedConnection_ = QObject::connect(
        service, SIGNAL(allJobsCompleted()), this,
        SLOT(handleAllJobsCompleted()));
  }
}

void RenderQueueManagerWidget::handleAllJobsCompleted() {
  impl_->isRendering_ = false;
  if (impl_->controlPanel_) {
    impl_->controlPanel_->setRenderingState(false);
  }
  ::ArtifactWidgets::AppProgress::clear(this);
  QObject *service = impl_->service_;
  int failedCount = 0;
  int jobCount = 0;
  QMetaObject::invokeMethod(service, "jobCount", Qt::DirectConnection,
                            Q_RETURN_ARG(int, jobCount));
  for (int i = 0; i < jobCount; ++i) {
    QString status;
    QMetaObject::invokeMethod(service, "jobStatusAt", Qt::DirectConnection,
                              Q_RETURN_ARG(QString, status), Q_ARG(int, i));
    if (status == "Failed" || status == "Canceled") {
      ++failedCount;
    }
  }
  if (impl_->controlPanel_ &&
      impl_->controlPanel_->autoOpenOutputFolderCheckBox() &&
      impl_->controlPanel_->autoOpenOutputFolderCheckBox()->isChecked()) {
    QString outputPath;
    for (int i = 0; i < jobCount; ++i) {
      QMetaObject::invokeMethod(service, "jobOutputPathAt",
                                Qt::DirectConnection,
                                Q_RETURN_ARG(QString, outputPath),
                                Q_ARG(int, i));
      if (!outputPath.trimmed().isEmpty()) {
        break;
      }
    }
    if (!outputPath.trimmed().isEmpty()) {
      QDesktopServices::openUrl(
          QUrl::fromLocalFile(QFileInfo(outputPath).absolutePath()));
    }
  }
  notifyBackgroundRender(
      "Render Queue",
      failedCount > 0
          ? QStringLiteral("Rendering finished with %1 failed job(s).")
                .arg(failedCount)
          : QStringLiteral("Rendering finished successfully."));
  updateInfoPanel();
}

void RenderQueueManagerWidget::showJobContextMenu(int jobIndex,
                                                  const QPoint &pos) {
  if (!impl_->service_)
    return;
  QObject *queueService = impl_->service_;

  QMenu menu(this);
  QPalette menuPalette = menu.palette();
  menuPalette.setColor(QPalette::Window, QColor(34, 34, 34));
  menuPalette.setColor(QPalette::Base, QColor(34, 34, 34));
  menuPalette.setColor(QPalette::Text, QColor(245, 245, 245));
  menuPalette.setColor(QPalette::ButtonText, QColor(245, 245, 245));
  menuPalette.setColor(QPalette::Highlight, QColor(52, 152, 219));
  menuPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
  menuPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(140, 140, 140));
  menuPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(140, 140, 140));
  menu.setPalette(menuPalette);

  // Get job status
  QString status;
  QMetaObject::invokeMethod(impl_->service_, "jobStatusAt",
                            Qt::DirectConnection, Q_RETURN_ARG(QString, status),
                            Q_ARG(int, jobIndex));

  QAction *renameAction = menu.addAction("✏️ Rename...");
  QAction *dupAction = menu.addAction("📋 Duplicate");
  menu.addSeparator();

  QAction *outputAction = menu.addAction("📁 Open Output Folder");
  QAction *settingsAction = menu.addAction("⚙️ Output Settings...");
  menu.addSeparator();

  QAction *pngSequenceAction = menu.addAction("🎞 Use PNG Sequence");
  menu.addSeparator();

  QAction *copySettingsAction = nullptr;
  const QModelIndexList selectedRows =
      impl_->jobPanel_ ? impl_->jobPanel_->selectionModel()->selectedRows()
                       : QModelIndexList();
  if (queueService && selectedRows.size() > 1) {
    copySettingsAction = menu.addAction("📋 Copy Settings to Selected");
  }

  QAction *errorDetailsAction = nullptr;
  if (status == "Failed" || status == "Canceled") {
    errorDetailsAction = menu.addAction("🧾 Error Details...");
  }

  QAction *resetAction = nullptr;
  if (status == "Failed" || status == "Completed") {
    resetAction = menu.addAction("🔄 Reset & Re-render");
  }

  QAction *removeAction = menu.addAction("🗑 Remove");

  QAction *chosen = menu.exec(pos);
  if (!chosen)
    return;

  if (chosen == renameAction) {
    // Inline rename via input dialog
    QString currentName;
    QMetaObject::invokeMethod(
        impl_->service_, "jobNameAt", Qt::DirectConnection,
        Q_RETURN_ARG(QString, currentName), Q_ARG(int, jobIndex));
    bool ok = false;
    QString newName = QInputDialog::getText(
        this, "Rename Job", "Job name:", QLineEdit::Normal,
        currentName.isEmpty() ? QString("Job %1").arg(jobIndex + 1)
                              : currentName,
        &ok);
    if (ok && !newName.trimmed().isEmpty()) {
      QMetaObject::invokeMethod(impl_->service_, "setJobNameAt",
                                Qt::QueuedConnection, Q_ARG(int, jobIndex),
                                Q_ARG(QString, newName.trimmed()));
      impl_->jobPanel_->refreshJobList();
    }
  } else if (chosen == dupAction) {
    QMetaObject::invokeMethod(impl_->service_, "duplicateRenderQueueAt",
                              Qt::QueuedConnection, Q_ARG(int, jobIndex));
    impl_->jobPanel_->refreshJobList();
  } else if (chosen == outputAction) {
    QString outputPath;
    QMetaObject::invokeMethod(
        impl_->service_, "jobOutputPathAt", Qt::DirectConnection,
        Q_RETURN_ARG(QString, outputPath), Q_ARG(int, jobIndex));
    if (!outputPath.isEmpty()) {
      QFileInfo fi(outputPath);
      QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absolutePath()));
    }
  } else if (chosen == settingsAction) {
    QMessageBox::information(this, "Output Settings",
                             "Output settings dialog coming soon.");
  } else if (chosen == pngSequenceAction) {
    const int targetCount = selectedRows.size() > 1 ? selectedRows.size() : 1;
    if (QMessageBox::question(
            this, "Image Sequence",
            QStringLiteral("Convert %1 job(s) to PNG image sequence?").arg(targetCount),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
      int updated = 0;
      const auto applyToIndex = [queueService](int target) {
        int width = 0;
        int height = 0;
        double fps = 0.0;
        int bitrateKbps = 0;
        QString *outputFormatPtr = nullptr;
        QString *codecPtr = nullptr;
        QString *codecProfilePtr = nullptr;
        bool ok = false;
        QMetaObject::invokeMethod(
            queueService, "jobOutputSettingsAt", Qt::DirectConnection,
            Q_RETURN_ARG(bool, ok), Q_ARG(int, target),
            Q_ARG(QString *, outputFormatPtr), Q_ARG(QString *, codecPtr),
            Q_ARG(QString *, codecProfilePtr),
            Q_ARG(int *, &width), Q_ARG(int *, &height), Q_ARG(double *, &fps),
            Q_ARG(int *, &bitrateKbps));
        if (!ok) {
          return;
        }
        QMetaObject::invokeMethod(
            queueService, "setJobOutputSettingsAt", Qt::DirectConnection,
            Q_ARG(int, target),
            Q_ARG(QString, QStringLiteral("PNG Sequence")),
            Q_ARG(QString, QStringLiteral("PNG")), Q_ARG(QString, QString()),
            Q_ARG(int, width), Q_ARG(int, height), Q_ARG(double, fps),
            Q_ARG(int, bitrateKbps));
        QMetaObject::invokeMethod(queueService, "setJobOutputPathAt",
                                  Qt::DirectConnection, Q_ARG(int, target),
                                  Q_ARG(QString, QString()));
      };
      if (selectedRows.size() > 1) {
        for (const QModelIndex &idx : selectedRows) {
          if (!idx.isValid()) {
            continue;
          }
          applyToIndex(idx.row());
          ++updated;
        }
      } else {
        applyToIndex(jobIndex);
        updated = 1;
      }
      if (updated > 0) {
        impl_->jobPanel_->refreshJobList();
      }
    }
  } else if (chosen == copySettingsAction) {
    QString outputFormat;
    QString codec;
    QString codecProfile;
    int width = 0;
    int height = 0;
    double fps = 0.0;
    int bitrateKbps = 0;
    int startFrame = 0;
    int endFrame = 0;
    bool integratedRenderEnabled = false;
    QString audioSourcePath;
    QString audioCodec;
    int audioBitrateKbps = 0;
    QString renderBackend;
    QString encoderBackend;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float scale = 1.0f;
    float rotationDeg = 0.0f;

    if (!queueService) {
      return;
    }
    {
      bool ok = false;
      QMetaObject::invokeMethod(
          queueService, "jobOutputSettingsAt", Qt::DirectConnection,
          Q_RETURN_ARG(bool, ok), Q_ARG(int, jobIndex),
          Q_ARG(QString *, &outputFormat), Q_ARG(QString *, &codec),
          Q_ARG(QString *, &codecProfile), Q_ARG(int *, &width),
          Q_ARG(int *, &height), Q_ARG(double *, &fps),
          Q_ARG(int *, &bitrateKbps));
      QMetaObject::invokeMethod(queueService, "jobFrameRangeAt",
                                Qt::DirectConnection, Q_RETURN_ARG(bool, ok),
                                Q_ARG(int, jobIndex),
                                Q_ARG(int *, &startFrame),
                                Q_ARG(int *, &endFrame));
      integratedRenderEnabled =
          false;
      QMetaObject::invokeMethod(
          queueService, "jobIntegratedRenderEnabledAt", Qt::DirectConnection,
          Q_RETURN_ARG(bool, integratedRenderEnabled), Q_ARG(int, jobIndex));
      QMetaObject::invokeMethod(
          queueService, "jobAudioSourcePathAt", Qt::DirectConnection,
          Q_RETURN_ARG(QString, audioSourcePath), Q_ARG(int, jobIndex));
      QMetaObject::invokeMethod(queueService, "jobAudioCodecAt",
                                Qt::DirectConnection,
                                Q_RETURN_ARG(QString, audioCodec),
                                Q_ARG(int, jobIndex));
      QMetaObject::invokeMethod(
          queueService, "jobAudioBitrateKbpsAt", Qt::DirectConnection,
          Q_RETURN_ARG(int, audioBitrateKbps), Q_ARG(int, jobIndex));
      QMetaObject::invokeMethod(queueService, "jobRenderBackendAt",
                                Qt::DirectConnection,
                                Q_RETURN_ARG(QString, renderBackend),
                                Q_ARG(int, jobIndex));
      QMetaObject::invokeMethod(queueService, "jobEncoderBackendAt",
                                Qt::DirectConnection,
                                Q_RETURN_ARG(QString, encoderBackend),
                                Q_ARG(int, jobIndex));
      QMetaObject::invokeMethod(
          queueService, "jobOverlayTransformAt", Qt::DirectConnection,
          Q_RETURN_ARG(bool, ok), Q_ARG(int, jobIndex),
          Q_ARG(float *, &offsetX), Q_ARG(float *, &offsetY),
          Q_ARG(float *, &scale), Q_ARG(float *, &rotationDeg));
    }

    const QString questionText = QStringLiteral(
        "Copy render settings from job %1 to %2 selected job(s)?")
                                    .arg(jobIndex + 1)
                                    .arg(selectedRows.size() - 1);
    if (QMessageBox::question(this, "Copy Settings", questionText,
                              QMessageBox::Yes | QMessageBox::No) ==
        QMessageBox::Yes) {
      int copied = 0;
      for (const QModelIndex &idx : selectedRows) {
        if (!idx.isValid() || idx.row() == jobIndex) {
          continue;
        }
        const int target = idx.row();
        QMetaObject::invokeMethod(
            queueService, "setJobOutputSettingsAt", Qt::DirectConnection,
            Q_ARG(int, target), Q_ARG(QString, outputFormat),
            Q_ARG(QString, codec), Q_ARG(QString, codecProfile),
            Q_ARG(int, width), Q_ARG(int, height), Q_ARG(double, fps),
            Q_ARG(int, bitrateKbps));
        QMetaObject::invokeMethod(queueService, "setJobFrameRangeAt",
                                  Qt::DirectConnection, Q_ARG(int, target),
                                  Q_ARG(int, startFrame),
                                  Q_ARG(int, endFrame));
        QMetaObject::invokeMethod(
            queueService, "setJobIntegratedRenderEnabledAt",
            Qt::DirectConnection, Q_ARG(int, target),
            Q_ARG(bool, integratedRenderEnabled));
        QMetaObject::invokeMethod(queueService, "setJobAudioSourcePathAt",
                                  Qt::DirectConnection, Q_ARG(int, target),
                                  Q_ARG(QString, audioSourcePath));
        QMetaObject::invokeMethod(queueService, "setJobAudioCodecAt",
                                  Qt::DirectConnection, Q_ARG(int, target),
                                  Q_ARG(QString, audioCodec));
        QMetaObject::invokeMethod(queueService,
                                  "setJobAudioBitrateKbpsAt",
                                  Qt::DirectConnection, Q_ARG(int, target),
                                  Q_ARG(int, audioBitrateKbps));
        QMetaObject::invokeMethod(queueService, "setJobRenderBackendAt",
                                  Qt::DirectConnection, Q_ARG(int, target),
                                  Q_ARG(QString, renderBackend));
        QMetaObject::invokeMethod(queueService, "setJobEncoderBackendAt",
                                  Qt::DirectConnection, Q_ARG(int, target),
                                  Q_ARG(QString, encoderBackend));
        QMetaObject::invokeMethod(queueService, "setJobOverlayTransform",
                                  Qt::DirectConnection, Q_ARG(int, target),
                                  Q_ARG(float, offsetX), Q_ARG(float, offsetY),
                                  Q_ARG(float, scale),
                                  Q_ARG(float, rotationDeg));
        ++copied;
      }
      if (copied > 0) {
        impl_->jobPanel_->refreshJobList();
      }
    }
  } else if (chosen == errorDetailsAction) {
    QString errorMessage;
    QMetaObject::invokeMethod(
        impl_->service_, "jobErrorMessageAt", Qt::DirectConnection,
        Q_RETURN_ARG(QString, errorMessage), Q_ARG(int, jobIndex));

    QMessageBox box(this);
    box.setWindowTitle(QStringLiteral("Render Error Details"));
    box.setIcon(QMessageBox::Warning);
    box.setText(QStringLiteral("Job %1").arg(jobIndex + 1));
    box.setInformativeText(
        errorMessage.isEmpty()
            ? QStringLiteral("No detailed error message is available.")
            : QStringLiteral("The render job reported an error."));
    box.setDetailedText(errorMessage.isEmpty()
                            ? QStringLiteral("(empty)")
                            : errorMessage);
    box.exec();
  } else if (chosen == resetAction) {
    QMetaObject::invokeMethod(impl_->service_, "resetJobForRerun",
                              Qt::QueuedConnection, Q_ARG(int, jobIndex));
    impl_->jobPanel_->refreshJobList();
  } else if (chosen == removeAction) {
    QMetaObject::invokeMethod(impl_->service_, "removeRenderQueueAt",
                              Qt::QueuedConnection, Q_ARG(int, jobIndex));
    impl_->jobPanel_->refreshJobList();
  }
}

void RenderQueueManagerWidget::updateInfoPanel() {
  if (!impl_->service_)
    return;

  // RAM usage
  int ramMB = 0;
  QMetaObject::invokeMethod(impl_->service_, "getCurrentRamUsageMB",
                            Qt::DirectConnection, Q_RETURN_ARG(int, ramMB));

  // Elapsed time
  QString elapsed = "--:--:--";
  if (impl_->isRendering_ && impl_->renderTimer_.isValid()) {
    elapsed = formatHmsFromSeconds(impl_->renderTimer_.elapsed() / 1000);
  }

  int totalProgress = 0;
  QMetaObject::invokeMethod(impl_->service_, "getTotalProgress",
                            Qt::DirectConnection,
                            Q_RETURN_ARG(int, totalProgress));
  impl_->controlPanel_->setTotalProgress(totalProgress);
  if (impl_->isRendering_) {
    const int bucket = std::clamp(totalProgress / 25, 0, 4);
    if (bucket != impl_->lastProgressBucket_ && totalProgress > 0) {
      impl_->lastProgressBucket_ = bucket;
      notifyBackgroundRender(
          "Render Queue",
          QStringLiteral("Rendering progress: %1%").arg(totalProgress));
    }
  }
  if (impl_->isRendering_) {
          ::ArtifactWidgets::AppProgress::setProgress(this, totalProgress, 100);
  } else {
      ::ArtifactWidgets::AppProgress::clear(this);
  }

  QString eta = QStringLiteral("--:--:--");
  if (impl_->isRendering_ && impl_->renderTimer_.isValid()) {
    if (totalProgress >= 100) {
      eta = QStringLiteral("00:00:00");
    } else if (totalProgress > 0) {
      const qint64 elapsedSecs = std::max<qint64>(1, impl_->renderTimer_.elapsed() / 1000);
      const qint64 remainingSecs =
          (elapsedSecs * (100 - totalProgress)) / std::max(1, totalProgress);
      eta = formatHmsFromSeconds(remainingSecs);
    }
  }

  // Log (last status message)
  QString log = "Idle";
  QMetaObject::invokeMethod(impl_->service_, "getLastStatusMessage",
                            Qt::DirectConnection, Q_RETURN_ARG(QString, log));

  impl_->infoWidget_->updateInfo(log, ramMB, elapsed, eta);
}

// ─────────────────────────────────────────────────────────
// RenderQueueManagerUnderInfoWidget
// ─────────────────────────────────────────────────────────

class RenderQueueManagerUnderInfoWidget::Impl {
public:
  Impl();
  ~Impl();
  QLabel *logLabel = nullptr;
  QLabel *ramUsageLabel = nullptr;
  QLabel *elapsedTimeLabel = nullptr;
};

RenderQueueManagerUnderInfoWidget::Impl::Impl() {}
RenderQueueManagerUnderInfoWidget::Impl::~Impl() {}

RenderQueueManagerUnderInfoWidget::RenderQueueManagerUnderInfoWidget(
    QWidget *parent)
    : QWidget(parent), impl_(new Impl()) {
  QHBoxLayout *layout = new QHBoxLayout();
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

RenderQueueManagerUnderInfoWidget::~RenderQueueManagerUnderInfoWidget() {
  delete impl_;
}

void RenderQueueManagerUnderInfoWidget::updateInfo(const QString &log,
                                                   int ramMB,
                                                   const QString &elapsedTime) {
  updateInfo(log, ramMB, elapsedTime, QStringLiteral("--:--:--"));
}

void RenderQueueManagerUnderInfoWidget::updateInfo(const QString &log,
                                                   int ramMB,
                                                   const QString &elapsedTime,
                                                   const QString &etaTime) {
  impl_->logLabel->setText(
      QString("Log: %1").arg(log.isEmpty() ? "Idle" : log));
  impl_->ramUsageLabel->setText(QString("RAM: %1 MB").arg(ramMB));
  impl_->elapsedTimeLabel->setText(
      QString("Elapsed: %1 | ETA: %2").arg(elapsedTime, etaTime));
}

}; // namespace ArtifactWidgets
