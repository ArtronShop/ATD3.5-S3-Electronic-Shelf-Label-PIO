#include <Arduino.h>
#include <lvgl.h>
#include <ATD3.5-S3.h>
#include "gui/ui.h"
#include <WiFi.h>
#include <WebServer.h>
#include <uri/UriRegex.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define LED_Y_PIN (5)

const char * ssid = "CYD1";
const char * password = "12345678";

WebServer server(80);

File fsUploadFile;

lv_obj_t * qr;
void ui_update() ;

void handleCreate() {
  JsonDocument doc;

  doc["name"] = server.arg("name");
  doc["detail"] = server.arg("detail");
  doc["price"] = server.arg("price").toInt();
  doc["code"] = server.arg("code");

  serializeJson(doc, Serial);
  Serial.println();

  {
    File f = SPIFFS.open("/configs.json", "w");
    serializeJson(doc, f);
    f.close();
  }

  server.send(200, "text/plain", "OK");

  ui_update();
}

void handleFileUpload() {
  /* if (!server.hasArg("image")) {
    return;
  } */

  HTTPUpload &upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = "/image.png";
    Serial.print("handleFileUpload Name: ");
    Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    Serial.print("handleFileUpload Data: "); Serial.println(upload.currentSize);
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
    Serial.print("handleFileUpload Size: ");
    Serial.println(upload.totalSize);
  }
}

void ui_update() {
  JsonDocument doc;

  File f = SPIFFS.open("/configs.json", "r");
  deserializeJson(doc, f);

  if (!doc["name"].isNull()) {
    lv_label_set_text(ui_product_name, doc["name"].as<const char *>());
  } else {
    lv_label_set_text(ui_product_name, "ตัวอย่าง");
  }

  if (!doc["detail"].isNull()) {
    lv_label_set_text(ui_product_detail, doc["detail"].as<const char *>());
  } else {
    lv_label_set_text(ui_product_detail, "รายละเอียด");
  }

  if (!doc["price"].isNull()) {
    lv_label_set_text_fmt(ui_product_price, "%d.-", doc["price"].as<int>());
  } else {
    lv_label_set_text(ui_product_price, "0.-");
  }

  String barcode = "0000000000";
  if (!doc["barcode"].isNull()) {
    barcode = doc["barcode"].as<String>();
  } else {
    barcode = "";
  }

  lv_qrcode_update(qr, barcode.c_str(), barcode.length());
  lv_obj_center(qr);

  extern void lv_img_set_src_from_spiffs(lv_obj_t * obj, const char * path) ;
  lv_img_set_src_from_spiffs(ui_product_image, "/image.png");
}

void setup() {
  Serial.begin(115200);
  
  // Setup peripherals
  Display.begin(0); // rotation number 0
  Touch.begin();
  Sound.begin();
  // Card.begin(); // uncomment if you want to Read/Write/Play/Load file in MicroSD Card
  SPIFFS.begin(true);

  // Map peripheral to LVGL
  Display.useLVGL(); // Map display to LVGL
  Touch.useLVGL(); // Map touch screen to LVGL
  Sound.useLVGL(); // Map speaker to LVGL
  // Card.useLVGL(); // Map MicroSD Card to LVGL File System

  // Display.enableAutoSleep(120); // Auto off display if not touch in 2 min
  
  // Add load your UI function
  ui_init();

  qr = lv_qrcode_create(ui_product_barcode_box, 80, lv_color_hex(0x000000), lv_color_hex(0xFFFFFF));

  ui_update();

  // Add event handle

  // HTTP
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    extern const char * html_page;
    server.send(200, "text/html", html_page);
  });
  server.on("/save", HTTP_POST, handleCreate, handleFileUpload);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  Display.loop(); // Keep GUI work
  server.handleClient(); // Keep Web Configs work
}
