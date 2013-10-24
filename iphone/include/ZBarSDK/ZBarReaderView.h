//------------------------------------------------------------------------
//  Copyright 2010 (c) Jeff Brown <spadix@users.sourceforge.net>
//
//  This file is part of the ZBar Bar Code Reader.
//
//  The ZBar Bar Code Reader is free software; you can redistribute it
//  and/or modify it under the terms of the GNU Lesser Public License as
//  published by the Free Software Foundation; either version 2.1 of
//  the License, or (at your option) any later version.
//
//  The ZBar Bar Code Reader is distributed in the hope that it will be
//  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
//  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser Public License for more details.
//
//  You should have received a copy of the GNU Lesser Public License
//  along with the ZBar Bar Code Reader; if not, write to the Free
//  Software Foundation, Inc., 51 Franklin St, Fifth Floor,
//  Boston, MA  02110-1301  USA
//
//  http://sourceforge.net/projects/zbar
//------------------------------------------------------------------------

#import <UIKit/UIKit.h>
#import "ZBarImageScanner.h"

@class AVCaptureSession, AVCaptureDevice;
@class CALayer;
@class ZBarImageScanner, ZBarCaptureReader, ZBarReaderView;

// delegate is notified of decode results.

@protocol ZBarReaderViewDelegate < NSObject >

- (void) readerView: (ZBarReaderView*) readerView
     didReadSymbols: (ZBarSymbolSet*) symbols
          fromImage: (UIImage*) image;

@end

// read barcodes from the displayed video preview.  the view maintains
// a complete video capture session feeding a ZBarCaptureReader and
// presents the associated preview with symbol tracking annotations.

@interface ZBarCropMaskLayer : CALayer

@property (assign, nonatomic) CGRect hollowRect;

@end

@interface ZBarReaderView
    : UIView
{
    id<ZBarReaderViewDelegate> readerDelegate;
    ZBarCaptureReader *captureReader;           // 设备流处理
    CGRect scanCrop, effectiveCrop;             // 扫描的裁切区域，有效的裁切区域
    CGAffineTransform previewTransform;         // 预览的变换
    CGFloat zoom, zoom0, maxZoom;               // 缩放率，最小和最大
    UIColor *trackingColor;                     // 有效区域跟踪的颜色
    BOOL tracksSymbols, showsFPS;               // 是否显示跟踪区域，是否显示刷新率
    NSInteger torchMode;                        // 闪光灯模式
    UIInterfaceOrientation interfaceOrientation;// 屏幕的朝向
    NSTimeInterval animationDuration;           // 动画的时长

    ZBarCropMaskLayer* overlay;
    CALayer *preview, *tracking, *cropLayer;      // 预览层,覆盖层,跟踪层,裁剪层
    UIView *fpsView;                            // 刷新率视图
    UILabel *fpsLabel;                          // 刷新率标签
    UIPinchGestureRecognizer *pinch;            // 捏合手势
    CGFloat imageScale;                         // 图片的比例
    CGSize imageSize;                           // 图片尺寸（摄像头采样的大小）
    BOOL started, running;                      // 是否开始，是否在运行
}

// supply a pre-configured image scanner.
- (id) initWithImageScanner: (ZBarImageScanner*) imageScanner;

// start the video stream and barcode reader.
- (void) start;

// stop the video stream and barcode reader.
- (void) stop;

// clear the internal result cache
- (void) flushCache;

// compensate for device/camera/interface orientation
- (void) willRotateToInterfaceOrientation: (UIInterfaceOrientation) orient
                                 duration: (NSTimeInterval) duration;

// delegate is notified of decode results.
@property (nonatomic, assign) id<ZBarReaderViewDelegate> readerDelegate;

// access to image scanner for configuration.
@property (nonatomic, readonly) ZBarImageScanner *scanner;

// whether to display the tracking annotation for uncertain barcodes
// (default YES).
@property (nonatomic) BOOL tracksSymbols;

// color of the tracking box (default green)
@property (nonatomic, retain) UIColor *trackingColor;

// enable pinch gesture recognition for zooming the preview/decode
// (default YES).
@property (nonatomic) BOOL allowsPinchZoom;

// torch mode to set automatically (default Auto).
@property (nonatomic) NSInteger torchMode;

// whether to display the frame rate for debug/configuration
// (default NO).
@property (nonatomic) BOOL showsFPS;

// zoom scale factor applied to video preview *and* scanCrop.
// also updated by pinch-zoom gesture.  clipped to range [1,maxZoom],
// defaults to 1.25
@property (nonatomic) CGFloat zoom;
- (void) setZoom: (CGFloat) zoom
        animated: (BOOL) animated;

// maximum settable zoom factor.
@property (nonatomic) CGFloat maxZoom;

// the region of the image that will be scanned.  normalized coordinates.
@property (nonatomic) CGRect scanCrop;

// additional transform applied to video preview.
// (NB *not* applied to scan crop)
@property (nonatomic) CGAffineTransform previewTransform;

// specify an alternate capture device.
@property (nonatomic, retain) AVCaptureDevice *device;

// direct access to the capture session.  warranty void if opened...
@property (nonatomic, readonly) AVCaptureSession *session;
@property (nonatomic, readonly) ZBarCaptureReader *captureReader;

// this flag still works, but its use is deprecated
@property (nonatomic) BOOL enableCache;

@end
