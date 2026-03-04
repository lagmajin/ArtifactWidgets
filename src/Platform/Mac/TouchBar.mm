#import <AppKit/AppKit.h>
#include <QWidget>

// Objective-C++ Implementation for macOS Touch Bar
// NSTouchBarDelegate を実装して、Qtのウィジェットと紐付けます。

/*
@interface ArtifactTouchBarDelegate : NSObject <NSTouchBarDelegate>
- (NSTouchBarItem *)touchBar:(NSTouchBar *)touchBar makeItemForIdentifier:(NSTouchBarItemIdentifier)identifier;
@end

@implementation ArtifactTouchBarDelegate
- (NSTouchBarItem *)touchBar:(NSTouchBar *)touchBar makeItemForIdentifier:(NSTouchBarItemIdentifier)identifier {
    if ([identifier isEqualToString:@"com.artifact.play"]) {
        NSButtonTouchBarItem *item = [[NSButtonTouchBarItem alloc] initWithIdentifier:identifier];
        item.button.title = @"▶️ Play";
        return item;
    }
    return nil;
}
@end
*/

namespace ArtifactWidgets {
    // macOS環境で、QWidgetのウィンドウに対してNSTouchBarをセットアップするエントリーポイント
}
