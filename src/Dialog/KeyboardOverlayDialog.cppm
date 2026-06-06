module;
#include <wobjectimpl.h>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
#include <QTimer>
#include <QApplication>
#include <QScreen>
#include <QPainter>
#include <QPainterPath>
#include <QScrollArea>
#include <QFrame>
#include <vector>
#include <set>

module Widgets.KeyboardOverlayDialog;

import Input.Operator;

namespace ArtifactWidgets
{
 class KeyboardOverlayDialog::Impl
 {
private:

 public:
  Impl(KeyboardOverlayDialog* owner);
  ~Impl() = default;

  KeyboardOverlayDialog* owner_;
  QTableWidget* table_;
  QLineEdit* searchBox_;
  QFrame* keyboardPreview_;
  QScrollArea* tableScroll_;
  bool isCompact_ = false;

  void reloadShortcuts();
  void rebuildKeyboardPreview();
 };

 KeyboardOverlayDialog::Impl::Impl(KeyboardOverlayDialog* owner) : owner_(owner) {}

KeyboardOverlayDialog::KeyboardOverlayDialog(QWidget* parent) : QDialog(parent), impl_(new Impl(this))
 {
  setWindowTitle("Keyboard Shortcuts");
  resize(800, 600);

  setAttribute(Qt::WA_TranslucentBackground, true);
  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
  setObjectName(QStringLiteral("KeyboardOverlayDialog"));

  auto* outerLayout = new QVBoxLayout(this);
  outerLayout->setContentsMargins(18, 18, 18, 18);
  outerLayout->setSpacing(12);

  auto* shell = new QWidget(this);
  shell->setObjectName(QStringLiteral("KeyboardOverlayShell"));
  shell->setAutoFillBackground(false);
  auto* shellLayout = new QVBoxLayout(shell);
  shellLayout->setContentsMargins(18, 18, 18, 18);
  shellLayout->setSpacing(12);

  // Search Bar
  auto* searchLayout = new QHBoxLayout();
  auto* searchIcon = new QLabel(QStringLiteral("⌕"));
  impl_->searchBox_ = new QLineEdit();
  impl_->searchBox_->setPlaceholderText("Search shortcuts or actions...");
  impl_->searchBox_->setObjectName(QStringLiteral("KeyboardOverlaySearch"));
  searchLayout->addWidget(searchIcon);
  searchLayout->addWidget(impl_->searchBox_);
  shellLayout->addLayout(searchLayout);

  impl_->keyboardPreview_ = new QFrame(shell);
  impl_->keyboardPreview_->setMinimumHeight(220);
  impl_->keyboardPreview_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  impl_->keyboardPreview_->setObjectName(QStringLiteral("KeyboardOverlayPreview"));
  impl_->keyboardPreview_->setFrameStyle(QFrame::Box | QFrame::Plain);
  impl_->keyboardPreview_->setLineWidth(1);
  shellLayout->addWidget(impl_->keyboardPreview_);

  // Table
  impl_->tableScroll_ = new QScrollArea(shell);
  impl_->tableScroll_->setWidgetResizable(true);
  impl_->tableScroll_->setFrameShape(QFrame::NoFrame);
  impl_->tableScroll_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  impl_->tableScroll_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  impl_->table_ = new QTableWidget();
  impl_->table_->setColumnCount(5);
  impl_->table_->setHorizontalHeaderLabels({"Context", "KeyMap", "Category", "Action", "Shortcut"});
  impl_->table_->horizontalHeader()->setStretchLastSection(true);
  impl_->table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  impl_->table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  impl_->table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  impl_->table_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  impl_->table_->verticalHeader()->setVisible(false);
  impl_->table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
  impl_->table_->setSelectionBehavior(QAbstractItemView::SelectRows);
  impl_->table_->setSelectionMode(QAbstractItemView::SingleSelection);
  impl_->table_->setShowGrid(false);
  impl_->tableScroll_->setWidget(impl_->table_);
  shellLayout->addWidget(impl_->tableScroll_);
  outerLayout->addWidget(shell);

  connect(impl_->searchBox_, &QLineEdit::textChanged, this, [this](const QString& text) {
      for (int i = 0; i < impl_->table_->rowCount(); ++i) {
          bool match = false;
          for (int j = 0; j < impl_->table_->columnCount(); ++j) {
              auto* item = impl_->table_->item(i, j);
              if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                  match = true;
                  break;
              }
          }
          impl_->table_->setRowHidden(i, !match);
      }
      impl_->rebuildKeyboardPreview();
  });

  setOverlayOpacity(0.9f); // Default
  impl_->reloadShortcuts();
  impl_->rebuildKeyboardPreview();
 }

 KeyboardOverlayDialog::~KeyboardOverlayDialog()
 {
     delete impl_;
 }

 void KeyboardOverlayDialog::setCompactMode(bool enabled)
 {
     impl_->isCompact_ = enabled;
     if (enabled) {
         if (layout()) layout()->setContentsMargins(10, 10, 10, 10);
         resize(600, 400);
     } else {
         if (layout()) layout()->setContentsMargins(18, 18, 18, 18);
         resize(800, 600);
     }
 }

 void KeyboardOverlayDialog::setOverlayOpacity(float opacity)
 {
     setWindowOpacity(opacity);
     QPalette palette = this->palette();
     palette.setColor(QPalette::Window, QColor(18, 20, 24, int(opacity * 255)));
     palette.setColor(QPalette::Base, QColor(24, 27, 32, int(opacity * 255)));
     palette.setColor(QPalette::Text, QColor(235, 240, 245));
     palette.setColor(QPalette::WindowText, QColor(235, 240, 245));
     setPalette(palette);
     setAutoFillBackground(true);
 }

 void KeyboardOverlayDialog::setAlwaysOnTop(bool enabled)
 {
     if (enabled) {
         setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
     } else {
         setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
     }
 }

 void KeyboardOverlayDialog::showCentered()
 {
     if (parentWidget() && parentWidget()->window()) {
         const auto rect = parentWidget()->window()->geometry();
         move(rect.center() - rect.center() / 2);
     } else if (auto* screen = QGuiApplication::primaryScreen()) {
         const auto rect = screen->geometry();
         move(rect.center() - rect.center() / 2);
     }
     show();
     impl_->searchBox_->setFocus();
     impl_->searchBox_->selectAll();
 }

 void KeyboardOverlayDialog::Impl::reloadShortcuts()
 {
     table_->setRowCount(0);
     auto* am = ArtifactCore::ActionManager::instance();
     auto* input = ArtifactCore::InputOperator::instance();
     if (!am || !input) return;

     std::vector<ShortcutEntry> entries;
     
     // Helper to grab bound action IDs to avoid showing them as "Unassigned" later
     std::set<QString> boundActionIds;

     for (auto* keyMap : input->allKeyMaps()) {
         if (!keyMap) continue;
         for (auto* binding : keyMap->allBindings()) {
             if (!binding) continue;
             ShortcutEntry entry;
             entry.context = keyMap->context();
             entry.keymap = keyMap->name();
             entry.key = binding->toString();
             entry.action = binding->name().isEmpty() ? binding->actionId() : binding->name();
             entry.section = QStringLiteral("General");
             if (auto* action = am->getAction(binding->actionId())) {
                 entry.action = action->label();
                 entry.category = action->category();
                 const QString actionId = binding->actionId();
                 if (actionId.contains(QStringLiteral("play"), Qt::CaseInsensitive) ||
                     actionId.contains(QStringLiteral("transport"), Qt::CaseInsensitive)) {
                     entry.section = QStringLiteral("Playback");
                 } else if (actionId.contains(QStringLiteral("tool"), Qt::CaseInsensitive) ||
                            actionId.contains(QStringLiteral("select"), Qt::CaseInsensitive) ||
                            actionId.contains(QStringLiteral("move"), Qt::CaseInsensitive)) {
                     entry.section = QStringLiteral("Editing");
                 }
             } else {
                 entry.category = keyMap->name();
             }
             boundActionIds.insert(binding->actionId());
             entries.push_back(entry);
         }
     }

     for (auto* action : am->allActions()) {
         if (!action || boundActionIds.contains(action->id())) continue;
         ShortcutEntry entry;
         entry.context = QStringLiteral("Global");
         entry.keymap = QStringLiteral("---");
         entry.action = action->label();
         entry.category = action->category();
         entry.section = QStringLiteral("General");
         entries.push_back(entry);
     }

     std::sort(entries.begin(), entries.end(), [](const ShortcutEntry& a, const ShortcutEntry& b) {
         if (a.section != b.section) return a.section < b.section;
         if (a.category != b.category) return a.category < b.category;
         return a.action < b.action;
     });

     table_->setRowCount(entries.size());
     for (int i = 0; i < entries.size(); ++i) {
         auto* ctxItem = new QTableWidgetItem(entries[i].context.isEmpty() ? QStringLiteral("Global") : entries[i].context);
         auto* mapItem = new QTableWidgetItem(entries[i].keymap);
         auto* catItem = new QTableWidgetItem(entries[i].category);
         auto* actItem = new QTableWidgetItem(entries[i].action);
         auto* keyItem = new QTableWidgetItem(entries[i].key);
         
         // Style shortcuts like a badge
         keyItem->setForeground(QBrush(QColor("#7BD0FF")));
         QFont font = keyItem->font();
         font.setBold(true);
         keyItem->setFont(font);

         table_->setItem(i, 0, ctxItem);
         table_->setItem(i, 1, mapItem);
         table_->setItem(i, 2, catItem);
         table_->setItem(i, 3, actItem);
         table_->setItem(i, 4, keyItem);
     }
     table_->resizeColumnsToContents();
 }

 void KeyboardOverlayDialog::Impl::rebuildKeyboardPreview()
 {
     if (!keyboardPreview_) return;
     auto* text = keyboardPreview_->findChild<QLabel*>(QString(), Qt::FindDirectChildrenOnly);
     if (!text) {
         text = new QLabel(keyboardPreview_);
         text->setAlignment(Qt::AlignCenter);
         text->setWordWrap(true);
         text->setObjectName(QStringLiteral("KeyboardOverlayPreviewText"));
         text->setGeometry(keyboardPreview_->rect().adjusted(16, 16, -16, -16));
     }

     int visibleCount = 0;
     QStringList categories;
     for (int row = 0; row < table_->rowCount(); ++row) {
         if (table_->isRowHidden(row)) continue;
         ++visibleCount;
         const auto* item = table_->item(row, 2);
         if (item && !categories.contains(item->text())) {
             categories.push_back(item->text());
         }
     }

     text->setText(QStringLiteral("Keyboard wireframe overlay\n%1 visible shortcuts\n%2")
         .arg(visibleCount)
         .arg(categories.isEmpty() ? QStringLiteral("No category matches") : categories.join(QStringLiteral("  •  "))));
     QPalette textPalette = text->palette();
     textPalette.setColor(QPalette::WindowText, QColor(217, 241, 255));
     text->setPalette(textPalette);
     QFont textFont = text->font();
     textFont.setBold(true);
     text->setFont(textFont);
     keyboardPreview_->update();
 }

}
