#include <napi.h>
#include <iostream>
#include <Carbon/Carbon.h>

#define MAX_DISPLAYS 16
// 定义 Add 函数
Napi::Value getWindowRect(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  // 接收参数
    double arg0 = info[0].As<Napi::Number>().DoubleValue();

    std::string baseImageOutput = "file:///Users/hank.huang/Desktop";
    std::string pathSeparator = "/";
    std::string baseImageName = "image-";
    std::string imageExtension = ".png";

    CGDisplayErr      dErr;
    CGDisplayCount    displayCount, i;
    CGDirectDisplayID onlineDisplays[ MAX_DISPLAYS ];

    dErr = CGGetOnlineDisplayList(2, onlineDisplays, &displayCount);
    if (dErr != kCGErrorSuccess) {
            fprintf(stderr, "CGGetOnlineDisplayList: error %d.\n", dErr);
            exit(1);
    }
    for (i = 0; i < displayCount; i++) {
            std::string imagePath = baseImageOutput + pathSeparator + baseImageName + std::to_string(i) + imageExtension;
            const char *charPath = imagePath.c_str();

            CFStringRef imageOutputPath = CFStringCreateWithCString(kCFAllocatorDefault, charPath, kCFURLPOSIXPathStyle);

            CGDirectDisplayID dID = onlineDisplays[i];
            CGImageRef image = CGDisplayCreateImage(dID);
            CFURLRef url = CFURLCreateWithString(kCFAllocatorDefault, imageOutputPath, NULL);
            CGImageDestinationRef destination = CGImageDestinationCreateWithURL(url, kUTTypePNG, 1, NULL);
            if (!destination) {
            std::cout<< "The destination does not exist: " << imagePath << std::endl;
            CGImageRelease(image);
        }
            CGImageDestinationAddImage(destination, image, NULL);
            if (!CGImageDestinationFinalize(destination)) {
            std::cout << "Failed to write image to the path" << std::endl;;
            CFRelease(destination);
            CGImageRelease(image);
        }
            CFRelease(destination);
            CGImageRelease(image);    
            // UIImage* image = [UIImage imageWithCGImage: cgRef];
            printf("%-16p", dID);
           
    }
}

// 入口函数，用于注册我们的函数、对象等等
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  // 将一个名为 add 的函数挂载到 exports 上
  exports.Set(Napi::String::New(env, "getWindowRect"), Napi::Function::New(env, getWindowRect));
  return exports;
}

// 固定的宏使用
NODE_API_MODULE(addon, Init)