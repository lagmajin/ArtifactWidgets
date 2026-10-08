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
#include <QKeySequence>
#include <QFont>
#include <QPalette>
#include <QColor>
#include <QStringList>
#include <QPoint>
#include <QSizePolicy>
#include <QBrush>
#include <algorithm>
#include <vector>
#include <set>

module Widgets.KeyboardOverlayDialog;

import Input.Operator;

namespace ArtifactWidgets
{
 // Standard button activation covers mouse, keyboard and accessibility without
 // introducing another signal/slot connection.
 class OverlayCloseButton final : public QPushButton {
 public:
  explicit OverlayCloseButton(QDialog* dialog) : QPushButton(QStringLiteral("×"), dialog), dialog_(dialog) {
   setAccessibleName(QObject::tr("Close keyboard shortcuts"));
   setAutoDefault(false);
   setFixedSize(30, 30);
  }
 protected:
  void nextCheckState() override { dialog_->reject(); }
 private:
  QDialog* dialog_;
 };

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
  QLabel* summary_ = nullptr;
  struct KeyCap { QLabel* label = nullptr; int key = 0; };
  KeyCap keyCaps_[96]{};
  int keyCapCount_ = 0;
  bool isCompact_ = false;

  void reloadShortcuts();
  void rebuildKeyboardPreview();
 };

 KeyboardOverlayDialog::Impl::Impl(KeyboardOverlayDialog* owner) : owner_(owner) {}

KeyboardOverlayDialog::KeyboardOverlayDialog(QWidget* parent) : QDialog(parent), impl_(new Impl(this))
 {
  setWindowTitle("Keyboard Shortcuts");
  resize(1120, 780);
  setMinimumSize(820, 640);

  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
  setObjectName(QStringLiteral("KeyboardOverlayDialog"));

  auto* outerLayout = new QVBoxLayout(this);
  outerLayout->setContentsMargins(18, 18, 18, 18);
  outerLayout->setSpacing(12);

  auto* shell = new QWidget(this);
  shell->setObjectName(QStringLiteral("KeyboardOverlayShell"));
  shell->setAutoFillBackground(true);
  auto* shellLayout = new QVBoxLayout(shell);
  shellLayout->setContentsMargins(18, 18, 18, 18);
  shellLayout->setSpacing(12);

  auto* header = new QHBoxLayout();
  auto* title = new QLabel(tr("Keyboard Shortcuts"), shell);
  QFont titleFont = title->font();
  titleFont.setPointSize(titleFont.pointSize() + 3);
  titleFont.setBold(true);
  title->setFont(titleFont);
  header->addWidget(title);
  header->addStretch();
  header->addWidget(new OverlayCloseButton(this));
  shellLayout->addLayout(header);
  auto* subtitle = new QLabel(tr("Current bindings • Search actions and keys"), shell);
  QPalette muted = subtitle->palette();
  muted.setColor(QPalette::WindowText, QColor(149, 160, 173));
  subtitle->setPalette(muted);
  shellLayout->addWidget(subtitle);

  // Search Bar
  auto* searchLayout = new QHBoxLayout();
  auto* searchIcon = new QLabel(QStringLiteral("⌕"));
  impl_->searchBox_ = new QLineEdit();
  impl_->searchBox_->setPlaceholderText("Search shortcuts or actions...");
  impl_->searchBox_->setObjectName(QStringLiteral("KeyboardOverlaySearch"));
  impl_->searchBox_->setMinimumHeight(34);
  searchLayout->addWidget(searchIcon);
  searchLayout->addWidget(impl_->searchBox_);
  shellLayout->addLayout(searchLayout);

  impl_->keyboardPreview_ = new QFrame(shell);
  impl_->keyboardPreview_->setMinimumHeight(270);
  impl_->keyboardPreview_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  impl_->keyboardPreview_->setObjectName(QStringLiteral("KeyboardOverlayPreview"));
  impl_->keyboardPreview_->setFrameShape(QFrame::NoFrame);
  auto* keyboardLayout = new QVBoxLayout(impl_->keyboardPreview_);
  keyboardLayout->setContentsMargins(0, 8, 0, 8);
  keyboardLayout->setSpacing(6);
  // Fixed ANSI reference geometry; bindings remain the source of highlights.
  const char* rows[] = {
   "Esc|F1|F2|F3|F4|F5|F6|F7|F8|F9|F10|F11|F12",
   "`|1|2|3|4|5|6|7|8|9|0|-|=|Backspace",
   "Tab|Q|W|E|R|T|Y|U|I|O|P|[|]|\\",
   "Caps Lock|A|S|D|F|G|H|J|K|L|;|'|Enter",
   "Shift|Z|X|C|V|B|N|M|,|.|/|Shift",
   "Ctrl|Win|Alt|Space|Alt|Menu|Ctrl|Left|Down|Up|Right"
  };
  for (const auto* row : rows) {
   auto* keyRow = new QHBoxLayout();
   keyRow->setSpacing(6);
   const auto legends = QString::fromLatin1(row).split(QLatin1Char('|'));
   for (const auto& legend : legends) {
    auto* cap = new QLabel(legend, impl_->keyboardPreview_);
    cap->setAlignment(Qt::AlignCenter);
    cap->setMinimumSize(24, 35);
    cap->setAutoFillBackground(true);
    cap->setFrameShape(QFrame::StyledPanel);
    const auto sequence = QKeySequence::fromString(legend, QKeySequence::PortableText);
    int key = sequence.isEmpty() ? 0 : int(sequence[0].key());
    if (legend == QStringLiteral("Ctrl")) key = Qt::Key_Control;
    else if (legend == QStringLiteral("Shift")) key = Qt::Key_Shift;
    else if (legend == QStringLiteral("Alt")) key = Qt::Key_Alt;
    else if (legend == QStringLiteral("Win")) key = Qt::Key_Meta;
    else if (legend == QStringLiteral("Caps Lock")) key = Qt::Key_CapsLock;
    impl_->keyCaps_[impl_->keyCapCount_++] = {cap, key};
    const int stretch = legend == QStringLiteral("Space") ? 6 : legend.size() > 2 ? 2 : 1;
    keyRow->addWidget(cap, stretch);
   }
   keyboardLayout->addLayout(keyRow);
  }
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
  impl_->table_->setFrameShape(QFrame::NoFrame);
  impl_->table_->verticalHeader()->setDefaultSectionSize(28);
  impl_->table_->setAlternatingRowColors(true);
  impl_->tableScroll_->setWidget(impl_->table_);
  shellLayout->addWidget(impl_->tableScroll_);
  auto* footer = new QHBoxLayout();
  impl_->summary_ = new QLabel(shell);
  impl_->summary_->setPalette(muted);
  footer->addWidget(impl_->summary_);
  footer->addStretch();
  auto* hint = new QLabel(tr("Use the Help menu shortcut to toggle"), shell);
  hint->setPalette(muted);
  footer->addWidget(hint);
  shellLayout->addLayout(footer);
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

  setOverlayOpacity(1.0f);
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
         resize(1040, 700);
     } else {
         if (layout()) layout()->setContentsMargins(18, 18, 18, 18);
         resize(1120, 780);
     }
 }

 void KeyboardOverlayDialog::setOverlayOpacity(float opacity)
 {
     setWindowOpacity(opacity);
     QPalette palette = this->palette();
     palette.setColor(QPalette::Window, QColor(29, 33, 38));
     palette.setColor(QPalette::Base, QColor(24, 28, 33));
     palette.setColor(QPalette::AlternateBase, QColor(32, 37, 43));
     palette.setColor(QPalette::Button, QColor(40, 46, 53));
     palette.setColor(QPalette::ButtonText, QColor(220, 226, 233));
     palette.setColor(QPalette::Highlight, QColor(48, 79, 106));
     palette.setColor(QPalette::HighlightedText, QColor(240, 246, 252));
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
     impl_->reloadShortcuts();
     // Keep the existing search connection as the sole filtering path.
     impl_->searchBox_->clear();
     impl_->rebuildKeyboardPreview();
     if (parentWidget() && parentWidget()->window()) {
         const auto rect = parentWidget()->window()->geometry();
         move(rect.center() - QPoint(width() / 2, height() / 2));
     } else if (auto* screen = QGuiApplication::primaryScreen()) {
         const auto rect = screen->geometry();
         move(rect.center() - QPoint(width() / 2, height() / 2));
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
     int visibleCount = 0;
     for (int row = 0; row < table_->rowCount(); ++row) {
         if (!table_->isRowHidden(row)) ++visibleCount;
     }
     summary_->setText(QObject::tr("%1 shortcuts • ANSI reference layout").arg(visibleCount));
     // Cold UI refresh only: resolve key sequences on search, never on a frame path.
     for (int index = 0; index < keyCapCount_; ++index) {
         auto& cap = keyCaps_[index];
         QStringList actions;
         for (int row = 0; row < table_->rowCount(); ++row) {
             if (table_->isRowHidden(row)) continue;
             const auto* keyItem = table_->item(row, 4);
             const auto* actionItem = table_->item(row, 3);
             if (!keyItem || !actionItem || keyItem->text().isEmpty()) continue;
             const auto sequence = QKeySequence::fromString(keyItem->text(), QKeySequence::NativeText);
             bool matches = false;
             for (int stroke = 0; stroke < sequence.count(); ++stroke) {
                 const auto combination = sequence[stroke];
                 const auto modifiers = combination.keyboardModifiers();
                 matches |= combination.key() == cap.key;
                 matches |= cap.key == Qt::Key_Control && modifiers.testFlag(Qt::ControlModifier);
                 matches |= cap.key == Qt::Key_Shift && modifiers.testFlag(Qt::ShiftModifier);
                 matches |= cap.key == Qt::Key_Alt && modifiers.testFlag(Qt::AltModifier);
                 matches |= cap.key == Qt::Key_Meta && modifiers.testFlag(Qt::MetaModifier);
             }
             if (matches) actions.append(keyItem->text() + QStringLiteral("  —  ") + actionItem->text());
         }
         QPalette colors = owner_->palette();
         colors.setColor(QPalette::Window, actions.isEmpty() ? QColor(40, 46, 53) : QColor(43, 69, 91));
         colors.setColor(QPalette::WindowText, actions.isEmpty() ? QColor(180, 190, 201) : QColor(224, 238, 250));
         cap.label->setPalette(colors);
         cap.label->setToolTip(actions.join(QLatin1Char('\n')));
     }
 }

}
