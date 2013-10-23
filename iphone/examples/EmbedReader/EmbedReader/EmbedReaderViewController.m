//
//  EmbedReaderViewController.m
//  EmbedReader
//
//  Created by spadix on 5/2/11.
//

#import "EmbedReaderViewController.h"

@implementation EmbedReaderViewController

@synthesize readerView, resultText;

- (void) cleanup
{
    [cameraSim release];
    cameraSim = nil;
    readerView.readerDelegate = nil;
    [readerView release];
    readerView = nil;
    [resultText release];
    resultText = nil;
}

- (void) dealloc
{
    [self cleanup];
    [super dealloc];
}

- (void) viewDidLoad
{
    [super viewDidLoad];
    
    CGSize size = [UIScreen mainScreen].bounds.size;
    
    if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_6_1)
    {
        [self setWantsFullScreenLayout:YES];
        [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleBlackTranslucent];
        [self.view setFrame:CGRectMake(0, 0, size.width, size.height)];
    }
    
    readerView = [[ZBarReaderView alloc] init];
    [self.view addSubview:readerView];
    
    // the delegate receives decode results
    readerView.readerDelegate = self;
    
    // 可能是因为坐标系的不同，导致设置存在问题
    [readerView setScanCrop:CGRectMake(.15, .15, .5, .7)];
    [readerView setShowsFPS:YES];
    [readerView setAllowsPinchZoom:NO];

    // you can use this to support the simulator
    if(TARGET_IPHONE_SIMULATOR) {
        cameraSim = [[ZBarCameraSimulator alloc]
                        initWithViewController: self];
        cameraSim.readerView = readerView;
    }
    
    [readerView setFrame:CGRectMake(0, 0, size.width, size.height)];
    
    resultText = [[UITextView alloc] initWithFrame:CGRectZero];
    [self.view addSubview:resultText];
    resultText.textColor = [UIColor colorWithWhite:1.0 alpha:1.0];
    resultText.backgroundColor = [UIColor clearColor];
    [resultText setFrame:CGRectMake(0, size.height - 32, size.width, 32)];
}

- (void) viewDidUnload
{
    [self cleanup];
    [super viewDidUnload];
}

- (BOOL) shouldAutorotateToInterfaceOrientation: (UIInterfaceOrientation) orient
{
    // auto-rotation is supported
    return(YES);
}

- (void) willRotateToInterfaceOrientation: (UIInterfaceOrientation) orient
                                 duration: (NSTimeInterval) duration
{
    // compensate for view rotation so camera preview is not rotated
    [readerView willRotateToInterfaceOrientation: orient
                                        duration: duration];
}

- (void) viewDidAppear: (BOOL) animated
{
    // run the reader when the view is visible
    [readerView start];
}

- (void) viewWillDisappear: (BOOL) animated
{
    [readerView stop];
}

- (void) readerView: (ZBarReaderView*) view
     didReadSymbols: (ZBarSymbolSet*) syms
          fromImage: (UIImage*) img
{
    // do something useful with results
    for(ZBarSymbol *sym in syms) {
        resultText.text = sym.data;
        break;
    }
}

@end
