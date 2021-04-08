#include <napi.h>
#include <iostream>
#include <Carbon/Carbon.h>

#define MAX_DISPLAYS 16
// 定义 Add 函数
Napi::Value getWindowRect(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  // 接收参数
    double arg0 = info[0].As<Napi::Number>().DoubleValue();

    CGDisplayErr      dErr;
    CGDisplayCount    displayCount, i;
    CGDirectDisplayID onlineDisplays[ MAX_DISPLAYS ];

    dErr = CGGetOnlineDisplayList(2, onlineDisplays, &displayCount);
    if (dErr != kCGErrorSuccess) {
            fprintf(stderr, "CGGetOnlineDisplayList: error %d.\n", dErr);
            exit(1);
    }
    for (i = 0; i < displayCount; i++) {
            CGDirectDisplayID dID = onlineDisplays[i];
            CGImageRef cgRef = CGDisplayCreateImage(dID);
            UIImage* image = [UIImage imageWithCGImage: cgRef];

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