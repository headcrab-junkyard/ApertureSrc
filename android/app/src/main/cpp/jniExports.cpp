#include <jni.h>

extern "C" JNIEXPORT void JNICALL
Java_com_ogs_next_EngineInterface_init(JNIEnv* env, jobject /* this */) {
}

extern "C" JNIEXPORT void JNICALL
Java_com_ogs_next_EngineInterface_resize(JNIEnv* env, jobject /* this */, jint newWidth, jint newHeight) {
}

extern "C" JNIEXPORT void JNICALL
Java_com_ogs_next_EngineInterface_render(JNIEnv* env, jobject /* this */) {
}