package android.os;

interface IHQFMService {
  int enableFmFacility(int isEnable);
  int setFreq(int freq, int direct);
  int setVolume(int volume);
  int setMute(int mute);
  int DoDCStepUp();
  int DoDCStepDown();
  int DoDCSearch();
  int[] GetFreqBuffer();
}

   








