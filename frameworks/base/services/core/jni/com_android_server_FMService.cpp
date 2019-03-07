#define LOG_TAG "HQ_FMService"
#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"
#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/hardware.h>
#include <hardware/hq_fm_hal.h>
#include <stdio.h>
#include <android/log.h>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define ENABLE_DEBUG_LOG

/* 调试打印宏. */
#ifdef ENABLE_DEBUG_LOG
#define D(fmt, args...) \
    { LOGD("[Line] : %d; [Func] : %s() : " fmt, __LINE__, __FUNCTION__, ## args); }
#else 
#define D(fmt, args...)
#endif

#define I(fmt, args...) \
    { LOGI("[Line] : %d; [Func] : %s() ; ! Info : " fmt, __LINE__, __FUNCTION__, ## args); }

/* Error Log. */
#define E(fmt, args...) \
    { LOGE("[Line] : %d; [Func] : %s() ; !!! Error : " fmt, __LINE__, __FUNCTION__, ## args); }

namespace android
{


	/*在硬件抽象层中定义的硬件访问结构体，参考<hardware/hq_fm_hal.h>*/
	fm_control_device_t* sFmControlDevice;

	/*-------------------访问硬件的接口------------------*/
	
	 /*************************
         *功能：设置频率
         *参数：freq为频率，direct为FM/AM（0/1）
         *返回值：result
         *************************/
	static jint FM_setFreq(JNIEnv* env, jobject clazz, jint freq, jint direct){
		jint result = 0;
		D("yang hello freq");
		if ( NULL == sFmControlDevice )
		{
			E("'sFmControlDevice' is null.");
			return -1;
		}
		result = sFmControlDevice->set_freq(sFmControlDevice, freq, direct);
		return result;
	}

	 /*************************
         *功能：设置音量
         *参数：volume为音量
         *返回值：result
         *************************/
	static jint FM_setVolume(JNIEnv* env, jobject clazz, jint volume){
		jint result = 0;
		D("yang set Volume");
		if ( NULL == sFmControlDevice )
		{
			E("'sFmControlDevice' is null.");
			return -1;
		}
		result = sFmControlDevice->set_volume(sFmControlDevice, volume);
		return result;
	}

	 /*************************
         *功能：设置静音
         *参数：mute静音使能位
         *返回值：result
         *************************/
	static jint FM_setMute(JNIEnv* env, jobject clazz, jint mute){
		jint result = 0;
		
		D("yang set MUTe");
		if ( NULL == sFmControlDevice )
		if ( NULL == sFmControlDevice )
		{
			E("'sFmControlDevice' is null.");
			return -1;
		}
		result = sFmControlDevice->set_mute(sFmControlDevice, mute);
		return result;
	}

         /*************************
         *功能：当前AM或FM模式的上一频道
         *参数：
         *返回值：result
         *************************/
	static jint FM_DoDCStepUp(JNIEnv* env, jobject clazz){
		jint result = 0;
		D("yang set SET UP");
		if ( NULL == sFmControlDevice )
	
		if ( NULL == sFmControlDevice )
		{
			E("'sFmControlDevice' is null.");
			return -1;
		}
		result = sFmControlDevice->hq_stepup(sFmControlDevice);
		return result;
	}

	 /*************************
         *功能：当前AM或FM模式的下一频道
         *参数：mute静音使能位
         *返回值：result
         *************************/
	static jint FM_DoDCStepDown(JNIEnv* env, jobject clazz){
		jint result = 0;
		if ( NULL == sFmControlDevice )
		{
				//E("'sFmControlDevice' is null.");
				return -1;
		}
		result = sFmControlDevice->hq_stepdown(sFmControlDevice);
		//LOGI("set fm freq = %d",freq);
		return result;
	}

	 /*************************
         *功能：扫台动作
         *参数：
         *返回值：result
         *************************/
	static jint FM_DoDCSearch(JNIEnv* env, jobject clazz){
		jint result = 0;
		if ( NULL == sFmControlDevice )
		{
			E("'sFmControlDevice' is null.");
			return -1;
		}
		result = sFmControlDevice->autoSearch(sFmControlDevice);
		return result;
	}
	
         /*************************
         *功能：获取搜台结果
         *参数：
         *返回值：搜台结果
         *************************/
	 static jintArray FM_GetFreqBuffer(JNIEnv* env, jobject clazz){
		jint n = 18;
		jintArray rarr;
		jint *oarr;
		/*malloc*/
		oarr =(jint* )malloc(sizeof(n)*n);
		if (oarr == NULL)
		{
			return 0;
		}
		
		/* create jintArray*/
		rarr = env->NewIntArray( n);
		if(rarr == NULL)
		{
			return 0;
		}
		
		if ( NULL == sFmControlDevice )
		{
			E("'sFmControlDevice' is null.");
	//		return -1;
		}
		D("GET BUf  yang");
		//oarr数组填充
		sFmControlDevice->get_freqbuff(sFmControlDevice, oarr);
				
		
		D("yang  buf yang %d ", *oarr);
				
		env->SetIntArrayRegion(rarr, 0, n, oarr);	
		

		return rarr;	
	}

	/**
	 * 加载 FM HAL 模块, 并返回其 控制设备实例的指针
	 * @param controlDevice
	 *          用来返回 控制设备指针.
	 *          由调用者保证实参正确.
	 * @return
	 *          若成功, 返回 0.
	 *          若失败, error code.
	 */
	/*通过硬件抽象层定义的硬件模块打开接口打开硬件设备*/
	static int loadFmHal(fm_control_device_t** controlDevice)
	{
	    	 int result = 0;
	   	 hw_module_t* module;
		D("yang load");
	    	/* 尝试加载 HDMI HAL module 的实例数据. */
      		result = hw_get_module(HQ_FM_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
      		if(result == 0){
	 		 result = fm_control_open(module, controlDevice);
          		if(result == 0){
				D("yang OK OPEN");
            			return 0;  
			}else{	
				D("yang OFF OPEN");
				return -1;
			}
      		}
	E("orr yang native %d",result);
	return -1;
	}

         /*************************
         *功能：使能动作
         *参数：
         *返回值：result
         *************************/
	static jint android_FmService_enableFmFacility(JNIEnv *env, jclass clazz, jint isEnable)
	{
   		 jint result = 0;

    		 if ( isEnable ){
			D("yang enable");
	         	if ( NULL == sFmControlDevice ){
            	 	/* 加载 FM HAL 实例. */
            			 if ( (result = loadFmHal(&sFmControlDevice) ) != 0 ){
		    			E("yang Fail to load FM HAL module, result = %d", result);
                    			goto EXIT;
            	 	 	 }
        		}
     		   	/* 使能 FM 机构. */
       			 if ( (result = sFmControlDevice->enable_fm_facility(sFmControlDevice, 1) ) != 0 ){
 				E("yang Fail to enable FM facility, result = %d", result);
           			goto EXIT;
        		}
		
    		}
  		  /* 否则, 即要 "禁止" FM 机构, 则 ... */
   		 else{
		 /* 若 FM HAL "已经" 加载, 则 ... */
       			 if ( NULL != sFmControlDevice ){
           			 if ( (result = sFmControlDevice->enable_fm_facility(sFmControlDevice, FALSE) ) != 0 ){
            				  E("Fail to disable FM facility, result = %d", result);
                			goto EXIT;
            			}
        		}
    		}
EXIT:
	return result;
}

	/*JNI 方法表*/
	static const JNINativeMethod method_table[] = {
		{"native_enableFmFacility", "(I)I", (void *)android_FmService_enableFmFacility},//1、初始化
		{"native_setFmFreq", "(II)I", (void*)FM_setFreq},				//2、设置频率：AM/FM切换
		{"native_setFmVolume", "(I)I", (void*)FM_setVolume},				//3、设置音量
		{"native_setFmMute", "(I)I", (void*)FM_setMute},				//4、设置静音
		{"native_DoDCStepUp", "()I", (void*)FM_DoDCStepUp},				//5、向上单步搜索
		{"native_DoDCStepDown", "()I", (void*)FM_DoDCStepDown},				//6、向下单步搜索
		{"native_DoDCSearch", "()I", (void*)FM_DoDCSearch},				//7、自动搜索动作
		{"native_GetFreqBuffer", "()[I", (void*)FM_GetFreqBuffer},			//8、获取搜台结果
	};

	/*注册JNI方法*/
	int register_android_server_FMService(JNIEnv *env){
    	return jniRegisterNativeMethods(env, "com/android/server/HQFMService", method_table, NELEM(method_table));
	}
}
