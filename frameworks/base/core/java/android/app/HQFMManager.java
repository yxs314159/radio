package android.app;

import android.annotation.SdkConstant;
import android.annotation.SystemApi;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Parcel;
import android.os.Parcelable;
import android.os.RemoteException;
import android.os.IHQFMService;
import android.util.Log;


public class HQFMManager {
    private static final String TAG = "FmManager";
    private static final boolean DEBUG = true;
	
   // private static final boolean DEBUG = false;
    private static void LOG(String msg) {
        if ( DEBUG ) {
            Log.d(TAG, msg);
        }
    }
	
  	 /** FM 输出机构被系统 or 用户 禁用. */
	public static final int FM_STATE_DISABLED = -1;
	
	/** HDMI 接口被使能, 且已经同其他设备硬件连接, 但是还未被用来输出数据. */
	public static final int FM_STATE_STEREO = 1;

	public static final int FM_STATE_MONO = 0;
	
	public static final int FM_SEEKMODE_HAND = 0;

	public static final int FM_SEEKMODE_AUTO = 1;

        IHQFMService mService;

        public HQFMManager(Context ctx,IHQFMService service){
        	mService=service;
    	}


	public int enableFmFacility(int isEnable ) {
		try {
			return mService.enableFmFacility(isEnable);
        } catch (RemoteException e) {
			Log.e(TAG, "enableFmFacility() : e = " + e);
            return -1;
        }
	}
	
	//设置频率
	public int setFreq(int currentFreq,int direct){
		
		try {
			return mService.setFreq(currentFreq,direct);
		} catch (RemoteException e) {
			Log.e(TAG, "Fail to get Fm curentfreq  from FM service : " + e);
			return FM_STATE_DISABLED;
		}
	}
	
	//设置音量
	public int setVolume(int Volume){

                try {
                        return mService.setVolume(Volume);
                } catch (RemoteException e) {
                        Log.e(TAG, "Fail to get Fm curentfreq  from FM service : " + e);
                        return FM_STATE_DISABLED;
                }
        }

	//设置静音	
        public int setMute(int mute){

                try {
                        return mService.setMute(mute);
                } catch (RemoteException e) {
                        Log.e(TAG, "Fail to get Fm curentfreq  from FM service : " + e);
                        return FM_STATE_DISABLED;
                }
        }

	//向上搜索     
        public int DoDCStepUp(){

                try {
                        return mService.DoDCStepUp();
                } catch (RemoteException e) {
                        Log.e(TAG, "Fail to get Fm curentfreq  from FM service : " + e);
                        return FM_STATE_DISABLED;
                }
        }

        //向下搜索     
        public int DoDCStepDown(){

                try {
                        return mService.DoDCStepDown();
                } catch (RemoteException e) {
                        Log.e(TAG, "Fail to get Fm curentfreq  from FM service : " + e);
                        return FM_STATE_DISABLED;
                }
        }

        //自动扫描    
        public int DoDCSearch(){

                try {
                        return mService.DoDCSearch();
                } catch (RemoteException e) {
                        Log.e(TAG, "Fail to get Fm curentfreq  from FM service : " + e);
                        return FM_STATE_DISABLED;
                }
        }

	//添加读取
	public int[] GetFreqBuffer(){
		try {
			return mService.GetFreqBuffer();
		} catch (RemoteException e) {
                        Log.e(TAG, "Fail to get Fm curentfreq  from FM service : " + e);
	//		return new int[0];
		}
		return new int[0];
	}
}


