package com.android.server;

import android.content.Context;
import android.os.IHQFMService;
import android.util.Slog;
import android.util.Log;

public class HQFMService extends IHQFMService.Stub {

private static final String TAG = "HQFMService";


private Context mContext;

public int enableFmFacility(int isEnable ) {
	int result = native_enableFmFacility(isEnable);
	if ( result != 0 ) {
	//	Log.e(TAG, "enableFmFacility() : Fail to " + (isEnable ? "enable" : "disable") + "the whole FM facility.");
		return result;
	}
	return 0;
}



//使能
public  HQFMService(Context context){
	//		LOG("yang FmService() : FmService starting!.");
		
		mContext = context;

		return;
//	native_FmOpen();
          //      int result;
 //                native_enableFmFacility(1);
	//	return result;
}


public int setFreq(int freq, int direct){
	int result = native_setFmFreq(freq, direct);
	if ( result != 0 ) {
			//Log.e(TAG, "enableFmFacility() : Fail to " + (isEnable ? "enable" : "disable") + "the whole FM facility.");
		return result;
	}
return 0;
}


public int setVolume(int volume){
	int result = native_setFmVolume(volume);
	if ( result != 0 ) {
		//	Log.e(TAG, "enableFmFacility() : Fail to " + (isEnable ? "enable" : "disable") + "the whole FM facility.");
		return result;
	}
return 0;
}

public int setMute(int mute){
	int result = native_setFmMute(mute);
	if ( result != 0 ) {
		//	Log.e(TAG, "enableFmFacility() : Fail to " + (isEnable ? "enable" : "disable") + "the whole FM facility.");
	return result;
	}
return 0;
}

public int setMode(int mode){
	int result = native_setFmMode(mode);
	if ( result != 0 ) {
	//	Log.e(TAG, "enableFmFacility() : Fail to " + (isEnable ? "enable" : "disable") + "the whole FM facility.");
	return result;
	}
return 0;
}

public int DoDCStepUp(){
	int result = native_DoDCStepUp();
	if ( result != 0 ) {
//		Log.e(TAG, "enableFmFacility() : Fail to " + (isEnable ? "enable" : "disable") + "the whole FM facility.");
	return result;
	}
return 0;
}

public int DoDCStepDown(){
	int result = native_DoDCStepDown();
	if ( result != 0 ) {
//		Log.e(TAG, "enableFmFacility() : Fail to " + (isEnable ? "enable" : "disable") + "the whole FM facility.");
	return result;
	}
return 0;
}

public int DoDCSearch(){
	int result = native_DoDCSearch();
	if ( result != 0 ) {
	//	Log.e(TAG, "enableFmFacility() : Fail to " + (isEnable ? "enable" : "disable") + "the whole FM facility.");
	return result;
	}
return 0;
}

public int[] GetFreqBuffer(){
	return native_GetFreqBuffer();
}

private static native int  native_FmOpen();
private static native int native_enableFmFacility(int isEnable);
private static native int native_setFmFreq(int currentfreq,int direct);
private native int native_setFmVolume(int Volume);
private native int native_setFmMute(int mute);
private native int native_setFmMode(int mode);
private native int native_DoDCStepUp();
private native int native_DoDCStepDown();
private native int native_DoDCSearch();
private native int[] native_GetFreqBuffer();

//添加读取


}





























	















		








