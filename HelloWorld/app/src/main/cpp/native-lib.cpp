#include <jni.h>
#include <string>
#include <android/log.h>
#include <inttypes.h>
#include <stdio.h>
#include <cstdint>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_wil_helloworld_MainActivity_getCanary(JNIEnv *env, jobject /* this */) {
    char low_alloc_buf[10];
    //const char *myNewBuf = env->GetStringUTFChars(myBuf, 0);
    const char *my_new_Buffer = "Hello";
    uint64_t i;
    bool is32 = true;
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Made it before Assembly!");
#if defined(__i386__)
    asm("movl 0x98(%%esp),%0;" : "=r"(i) :);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Canary: %x", i);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Made it Assembly i386!");
#endif
#if defined(__aarch64__)
    asm("ldr %0,[x29,#-16];" : "=r"(i) :);
    is32 = false;
#endif
#if defined(__arm__)
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Made it Assembly arm!");
    asm("ldr %0,[sp, #132];" : "=r"(i) :);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Canary: %x", i);
#if defined(__ARM_ARCH_7A__)
    asm("ldr %0,[sp, #132];" : "=r"(i) :);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Canary: %x", i);
#endif
#endif

    strcpy(low_alloc_buf, my_new_Buffer);
    char canaryBuffer[20];
    if (is32)
        sprintf(canaryBuffer, "%" PRIx32, i);
    else
        sprintf(canaryBuffer, "%" PRIx64, i);
    return env->NewStringUTF(canaryBuffer);;
}
