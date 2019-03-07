#ifndef ANDROID_HQ_FM_INTERFACE_H
#define ANDROID_HQ_FM_INTERFACE_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <hardware/hardware.h>
#include <stdbool.h>

#define MAX_SHORT_COUNT (6)

#define TRUE 1
#define FALSE 0

__BEGIN_DECLS

/*定义模块ID*/
#define HQ_FM_HARDWARE_MODULE_ID "hq_fm"

// Name of the hdmi control device to open
#define FM_HARDWARE_CONTROL    "control"

//声明
extern int hq_start_command(void);
extern int hq_set_freq(int freq);      //设置频率
extern int hq_set_volume(int volume); //设置音量
extern int hq_set_mute(int mute);      //设置是否静音
extern int hq_get_status(void);         //获得当前状态
extern int hq_fm_init(void);     //初始化
extern void hq_fm_uninit(void);  //关闭

extern int hq_auto_scan(void);  	   //自动搜索
extern int hq_set_fmam(int module);//设置FM/AM

extern int hq_get_quality_data(int* Data);//获取搜台数据

//硬件模块结构体
typedef struct fm_module_t {
    struct hw_module_t common;
}fm_module_t;

//app设置的频率
#pragma pack(push, 1)
struct TDCRadioFreq
{
    unsigned char Mode; // 0-FM 1-AM
    unsigned char IsSearching; // 0-unsearching 1-searching
    unsigned short Freq; // FM:freq*100(9180) AM:freq(522)
};
#pragma pack(pop)

//app设置的引音量
#pragma pack(push, 1)
struct TDCRadioVolume
{
    unsigned char Mute; //
    unsigned short Volume;
};
#pragma pack(pop)

//当前
enum TRadioType
{
		rtFM,
		rtAM,
		rtCount,
};

#pragma pack(push, 1)
struct TRadioEngine
{
		bool            FTerminated;
    bool            FWorkOn;            // 收音机启动状态
    bool            FAutoScanning;      // 是否扫描
    bool            FLogTrace;          //是否打印详细日志到logcat
    //TList          *FFreqList;          // 当前收音机类型的频率列表
    unsigned long   FCurrTick;          // 当前tick值，减少GettickCount调用次数
    unsigned long   FLastScanTick;      // 最后一次扫描Tick
    unsigned long   FLastCheckRadioTick;// 最后一次检测收音机Tick

};
#pragma pack(pop)

#pragma pack(push, 1)
struct TRadioCompare
{
		int Freq;                   // 频率
		signed short Level;         // 信号强度
		unsigned short Usn;         // 噪声水平 (AM无)
		unsigned short Wam;         // 多径干扰 (AM无)
		signed short Offset;        // 频率偏移
		unsigned short Bandwidth;   // 带宽
		unsigned short Modulation;  // 调制
		unsigned short SoftmuteMax; // 弱信号静音衰减量
		signed short Reserve;       // 保留
};
#pragma pack(pop)

//接收的数据
#pragma pack(push, 1)
struct TCmdGetQualityData
{
    unsigned short Status;
    signed short Level;         // 信号强度
    unsigned short Usn;         // 噪声水平 (AM无)
    unsigned short Wam;         // 多径干扰 (AM无)
    signed short Offset;        // 频率偏移
    unsigned short Bandwidth;   // 带宽
    unsigned short Modulation;  // 调制
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TRadioParam
{
		signed short Level;         // 信号强度
		unsigned short Usn;         // 噪声水平 (AM无)
		unsigned short Wam;         // 多径干扰 (AM无)
		signed short Offset;        // 频率偏移
		unsigned short Bandwidth;   // 带宽
		unsigned short Modulation;  // 调制
		unsigned short SoftmuteMax; // 弱信号静音衰减量
		signed short Reserve;       // 保留

		int Freq;                   // 频率
		int FreqBegin;              // 频率起始
		int FreqEnd;                // 频率结束
		int Step;                   // 微调大小
		int ShortCuts[MAX_SHORT_COUNT];
		//TList *FreqList;   链表
};
#pragma pack(pop)

//信号干扰项
#pragma pack(push, 1)
#define INTERFERERSIGNALITEMCOUNT   (3)
struct InterfererSignalItem
{
    int Freq;
    signed short Level;
};
#pragma pack(pop)

/*硬件接口结构体*/
typedef struct fm_control_device_t {
	struct hw_device_t common; // "基类子对象".
	int (*enable_fm_facility)(struct fm_control_device_t* dev, bool enable);
	void (*start_command)(struct fm_control_device_t* dev);
	int (*set_freq)(struct fm_control_device_t* dev, int freq, int direct);     //设置频率
	int (*set_volume)(struct fm_control_device_t* dev, int volume); //设置音量
	int (*set_mute)(struct fm_control_device_t* dev, int mute);     //设置是否静音
	int (*hq_stepup)(struct fm_control_device_t* dev);              //向上微调
	int (*hq_stepdown)(struct fm_control_device_t* dev);            //向下微调
	//void (*get_status)(struct hq_fm_device_t* dev,int *status);//获得当前状态
	int (*autoSearch)(struct fm_control_device_t* dev);            //自动搜索

	int (*get_freqbuff)(struct fm_control_device_t* dev, int* buffer);		//得到频率
}fm_control_device_t;

/** convenience API for opening control device */
inline int fm_control_open(const struct hw_module_t* module, fm_control_device_t** device)
{
    return module->methods->open(module,
                                 FM_HARDWARE_CONTROL,
                                 (struct hw_device_t**)device);
}

inline int fm_control_close(fm_control_device_t* device) {
    return device->common.close(&device->common);
}


__END_DECLS

#endif
