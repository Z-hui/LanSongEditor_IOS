//
//  CameraPen2.h
//  LanSongEditorFramework
//
//  Created by sno on 2018/8/20.
//  Copyright © 2018 sno. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import "LanSongContext.h"
#import "LanSongOutput.h"
#import "LanSongColorConversion.h"
#import "Pen.h"


//Optionally override the YUV to RGB matrices
//void setColorConvert601( GLfloat conversionMatrix[9] );
//void setColorConvert601FullRange( GLfloat conversionMatrix[9] );
//void setColorConvert709( GLfloat conversionMatrix[9] );


/**
 A LanSongOutput that provides frames from either camera
 */
@interface CameraPen2 : Pen <AVCaptureVideoDataOutputSampleBufferDelegate, AVCaptureAudioDataOutputSampleBufferDelegate>
{
    NSUInteger numberOfFramesCaptured;
    CGFloat totalFrameTimeDuringCapture;
    
    AVCaptureSession *_captureSession;
    AVCaptureDevice *_inputCamera;
    AVCaptureDevice *_microphone;
    AVCaptureDeviceInput *videoInput;
    AVCaptureVideoDataOutput *videoOutput;
    
    BOOL capturePaused;
    LanSongRotationMode outputRotation, internalRotation;
    dispatch_semaphore_t frameRenderingSemaphore;
    
    BOOL captureAsYUV;
    GLuint luminanceTexture, chrominanceTexture;
    
}

/// Whether or not the underlying AVCaptureSession is running
@property(readonly, nonatomic) BOOL isRunning;

/// The AVCaptureSession used to capture from the camera
@property(readonly, retain, nonatomic) AVCaptureSession *captureSession;

/// This enables the capture session preset to be changed on the fly
@property (readwrite, nonatomic, copy) NSString *captureSessionPreset;

@property (readwrite) int32_t frameRate;

/// Easy way to tell which cameras are present on device
@property (readonly, getter = isFrontFacingCameraPresent) BOOL frontFacingCameraPresent;
@property (readonly, getter = isBackFacingCameraPresent) BOOL backFacingCameraPresent;

/// This enables the benchmarking mode, which logs out instantaneous and average frame times to the console
@property(readwrite, nonatomic) BOOL runBenchmark;

/// Use this property to manage camera settings. Focus point, exposure point, etc.
@property(readonly) AVCaptureDevice *inputCamera;

/// This determines the rotation applied to the output image, based on the source material
@property(readwrite, nonatomic) UIInterfaceOrientation outputImageOrientation;

/// These properties determine whether or not the two camera orientations should be mirrored. By default, both are NO.
@property(readwrite, nonatomic) BOOL horizontallyMirrorFrontFacingCamera, horizontallyMirrorRearFacingCamera;

//@property(nonatomic, assign) id<LanSongVideoCameraDelegate> delegate;

/// @name Initialization and teardown

/** Begin a capture session
 
 See AVCaptureSession for acceptable values
 
 @param sessionPreset Session preset to use
 @param cameraPosition Camera to capture from
 */
- (id)initWithSessionPreset:(NSString *)sessionPreset cameraPosition:(AVCaptureDevicePosition)cameraPosition padSize:(CGSize)size;

/** Add audio capture to the session. Adding inputs and outputs freezes the capture session momentarily, so you
 can use this method to add the audio inputs and outputs early, if you're going to set the audioEncodingTarget
 later. Returns YES is the audio inputs and outputs were added, or NO if they had already been added.
 */
- (BOOL)addAudioInputsAndOutputs;

/** Remove the audio capture inputs and outputs from this session. Returns YES if the audio inputs and outputs
 were removed, or NO is they hadn't already been added.
 */
- (BOOL)removeAudioInputsAndOutputs;

/** Tear down the capture session
 */
- (void)removeInputsAndOutputs;

/// @name Manage the camera video stream

/** Start camera capturing
 */
- (void)startCameraCapture;

/** Stop camera capturing
 */
- (void)stopCameraCapture;

/** Pause camera capturing
 */
- (void)pauseCameraCapture;

/** Resume camera capturing
 */
- (void)resumeCameraCapture;

/** Process a video sample
 @param sampleBuffer Buffer to process
 */
- (void)processVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;

/** Process an audio sample
 @param sampleBuffer Buffer to process
 */
- (void)processAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;

/** Get the position (front, rear) of the source camera
 */
- (AVCaptureDevicePosition)cameraPosition;

/** Get the AVCaptureConnection of the source camera
 */
- (AVCaptureConnection *)videoCaptureConnection;

/** This flips between the front and rear cameras
 */
- (void)rotateCamera;

/// @name Benchmarking

/** When benchmarking is enabled, this will keep a running average of the time from uploading, processing, and final recording or display
 */
- (CGFloat)averageFrameDurationDuringCapture;

- (void)resetBenchmarkAverage;

+ (BOOL)isBackFacingCameraPresent;
+ (BOOL)isFrontFacingCameraPresent;

@end