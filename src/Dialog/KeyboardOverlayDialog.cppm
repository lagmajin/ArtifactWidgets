module;
#include <wobjectimpl.h>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
#include <QTimer>
#include <QApplication>
#include <QScreen>
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
  bool isCompact_ = false;

  void reloadShortcuts();
 };

 KeyboardOverlayDialog::Impl::Impl(KeyboardOverlayDialog* owner) : owner_(owner) {}

 KeyboardOverlayDialog::KeyboardOverlayDialog(QWidget* parent) : QDialog(parent), impl_(new Impl(this))
 {
  setWindowTitle("Keyboard Shortcuts");
  resize(800, 600);
  
  auto* layout = new QVBoxLayout(this);
  layout->setContentsMargins(16, 16, 16, 16);
  layout->setSpacing(8);

  // Search Bar
  auto* searchLayout = new QHBoxLayout();
  auto* searchIcon = new QLabel("🔍");
  impl_->searchBox_ = new QLineEdit();
  impl_->searchBox_->setPlaceholderText("Search shortcuts or actions...");
  impl_->searchBox_->setStyleSheet("QLineEdit { background: rgba(50, 50, 50, 180); color: white; border: 1px solid #555; padding: 4px; border-radius: 4px; }");
  searchLayout->addWidget(searchIcon);
  searchLayout->addWidget(impl_->searchBox_);
  layout->addLayout(searchLayout);

  // Table
  impl_->table_ = new QTableWidget();
  impl_->table_->setColumnCount(3);
  impl_->table_->setHorizontalHeaderLabels({"Category", "Action", "Shortcut"});
  impl_->table_->horizontalHeader()->setStretchLastSection(true);
  impl_->table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  impl_->table_->verticalHeader()->setVisible(false);
  impl_->table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
  impl_->table_->setSelectionBehavior(QAbstractItemView::SelectRows);
  impl_->table_->setSelectionMode(QAbstractItemView::SingleSelection);
  impl_->table_->setShowGrid(false);
  
  impl_->table_->setStyleSheet(R"(
    QTableWidget {
        background-color: transparent;
        color: #E0E0E0;
        border: none;
    }
    QTableWidget::item {
        padding: 4px;
        border-bottom: 1px solid rgba(255, 255, 255, 10);
    }
    QTableWidget::item:selected {
        background-color: rgba(60, 120, 200, 80);
    }
    QHeaderView::section {
        background-color: rgba(40, 40, 40, 200);
        color: #888;
        padding: 4px;
        border: none;
        border-bottom: 2px solid #555;
    }
  )");
  layout->addWidget(impl_->table_);

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
  });

  setOverlayOpacity(0.9f); // Default
  impl_->reloadShortcuts();
 }

 KeyboardOverlayDialog::~KeyboardOverlayDialog()
 {
     delete impl_;
 }

 void KeyboardOverlayDialog::setCompactMode(bool enabled)
 {
     impl_->isCompact_ = enabled;
     if (enabled) {
         impl_->table_->setStyleSheet(impl_->table_->styleSheet() + "QTableWidget::item { padding: 2px; font-size: 11px; }");
         layout()->setContentsMargins(8, 8, 8, 8);
         resize(600, 400);
     } else {
         impl_->table_->setStyleSheet(impl_->table_->styleSheet().replace("QTableWidget::item { padding: 2px; font-size: 11px; }", ""));
         layout()->setContentsMargins(16, 16, 16, 16);
         resize(800, 600);
     }
 }

 void KeyboardOverlayDialog::setOverlayOpacity(float opacity)
 {
     setWindowOpacity(opacity);
     setStyleSheet(QString("QDialog { background-color: rgba(30, 30, 30, %1); }").arg(int(opacity * 255)));
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
             entry.key = binding->toString();
             entry.action = binding->name().isEmpty() ? binding->actionId() : binding->name();
             if (auto* action = am->getAction(binding->actionId())) {
                 entry.action = action->label();
                 entry.category = action->category();
             } else {
                 entry.category = keyMap->name();
             }
             boundActionIds.insert(binding->actionId());
             entries.push_back(entry);
         }
     }

     for (auto* action : am->allActions()) {
         if (!action || boundActionIds.contains(action->id())) continue;
         entries.push_back({action->label(), "---", action->category()});
     }

     std::sort(entries.begin(), entries.end(), [](const ShortcutEntry& a, const ShortcutEntry& b) {
         if (a.category != b.category) return a.category < b.category;
         return a.action < b.action;
     });

     table_->setRowCount(entries.size());
     for (int i = 0; i < entries.size(); ++i) {
         auto* catItem = new QTableWidgetItem(entries[i].category);
         auto* actItem = new QTableWidgetItem(entries[i].action);
         auto* keyItem = new QTableWidgetItem(entries[i].key);
         
         // Style shortcuts like a badge
         keyItem->setForeground(QBrush(QColor("#7BD0FF")));
         QFont font = keyItem->font();
         font.setBold(true);
         keyItem->setFont(font);

         table_->setItem(i, 0, catItem);
         table_->setItem(i, 1, actItem);
         table_->setItem(i, 2, keyItem);
     }
     table_->resizeColumnsToContents();
 }

}
