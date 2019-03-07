package com.android.server;

import android.content.Context;
import android.os.IHQFMService;
import android.util.Slog;

public class HQFMService extends IHQFMService.Stub {
private static final String TAG = "HQFMService";

private Context mContext;

/*使能函数*/
public int enableFmFacility(int isEnable) {
	int result = native_enableFmFacility(isEnable);
	if ( result != 0 ) {
		return result;
	}
	return 0;
}


public  HQFMService(Context context){
	mContext = context;
//      int result;
//      native_enableFmFacility(1);
//	return result;
        }

/*设置频率*/
public int setFreq(int freq, int direct){
	int result = native_setFmFreq(freq, direct);
	if ( result != 0 ) {
		return result;
	}
return 0;
}

/*设置音量*/
public int setVolume(int volume){
	int result = native_setFmVolume(volume);
	if ( result != 0 ) {
		return result;
	}
return 0;
}

/*设置静音*/
public int setMute(int mute){
	int result = native_setFmMute(mute);
	if ( result != 0 ) {
	return result;
	}
return 0;
}

/*向上搜索*/
public int DoDCStepUp(){
	int result = native_DoDCStepUp();
	if ( result != 0 ) {
	return result;
	}
return 0;
}

/*向下搜索*/
public int DoDCStepDown(){
	int result = native_DoDCStepDown();
	if ( result != 0 ) {
	return result;
	}
return 0;
}

/*自动搜索*/
public int DoDCSearch(){
	int result = native_DoDCSearch();
	if ( result != 0 ) {
	return result;
	}
return 0;
}

/*获取频率*/
public int[] GetFreqBuffer(){	
	return native_GetFreqBuffer();	
}

private native int native_enableFmFacility(int isEnable);
private native int native_setFmFreq(int currentfreq,int direct);
private native int native_setFmVolume(int Volume);
private native int native_setFmMute(int mute);
private native int native_DoDCStepUp();
private native int native_DoDCStepDown();
private native int native_DoDCSearch();


//添加读取
private native int[] native_GetFreqBuffer();



}





























	















		








