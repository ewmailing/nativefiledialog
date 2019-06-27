#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <android/log.h>
#include <jni.h>


static lua_State* g_luaState = NULL;

JNIEXPORT jstring JNICALL Java_br_pucrio_tecgraf_testapp_MainActivity_stringFromJNI(
		JNIEnv* jni_env,
		jobject thiz)
{
	__android_log_print(ANDROID_LOG_DEBUG, "LuaTestAppLib", "Hello from C");
	
//	return (*jni_env)->NewStringUTF(jni_env, "Hello From C");
    lua_getglobal(g_luaState, "_VERSION"); // Returns something like "Lua 5.3"
    const char* c_version_string = lua_tostring(g_luaState, -1);
    jstring j_version_string = (*jni_env)->NewStringUTF(jni_env, c_version_string);
    lua_pop(g_luaState, 1);
    return j_version_string;
}

JNIEXPORT void JNICALL Java_br_pucrio_tecgraf_testapp_MainActivity_doLuaInit(JNIEnv* jni_env, jobject thiz_activity)
{
	g_luaState = luaL_newstate();
	luaL_openlibs(g_luaState);

}

JNIEXPORT void JNICALL Java_br_pucrio_tecgraf_testapp_MainActivity_doLuaQuit(JNIEnv* jni_env, jobject thiz_activity)
{

	lua_close(g_luaState);
	g_luaState = NULL;


}

/*
JNIEXPORT jboolean JNICALL Java_net_playcontrol_inapptest_MainActivity_IABHandleActivityResult(JNIEnv* jni_env, jobject thiz_activity, jint request_code, jint result_code, jobject intent_data)
{
	return (jboolean)BIAP_Android_HandleOnActivityResult(jni_env, request_code, result_code, intent_data);
}
*/
//BIAP_EXPORT void BIAP_Android_RegisterActivity(void* context_or_activity_object);
//BIAP_EXPORT _Bool BIAP_Android_HandleOnActivityResult(int32_t request_code, int32_t result_code, void* intent_data);

