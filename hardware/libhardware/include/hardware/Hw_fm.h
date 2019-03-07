#ifndef _TEA6851A_H
#define _TEA6851A_H
#include <linux/ioctl.h>

#define TEA6851A_MAGIC 'L' //

#define HQ_START_COMMAND  _IOW(TEA6851A_MAGIC, 0, int)
#define HQ_SET_FREQ       _IOW(TEA6851A_MAGIC, 1, int)
#define HQ_SET_VOLUME     _IOW(TEA6851A_MAGIC, 2, int)
#define HQ_SET_MUTE       _IOW(TEA6851A_MAGIC, 3, bool)
#define HQ_SET_MODE       _IOW(TEA6851A_MAGIC, 4, int)
#define HQ_DoDCStepDown   _IOW(TEA6851A_MAGIC, 7, int)
#define HQ_DoDCStepUp     _IOW(TEA6851A_MAGIC, 8, int)

#define HQ_GET_STATUS     _IOR(TEA6851A_MAGIC, 5, int)
#define HQ_AUTO_SCAN      _IOR(TEA6851A_MAGIC, 6, int)


#endif
