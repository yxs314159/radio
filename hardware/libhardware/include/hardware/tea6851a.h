#ifndef _TEA6851A_H
#define _TEA6851A_H

#include <linux/ioctl.h>
#include <signal.h>
#define FM_DEV "/dev/hq_fm"


int hq_start_command(void);
int hq_set_freq(int freq);      //设置频率
int  hq_set_volume(int volume); //设置音量
int hq_set_mute(int mute);      //设置是否静音
int hq_set_mode(int mode);      //设置模式
int hq_get_status(void);         //获得当前状态
int hq_set_fmam(int module);//设置AM/FM
int hq_get_quality_data(int* Data);//获取搜台结果


int hq_stepup(void);      //向上微调
int hq_stepdown(void);    //向下微调
int hq_auto_scan(void);   //自动搜索
int hq_fm_init(void);     //初始化
void hq_fm_uninit(void);  //关闭




#endif
