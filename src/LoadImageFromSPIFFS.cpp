#include <Arduino.h>
#include <lvgl.h>
#include <SPIFFS.h>
#include <esp32-hal-log.h>

static const char * TAG = "SPIFFS";

static lv_img_dsc_t dsc;

void lv_img_set_src_from_spiffs(lv_obj_t * obj, const char * path) {
    // Load file from SD Card to PSRAM
    ESP_LOGV(TAG, "start open file at %s", path);
    File f = SPIFFS.open(path);
    if (!f) {
        ESP_LOGE(TAG, "open file %s fail", path);
        return;
    }

    ESP_LOGV(TAG, "malloc buffer file %s", path);
    size_t file_size = f.size();
    uint8_t * buff = (uint8_t *) ps_malloc(file_size);
    if (!buff) {
        ESP_LOGE(TAG, "malloc file %s fail", path);
        f.close();
        return;
    }

    ESP_LOGV(TAG, "read file %s length %d bytes", f.name(), file_size);
    size_t byte_read = f.readBytes((char *) buff, file_size);
    if (byte_read != file_size) {
        ESP_LOGE(TAG, "read file size %d but can read only %d bytes", file_size, byte_read);
        free(buff);
        f.close();
        return;
    }

    ESP_LOGV(TAG, "close file %s", f.name());
    f.close();

    dsc.header.always_zero = 0;
    dsc.header.w = 0; // Auto width after decode
    dsc.header.h = 0; // Auto hight after decode
    dsc.header.cf = 0; // Auto color format after decode
    dsc.data_size = file_size;
    dsc.data = buff;
    lv_img_set_src(obj, &dsc);
}

