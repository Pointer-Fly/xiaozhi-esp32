# ESP32-P4-Function-EV-Board 小智 
支持MIPI LCD以及HDMI视频输出，需要在Component config->Board Support Package->Display->Select LCD Type进行选择

## 注意事项：
- 使用vscode idf插件进行开发时，使用menuconfig的按钮会报错，目前原因未知，可以使用命令行`idf.py menuconfig`进行配置
- LT8912B支持以下分辨率，但有些显示器可能并不支持某些分辨率，选择HDMI输出时需要将颜色格式设置为`RGB888`
|800x600@60HZ|
|1280x720@60HZ|
|1280x800@60HZ|
|1920x1080@30HZ|

## 参考：
- [https://bbs.elecfans.com/jishu_2488204_1_1.html](https://bbs.elecfans.com/jishu_2488204_1_1.html)