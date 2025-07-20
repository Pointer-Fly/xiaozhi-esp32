#include "esp_err.h"
#include "esp_check.h"
#include "esp_memory_utils.h"
#include "bsp/esp-bsp.h"
#include "bsp/display.h"
#include "driver/gpio.h"

#include "display/lcd_display.h"
#include "wifi_board.h"
#include "config.h"
#include "application.h"

#include "es8311_audio_codec.h"
#include "button.h"
#include <wifi_station.h>

#define TAG "ESP_P4_FUNCTION_EV_Board"

LV_FONT_DECLARE(font_puhui_30_4);
LV_FONT_DECLARE(font_awesome_30_4);

class ESP_P4_FUNCTION_EV_Board : public WifiBoard
{
private:
    bsp_display_cfg_t cfg;
    LcdDisplay *display_;
    i2c_master_bus_handle_t i2c_bus_;
    XiaozhiStatus XiaozhiStatus_ = kDevice_Exit_Distributionnetwork;
    lv_display_t *lv_displayp;

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_io_handle_t panel_io = nullptr;

    Button boot_button_;

    // 显示屏初始化
    void Initialize_MIPI_Dispaly()
    {
        ESP_LOGI(TAG, "Init MIPI Output");

        cfg = {
            .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
            .buffer_size = BSP_LCD_DRAW_BUFF_SIZE,
            .double_buffer = BSP_LCD_DRAW_BUFF_DOUBLE,
            .hw_cfg = {
#if CONFIG_BSP_LCD_TYPE_HDMI
#if CONFIG_BSP_LCD_HDMI_800x600_60HZ
                .hdmi_resolution = BSP_HDMI_RES_800x600,
#elif CONFIG_BSP_LCD_HDMI_1280x720_60HZ
                .hdmi_resolution = BSP_HDMI_RES_1280x720,
#elif CONFIG_BSP_LCD_HDMI_1280x800_60HZ
                .hdmi_resolution = BSP_HDMI_RES_1280x800,
#elif CONFIG_BSP_LCD_HDMI_1920x1080_30HZ
                .hdmi_resolution = BSP_HDMI_RES_1920x1080,
#endif
#else
                .hdmi_resolution = BSP_HDMI_RES_NONE,
#endif
                .dsi_bus = {
                    .phy_clk_src = MIPI_DSI_PHY_CLK_SRC_DEFAULT,
                    .lane_bit_rate_mbps = BSP_LCD_MIPI_DSI_LANE_BITRATE_MBPS,
                },
            },
            .flags = {
#if CONFIG_BSP_LCD_COLOR_FORMAT_RGB888
                .buff_dma = false,
#else
                .buff_dma = true,
#endif
                .buff_spiram = false,
#if CONFIG_BSP_LCD_TYPE_HDMI
                .sw_rotate = true,
#else
                .sw_rotate = false,
#endif
            }};

        lv_displayp = bsp_display_start_with_config(&cfg);
        bsp_display_backlight_on();

        // 创建显示屏对象
        display_ = new MipiLcdDisplay(panel_io, panel_handle,
                                      BSP_LCD_V_RES, BSP_LCD_H_RES, DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y, DISPLAY_MIRROR_X,
                                      DISPLAY_MIRROR_Y, DISPLAY_SWAP_XY,
                                      {
                                          .text_font = &font_puhui_30_4,
                                          .icon_font = &font_awesome_30_4,
                                          .emoji_font = font_emoji_64_init(),
                                      });

        display_->SetLvglDisplayp(lv_displayp);
    }

    // 按钮初始化
    void InitializeButtons()
    {
        boot_button_.OnPressDown([this]()
                                 {
            ESP_LOGI(TAG,"Button pressed down");
            auto& app = Application::GetInstance();
            app.ToggleChatState(); });

        boot_button_.OnPressUp([this]()
                               {
            ESP_LOGI(TAG,"Button pressed up");
            Application::GetInstance().StopListening();
            Application::GetInstance().SetDeviceState(kDeviceStateIdle); });
    }

public:
    ESP_P4_FUNCTION_EV_Board() : boot_button_(BOOT_BUTTON_GPIO)
    {
        bsp_i2c_init();
        i2c_bus_ = bsp_i2c_get_handle();
        Initialize_MIPI_Dispaly();
        InitializeButtons();
    }
    virtual AudioCodec *GetAudioCodec() override
    {
        static Es8311AudioCodec audio_codec(
            i2c_bus_,
            I2C_NUM_1,
            AUDIO_INPUT_SAMPLE_RATE,
            AUDIO_OUTPUT_SAMPLE_RATE,
            BSP_I2S_MCLK,
            BSP_I2S_SCLK,
            BSP_I2S_LCLK,
            BSP_I2S_DOUT,
            BSP_I2S_DSIN,
            BSP_POWER_AMP_IO,
            AUDIO_CODEC_ES8311_ADDR,
            false);
        return &audio_codec;
    }

    virtual Display *GetDisplay() override
    {
        return display_;
    }
};

DECLARE_BOARD(ESP_P4_FUNCTION_EV_Board);
