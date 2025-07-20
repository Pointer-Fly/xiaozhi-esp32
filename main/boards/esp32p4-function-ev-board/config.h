#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <driver/gpio.h>

// 音频配置
#define AUDIO_INPUT_SAMPLE_RATE 16000
#define AUDIO_OUTPUT_SAMPLE_RATE 16000

#define AUDIO_CODEC_ES8311_ADDR ES8311_CODEC_DEFAULT_ADDR

// 显示屏配置
#define DISPLAY_WIDTH   480
#define DISPLAY_HEIGHT  480
#define DISPLAY_MIRROR_X false
#define DISPLAY_MIRROR_Y false
#define DISPLAY_SWAP_XY false

#define DISPLAY_OFFSET_X  0
#define DISPLAY_OFFSET_Y  0



#define BOOT_BUTTON_GPIO GPIO_NUM_35

enum XiaozhiStatus {
    kDevice_null,
    kDevice_Distributionnetwork,
    kDevice_Exit_Distributionnetwork,
};




#endif // _BOARD_CONFIG_H_
