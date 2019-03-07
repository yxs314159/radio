#define LOG_TAG "HQFMHal"

#include <hardware/vibrator.h>
#include <hardware/hardware.h>
#include <hardware/hq_fm_hal.h>
#include <cutils/log.h>
#include <stdbool.h>

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <utils/Log.h>
#include <string.h>

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

/* Info Log. */
#define I(fmt, args...) \
    { LOGI("[Line] : %d; [Func] : %s() ; ! Info : " fmt, __LINE__, __FUNCTION__, ## args); }

/* Warning Log. */
#define W(fmt, args...) \
    { LOGW("[Line] : %d; [Func] : %s() ; ! Warning : " fmt, __LINE__, __FUNCTION__, ## args); }

/* Error Log. */
#define E(fmt, args...) \
    { LOGE("[Line] : %d; [Func] : %s() ; !!! Error : " fmt, __LINE__, __FUNCTION__, ## args); }

/*---------------------------------------------------------------------------*/


static const struct InterfererSignalItem InterfererSignal[INTERFERERSIGNALITEMCOUNT] =
{
    {8850, 28}, {9600, 118}, {10470, 119},
};


/**
 * 对 fm_control_device_t 的定制扩展.
 * 扩展某些同对控制设备操作流程相关的 定制数据.
 */
typedef struct fm_control_context_t
{

    /* "基类子对象" : 必须是首成员. */
	fm_control_device_t	device;
	bool            	is_fm_facility_enabled;
	struct TRadioParam      FParams[rtCount];//参数
	enum TRadioType		FRadioType;	// AM/FM
	int             FSystemVolume;		// 当前系统音量
	int             FVolume;	    	//音量
	int             FVolumeOnTTS;
	int             FDelayOnTTS; 		// TTS语音播放时，音量衰减/恢复的延时
	bool         FMute;
    	struct TRadioCompare   FRadioCompare[3];// 收音机参数比较
	struct TRadioEngine 	 FRadioEngine;
    	bool         FIsSeeking;		//扫描状态
    	bool         FStopSeek;
	int	FreqBuffer[18];			//扫台结果保存
}fm_control_context_t;

int FreqStep(fm_control_device_t* dev)
{
		fm_control_context_t* this = (fm_control_context_t*)dev;
		return this->FParams[this->FRadioType].Step;
}

int FreqLevel(fm_control_device_t* dev)
{
		fm_control_context_t* this = (fm_control_context_t*)dev;
		return this->FParams[this->FRadioType].Level;
}

/*************************
*功能：设置频率调用tea6851a.c
*
*************************/
static int control_context__set_freq(fm_control_device_t* dev, int currentFreq)
{
	int result = 0;
	bool Result = 0;
	D("yang yang set freq");
	fm_control_context_t* this = (fm_control_context_t*)dev;

	int FreqBegin = this->FParams[this->FRadioType].FreqBegin;

	int FreqEnd = this->FParams[this->FRadioType].FreqEnd;

	if(currentFreq >= FreqBegin && currentFreq <= FreqEnd){

		this->FParams[this->FRadioType].Freq = currentFreq;
		
		result = hq_set_freq(currentFreq);
		
		D("yang Freqbeg : %d freqend %d curr %d",FreqBegin,FreqEnd,currentFreq);
	}else{
		Result = -1;
	}
return result;
}

/*****************
*设置音量
*****************/
static int control_context__set_volume(fm_control_device_t* dev, int Value)
{
	int result = 0;
	D("yang set volume");
	fm_control_context_t* this = (fm_control_context_t*)dev;
	if (Value > -600 && Value < 240 && Value != -1)
	{
		result = hq_set_volume(Value);//设置音量
		this->FVolume = Value;
	}
	return result;
}

/****************
//1静音 0 不
//设置是否静音
****************/
static int control_context__set_mute(fm_control_device_t* dev, int Value)
{

	int result = 0;
	fm_control_context_t* this = (fm_control_context_t*)dev;
	if(Value != this->FMute)
	{
		this->FMute = Value;
		result = hq_set_mute(Value);
		return result;
	}
	return result;
}

/****************
//设置模式  不需要给驱动发送
//0是FM; 1是AM
****************/
static int control_context__set_mode(fm_control_device_t* dev, int Value)
{

    D("yang To DISABLE fm facility.");
	int result = 0;
	fm_control_context_t* this = (fm_control_context_t*)dev;
	if(Value != this->FRadioType && Value < rtCount)
	{
		D("yang jin qi");
		this->FRadioType = Value;
		hq_set_fmam(Value); //调用底层fmam
		return result;
	}
	return result;
}





/*****************
*功能：单步
*参数：ANext(1上，0下)
*
*****************/
int Seek(fm_control_device_t* dev, bool ANext)
{
	
    	D("yang Seekseek.");
	fm_control_context_t* this = (fm_control_context_t*)dev;
  	struct TRadioParam *Param = &this->FParams[this->FRadioType];//获得当前频率
  	int Offset = ANext ? Param->Step : - Param->Step; //获得调幅
  	int Freq = Param->Freq;//当前频率
  	bool result = 0;
	this->FIsSeeking = TRUE; //标志位
	Freq += Offset;
	result = control_context__set_freq(dev, Freq);

	this->FIsSeeking = FALSE;
  return result;
}

/**************
*功能:设置频率
*
*************/
int SetFreq(fm_control_device_t* dev, int currentFreq)
{
		
	D("yang yang set freq");
	int result = 0;
	result = control_context__set_freq(dev, currentFreq);
	return result;
}

/**************
*功能:
*
*************/
static int DoRadioSeek(fm_control_device_t* dev, bool Next)
{

	int result = 0;
	control_context__set_mute(dev, TRUE);
	Seek(dev, Next ? TRUE : FALSE);
	control_context__set_mute(dev, FALSE);
	return 0;
}

/******************
*功能：使能函数
*
******************/
static int  control_context__enable_fm_facility(fm_control_device_t* dev, bool is_enable )
{
    int result = 0;
    fm_control_context_t* this = (fm_control_context_t*)dev;

    if ( is_enable )
    {
        if ( !this->is_fm_facility_enabled ) {
	    if ( (result = hq_fm_init() ) != 0 )
            {
                E("Fail to init 'FM primitive'.");
                goto EXIT;
            }
	D("ENABLE yang");
		
            this->is_fm_facility_enabled = TRUE;
        }
    }
    else
    {
        D("To DISABLE fm facility.");
        if ( this->is_fm_facility_enabled )
        {
            hq_fm_uninit();
            this->is_fm_facility_enabled = FALSE;
        }
    }

EXIT:
    return result;
}

/*****************
*设置收音机电台频率
*
******************/
static int DoDCSetCurrentFreq(fm_control_device_t* dev, int freq, int direct)
{
	int result = 0;
	
	D("yang   do  set freq  %d  %d", freq,direct);
	fm_control_context_t* this = (fm_control_context_t*)dev;
	if (this->FRadioEngine.FAutoScanning)
	{
		this->FRadioEngine.FAutoScanning = FALSE;
		//Sleep(500);
	}
	control_context__set_mode(dev, direct);
	result = SetFreq(dev, freq);
  return result;
}

/*******************
 *设置收音机电台音量
 *
******************/
int DoDCSetVolumeSetVolume(fm_control_device_t* dev, int volume)
{

	D("yang  set volume");
  int result = 0;
	result = control_context__set_volume(dev, volume);
  if(result != 0)
  {
    goto EXIT;
  }
  EXIT:
  return result;
}

/************
 *功能：向上微调
 *1 上 0 下
************/
static int DoDCStepUp(fm_control_device_t* dev)
{

  int result = 0;

	D("yang yang set freq");
  fm_control_context_t* this = (fm_control_context_t*)dev;
	if(this->FRadioEngine.FAutoScanning)
	{
			this->FRadioEngine.FAutoScanning = FALSE;
			//Sleep(500);
	}
	result = DoRadioSeek(dev, 1);
  return result;
}

/*************
 *功能：向下微调
 *
*************/
static int DoDCStepDown(fm_control_device_t* dev)
{
  int result = 0;

	D("yang yang set freq");
  fm_control_context_t* this = (fm_control_context_t*)dev;
	if(this->FRadioEngine.FAutoScanning)
	{
			this->FRadioEngine.FAutoScanning = FALSE;
			//Sleep(500);
	}
	result = DoRadioSeek(dev, 0);
  return  result;
}


/*************
*功能：自动搜索
*
*************/
int DoDCSearch(fm_control_device_t* dev)
{
  int result = 0;
  fm_control_context_t* this = (fm_control_context_t*)dev;
  if(this->FRadioEngine.FAutoScanning){
    E("yang hq_auto_scan ");
    this->FRadioEngine.FAutoScanning = FALSE;
   }
  else
  {	
    D("yang hq_auto_scan")	
    this->FRadioEngine.FAutoScanning = TRUE;
    result =  hq_auto_scan();
  }
  return result;
}

/*************
*功能：获取扫台结果频率数组
*
*************/
int  DoGetFreq(fm_control_device_t* dev, int* buffer)
{
	int result = 0;
	int i;
	D("yang DoGetFreq");
	fm_control_context_t* this = (fm_control_context_t*)dev;
	result = hq_get_quality_data(this->FreqBuffer);
		
	//是否添加个数；
	for(i=0; i<18; i++){
		*buffer = this->FreqBuffer[i];
		D("yang %d  FreqBuffer %d",i , this->FreqBuffer[i]);
		buffer++;
	}		

	if(result != 0)
		return -1;
	
	return 0;
	//return	this->FreqBuffer; 
}

/*************
*功能:清空搜台结果
*
*************/
int clear_FreqBuffer(fm_control_device_t* dev)
{
	int result = 0;
	int i;
	fm_control_context_t* this = (fm_control_context_t*)dev;
	for(i=0; i<18; i++)
	{	
		this->FreqBuffer[i] = 0;
	}	
	return result;
}

static int control_context__close(struct hw_device_t *dev)
{
	fm_control_context_t* this = (fm_control_context_t*)dev;

	if ( NULL == this )
	{
		return 0;
	}
	/* 释放 "*this" 本体. */
	free(this);

	return 0;
}

static int hq_fm_device_open(const struct hw_module_t* module, const char* name,  struct hw_device_t** device)
{
	int  result = 0;
	int i;
	/* 控制设备的 heap 实例, "*device".*/
	fm_control_context_t* control_context = NULL;
	
        D("1 yang open hal");
	/* 若要 打开的是 抽象控制设备, 则 ... */
	if ( 0 == strcmp(name, FM_HARDWARE_CONTROL) )
	{
		control_context = (fm_control_context_t*)malloc(sizeof(fm_control_context_t) );
		if ( NULL == control_context )
		{
			result = -ENOMEM;
			goto EXIT;
		}
		memset(control_context, 0, sizeof(fm_control_context_t) );

		/* <初始化 "*control_context".> */
		control_context->device.common.tag		= HARDWARE_DEVICE_TAG;
		control_context->device.common.version		= 1;
		control_context->device.common.module		= (struct hw_module_t*)module;
		control_context->device.common.close		= control_context__close;
  	
		control_context->device.enable_fm_facility	= control_context__enable_fm_facility;
		control_context->device.set_freq		= DoDCSetCurrentFreq;
		control_context->device.set_volume		= DoDCSetVolumeSetVolume;
		control_context->device.set_mute		= control_context__set_mute;
		control_context->device.hq_stepup		= DoDCStepUp;
		control_context->device.hq_stepdown		= DoDCStepDown;
		control_context->device.autoSearch		= DoDCSearch;
		
		control_context->device.get_freqbuff		= DoGetFreq;

		//------------初始化相关参数-----------------
		control_context->FRadioType = rtFM;
	//	control_context->FreqBuffer
		control_context->FParams[rtFM].FreqBegin = 8750;
		control_context->FParams[rtFM].FreqEnd   = 10800;
		control_context->FParams[rtFM].Step      = 10;
		control_context->FParams[rtFM].Freq      = 8800;
		control_context->FParams[rtFM].Level     = 80;
		control_context->FParams[rtFM].Usn       = 300;
		control_context->FParams[rtFM].Wam       = 300;
		control_context->FParams[rtFM].Offset    = 100;
		control_context->FParams[rtFM].Bandwidth = 1140;
		control_context->FParams[rtFM].Modulation= 1000;
		control_context->FParams[rtFM].SoftmuteMax = 200;
	
		control_context->FParams[rtAM].FreqBegin = 531;
		control_context->FParams[rtAM].FreqEnd   = 1629;
		control_context->FParams[rtAM].Step      = 1;
		control_context->FParams[rtAM].Freq      = 540;
 		control_context->FParams[rtAM].Level     = 400;
		control_context->FParams[rtAM].Usn       = 0;
		control_context->FParams[rtAM].Wam       = 0;
		control_context->FParams[rtAM].Offset    = 20;
		control_context->FParams[rtAM].Bandwidth = 40;
		control_context->FParams[rtAM].Modulation= 700;
		control_context->FParams[rtAM].SoftmuteMax = 300;
	  	
		//搜台结果初始化
		for(i=0; i<18; i++)
		{
			control_context->FreqBuffer[i] = 0;
		}

		*device = &(control_context->device.common); 
	}
EXIT:
    if ( 0 != result )
    {
	
        control_context__close( (struct hw_device_t*)control_context);
    }
return result;
}

static struct hw_module_methods_t hq_fm_module_methods = {
    .open = hq_fm_device_open,
};

/*模块实例变量*/
fm_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.version_major = 1,
		.version_minor = 0,
		.id = HQ_FM_HARDWARE_MODULE_ID,
		.name = "FM HAL",
		.author = "yangxuesong@hopechart.com",
		.methods = &hq_fm_module_methods,
		.dso = NULL,
	},
};
