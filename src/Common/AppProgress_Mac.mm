#import <AppKit/AppKit.h>
#include <QWidget>
#include <QMenu>

// ※このファイルはmacOS環境でのみビルドされます
// Qtのネイティブハンドルを使用してDockTileを操作します

namespace ArtifactWidgets {

    // AppProgress の Mac用実装は通常、Qtの内部または nativeインターフェース（.mm）で記述します
    // ここでは概念的な実装例を提供します
    
    /*
    void AppProgress::setProgress(QWidget* widget, int value, int max)
    {
        // macOSのDockには標準的な進捗バーAPIがないため
        // DockTileのcontentViewにNSProgressIndicatorをセットするか、
        // アイコンを動的に生成して反映させる手法が一般的です。
        
        NSDockTile *tile = [NSApp dockTile];
        // ... プログレスを描画したViewをセット ...
        [tile display];
    }

    void AppProgress::setDockMenu(QMenu* menu)
    {
        // Qtの場合、QMenuBarやシステムのグローバルメニューとして統合されますが
        // ネイティブレベルでは NSApplicationDelegate の applicationDockMenu: に反応させます
    }
    */
}
