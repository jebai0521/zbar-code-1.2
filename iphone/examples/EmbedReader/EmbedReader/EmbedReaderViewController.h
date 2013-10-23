//
//  EmbedReaderViewController.h
//  EmbedReader
//
//  Created by spadix on 5/2/11.
//

#import <UIKit/UIKit.h>

@interface EmbedReaderViewController
    : UIViewController
    < ZBarReaderViewDelegate >
{
    ZBarReaderView *readerView;
    UITextView *resultText;
    ZBarCameraSimulator *cameraSim;
}

@property (nonatomic, retain) ZBarReaderView *readerView;
@property (nonatomic, retain) UITextView *resultText;

@end
