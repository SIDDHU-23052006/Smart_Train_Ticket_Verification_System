#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "esp_http_server.h"
#include "dl_image.hpp"
#include "human_face_detect_msr01.hpp"

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// ==== CHANGE WiFi CREDENTIALS ====
const char* ssid = "Oppo A58";
const char* password = "dsdrk@29";

// Person detection
HumanFaceDetectMSR01 detector;
int person_count = 0;

// Camera server
httpd_handle_t camera_httpd = NULL;

static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  char * part_buf[64];
  static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=frame";
  static const char* _STREAM_BOUNDARY = "\r\n--frame\r\n";
  static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK) return res;

  while (true) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
      break;
    }

    // Run detection
    dl::image::image_t img;
    img.width = fb->width;
    img.height = fb->height;
    img.data = fb->buf;
    auto results = detector.infer(&img);

    if (results.size() > 0) {
      person_count = results.size(); // count people detected
      Serial.printf("Detected persons: %d\n", person_count);
    }

    size_t jpg_buf_len = fb->len;
    uint8_t * jpg_buf = fb->buf;

    res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    if (res == ESP_OK) {
      snprintf((char *)part_buf, 64, _STREAM_PART, jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, strlen((char *)part_buf));
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)jpg_buf, jpg_buf_len);
    }
    if (res != ESP_OK) {
      break;
    }

    esp_camera_fb_return(fb);
  }
  return res;
}

void startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };
  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &stream_uri);
  }
}

void setup() {
  Serial.begin(115200);
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());

  startCameraServer();
}

void loop() {
  delay(1000);
  Serial.printf("Persons detected (last frame): %d\n", person_count);
}
