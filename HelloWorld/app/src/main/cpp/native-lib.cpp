#include <jni.h>
#include <string>
#include <cstring>
#include <android/log.h>
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_wil_helloworld_MainActivity_getBufferForNative(JNIEnv *env, jobject obj, jstring myBuf) {
    char buffer[10];
    const char *myNewBuf = env->GetStringUTFChars(myBuf, 0);
    __android_log_print(ANDROID_LOG_DEBUG, "debug", "str = %s, len = %d ", myNewBuf, strlen(myNewBuf));
    strcpy(buffer,myNewBuf);
    return env->NewStringUTF(buffer);
}
