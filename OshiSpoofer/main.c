#include <pthread.h>
#include <dlfcn.h>
#include <memory.h>

#include "jni.h"
#include "jvmti.h"

typedef void (*JVM_MonitorNotify)(JNIEnv *env, jobject obj);

typedef jint (JNICALL * Jvm) (JavaVM**, jsize, jsize*);

#define UNUSED(x) (void)(x)

void JNICALL Breakpoint(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method, jlocation location);
void JNICALL VMinit(jvmtiEnv *jvmti_env, JNIEnv* jni_env,  jthread thread);

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
    UNUSED(options);
    UNUSED(reserved);
    jvmtiEnv* jvmti = NULL;

    (*vm)->GetEnv(vm, (void**)&jvmti, JVMTI_VERSION);

    jvmtiEventCallbacks events;
    jvmtiCapabilities capabilities;

    memset(&events, 0, sizeof(jvmtiEventCallbacks));
    memset(&capabilities, 0, sizeof(jvmtiCapabilities));

    events.Breakpoint = &Breakpoint;
    events.VMInit = &VMinit;

    capabilities.can_generate_breakpoint_events = 1;
    capabilities.can_force_early_return = 1;


    (*jvmti)->AddCapabilities(jvmti, &capabilities);
    (*jvmti)->SetEventCallbacks(jvmti, &events, sizeof(events));
    (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE,
                              JVMTI_EVENT_VM_INIT, (jthread)NULL);

    return 0;
}

void JNICALL Breakpoint(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method, jlocation location) {
    UNUSED(method);
    UNUSED(location);
    jstring serialNumber = (*jni_env)->NewStringUTF(jni_env, "hello");
    (*jvmti_env)->ForceEarlyReturnObject(jvmti_env, thread, serialNumber);
}

void JNICALL VMinit(jvmtiEnv *jvmti, JNIEnv* env, jthread thread) {
    UNUSED(thread);
    jclass baseBoard = (*env)->FindClass(env, "oshi/hardware/platform/linux/LinuxBaseboard");
    jmethodID getSerialNumberId = (*env)->GetMethodID(env, baseBoard, "getSerialNumber", "()Ljava/lang/String;");

    (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_BREAKPOINT, NULL);

    (*jvmti)->SetBreakpoint(jvmti, getSerialNumberId, 0);

}



