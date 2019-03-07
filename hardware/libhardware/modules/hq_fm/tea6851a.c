#define LOG_TAG "tea6851a.c"
#include <cutils/log.h>

//#include <linux/Hw_fm.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <hardware/tea6851a.h>
#include <hardware/Hw_fm.h>
//#include "tea6851a.h"
#include <android/log.h>
#include <android/log.h>
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/*---------------------------------------------------------------------------*/

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


#define TEA6851A_MAGIC 'L' //

#define HQ_START_COMMAND  _IOW(TEA6851A_MAGIC, 0, int)
#define HQ_SET_FREQ       _IOW(TEA6851A_MAGIC, 1, int)
#define HQ_SET_VOLUME     _IOW(TEA6851A_MAGIC, 2, int)
#define HQ_SET_MUTE       _IOW(TEA6851A_MAGIC, 3, int)
#define HQ_SET_MODE       _IOW(TEA6851A_MAGIC, 4, int)
#define HQ_DoDCStepDown   _IOW(TEA6851A_MAGIC, 7, int)
#define HQ_DoDCStepUp     _IOW(TEA6851A_MAGIC, 8, int)

//add
#define HQ_SET_FMAM	  _IOW(TEA6851A_MAGIC, 10, int)

#define HQ_Get_Quality		_IOR(TEA6851A_MAGIC, 9, int*)

#define HQ_GET_STATUS     _IOR(TEA6851A_MAGIC, 5, int)
#define HQ_AUTO_SCAN      _IOR(TEA6851A_MAGIC, 6, int)


static int fm_fd = -1;

int hq_start_command(void)
{
  int ret;
  D("yang command");
  if(fm_fd < 0)
  {
    E("no init \n");
    return -1;
  }
  ret = ioctl(fm_fd, HQ_START_COMMAND, 0);
  return ret;
}

//设置AM、FM
int hq_set_fmam(int module)
{
 int ret;
  D("yang FM/AM");
  if(fm_fd < 0)
  {
    E("no init \n");
    return -1;
  }
  ret = ioctl(fm_fd, HQ_SET_FMAM, module);
  return ret;

}

//设置频率
int hq_set_freq(int freq)
{
	
  D("yang freq");
  int ret;
  if(fm_fd < 0)
  {
    E("no init \n");
    return -1;
  }
  ret = ioctl(fm_fd, HQ_SET_FREQ, freq);
  return ret;
}

//设置音量
int  hq_set_volume(int volume)
{
	
  D("yang freq");
  int ret;
  if(fm_fd < 0)
  {
    E("no init \n");
    return -1;
  }
  ret = ioctl(fm_fd, HQ_SET_VOLUME, volume);
  return ret;
}

//设置是否静音
int hq_set_mute(int mute)
{
	
  D("yang mute");
  int ret;
  if(fm_fd < 0)
  {
    E("no init \n");
    return -1;
  }
  ret = ioctl(fm_fd, HQ_SET_MUTE, mute);
  return ret;
}

//设置模式
int hq_set_mode(int mode)
{

  D("yang mode");
  int ret;
  if(fm_fd < 0)
  {
    E("no init \n");
    return -1;
  }
  ret = ioctl(fm_fd, HQ_SET_MODE, mode);
  return ret;
}

//获得当前状态
int hq_get_status(void)
{

  D("yang status");
  int ret;
  if(fm_fd < 0)
  {
    E("no init \n");
    return -1;
  }
  ret = ioctl(fm_fd, HQ_GET_STATUS);
  return ret;
}

//获取搜台结果
int hq_get_quality_data(int* Data)
{

  D("yang data");
  int ret;
  int buffer[18] = {0};
  int i;
  if(fm_fd < 0)
  {
    E("no init \n");
    return -1;
  }
    ret = ioctl(fm_fd, HQ_Get_Quality, buffer);
   
    D("yang this ioctl 1: %d",buffer[0]);
    D("yang this ioctl 1: %d",buffer[1]);

    for(i=0; i<18; i++)
    {
	*Data = buffer[i];
	
        D("yang this ioctl 2:%d",*Data);
	Data++;
    }

    D("=======");
    D("yang this ioctl 2:%d",*Data);
   
    return ret;
    
}


//自动搜索
int hq_auto_scan(void)
{

  D("yang scam");
  if(fm_fd < 0)
  {
    E("no init \n");
    return -1;
  }
  return ioctl(fm_fd, HQ_AUTO_SCAN);
}



//初始化
int hq_fm_init(void)
{

  D("yang init");
  int ret;
  if(fm_fd != -1){
  return 0;
  }
  D("yang INIT");
  fm_fd = open(FM_DEV, O_RDWR);
	if(fm_fd < 0){
		E("yang open %s err, %s\n", FM_DEV, strerror(errno));
		return -1;
	}
D("yang COMMAN");
  ret = hq_start_command();
  if(fm_fd < 0){
    E("yang open %s err, %s\n", FM_DEV, strerror(errno));
    return -1;
  }
  return 0;
}

//关闭
void hq_fm_uninit(void)
{
  if(fm_fd == -1){
  	return;
  }

  ioctl(fm_fd, HQ_START_COMMAND, 1);
  close(fm_fd);
  fm_fd = -1;
}
