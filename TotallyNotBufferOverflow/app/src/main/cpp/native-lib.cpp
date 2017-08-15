#include <jni.h>
#include <string>
#include <android/log.h>
#include <inttypes.h>
#include <stdio.h>
#include <cstdint>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_android_overflow_buffer_totallynotbufferoverflow_OverflowActivity_findMyCanary(JNIEnv *env, jobject /* this */) {
    char low_alloc_buf[50];
    //const char *myNewBuf = env->GetStringUTFChars(myBuf, 0);
    const char *my_new_Buffer = "Hello";
    uint64_t i;
    uint32_t j;
    bool is32 = true;

    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Made it before Assembly!");
#if defined(__i386__)
    asm("movl 0x1c(%%esp),%0;" : "=r"(j) :);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Canary: %" PRIx32, i);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Made it Assembly i386!");
#endif
#if defined(__aarch64__)
    asm("ldr %0,[x29,#-8];" : "=r"(i) :);
    is32 = false;
#endif
#if defined(__arm__)
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Made it Assembly arm!");
    asm("ldr %0,[sp, #188];" : "=r"(j) :);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Arm Canary: %" PRIx32, j);
#if defined(__ARM_ARCH_7A__)
    asm("ldr %0,[sp, #196];" : "=r"(j) :);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Armeabi Canary: %" PRIx32, j);
#endif
#endif

    strcpy(low_alloc_buf, my_new_Buffer);
    char canaryBuffer[40] = {0};
    if (is32) {
        sprintf(canaryBuffer, "0x%" PRIx32, j);
    } else
        sprintf(canaryBuffer, "0x%" PRIx64, i);
    return env->NewStringUTF(canaryBuffer);;
}
