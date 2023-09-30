#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <SD.h>
#include <SPI.h>
#include <StringSplitter.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ezTime.h>

#include "epd_driver.h"
#include "esp_adc_cal.h"
#include "lib/config.h"
#include "lib/font/poppins_r_10.h"
#include "lib/font/poppins_r_12.h"
#include "lib/images/cloud_bolt.h"
#include "lib/images/cloud_fog.h"
#include "lib/images/cloud_rain.h"
#include "lib/images/cloud_snow.h"
#include "lib/images/moon_filled.h"
#include "lib/images/storm.h"
#include "lib/images/sun_filled.h"
#include "lib/images/sun_wind.h"
#include "lib/pins.h"

JsonArray cal_events;
JsonArray todos;
JsonObject weather;
uint8_t* framebuffer = NULL;
Preferences preferences;
bool event_days[31 + 7];

enum alignment { LEFT,
                 RIGHT,
                 CENTER };

const int TITLE_BAR_Y = 35;
const int EPD_CENTER = EPD_WIDTH / 2;
const int START_HEIGHT = 85;
const int LINE_HEIGHT = 35;

class URL {
    String url;

   public:
    URL(String host) : url(host + "?") {}
    URL(char* host) : url(String(host) + "?") {}
    URL& add(char* param, char* value) {
        url = url + "&" + param + "=" + value;
        return *this;
    }
    URL& add(char* param, const char* value) {
        return this->add(param, (char*)value);
    }
    operator String() const {
        return url;
    }
};

void setup() {
    Serial.begin(115200);
    delay(1000);

    setupFrameBuffer();
    connectWiFi();
    getEvents();
    disconnectWiFi();
    drawTitleBar();
    if (!cal_events.isNull()) {
        drawEvents();
        drawCalendar();
        // drawToDo();
    }
    pushToDisplay();
    sleep();
}

void loop() {
}

void connectWiFi() {
    IPAddress dns(1, 1, 1, 1);  // Use Cloudflare DNS
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);  // switch off AP
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(SSID, PASS);

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        // TODO wifi connect failed. trying again in 1 minute
        Serial.print(".");
    }

    Serial.printf("\nConnected to '%s' with IP: %s\n", SSID, WiFi.localIP().toString().c_str());
}

void disconnectWiFi() {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi switched Off");
}

void getEvents() {
    URL url(EVENTS_HOST);

    url.add("id", TICKTICK_ID);

    if (LATITUDE && LONGITUDE) {
        url
            .add("weather", "true")
            .add("lat", LATITUDE)
            .add("lon", LONGITUDE);
    }

    if (esp_sleep_get_wakeup_cause())  // Wakeup not caused by initial start
        url.add("hash", preferences.getString("hash", "").c_str());

    Serial.println(url);

#if DEBUG
    WiFiClient client;
#else
    WiFiClientSecure client;
    client.setInsecure();
#endif

    HTTPClient http;

    http.begin(client, url);
    int status_code = http.GET();

    DynamicJsonDocument response(3072);

    DeserializationError error = deserializeJson(response, client.readString());

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    setTime(parseTime(response["timestamp"]));
    cal_events = response["events"].as<JsonArray>();
    weather = response["weather"].as<JsonObject>();
    todos = response["todo"].as<JsonArray>();

    // Store hash in memory
    preferences.putString("hash", response["hash"].as<String>());
}

// ---------------------------------- Drawing ----------------------------------

void drawTitleBar() {
    fillRect(0, TITLE_BAR_Y + 15, EPD_WIDTH, 2, 0x00);

    // Updated At
    drawString(10, TITLE_BAR_Y, dateTime("l n/j g:i a"), LEFT);

    // Weather text
    String weather_current = weather["current"].as<String>() + "°";
    String weather_high = weather["high"].as<String>() + "°";
    String weather_low = weather["low"].as<String>() + "°";
    int weather_code = weather["code"];
    drawString(EPD_CENTER - 20, TITLE_BAR_Y, weather_current, RIGHT);
    drawString(poppins_r_10, EPD_CENTER + 25, TITLE_BAR_Y - 3, weather_high, RIGHT);
    drawString(EPD_CENTER + 30, TITLE_BAR_Y, "/", CENTER);
    drawString(poppins_r_10, EPD_CENTER + 36, TITLE_BAR_Y + 3, weather_low, LEFT);

    // Weather icon
    uint8_t* weather_image = (uint8_t*)[weather_code]() {
        switch (weather_code) {
            case 0:
                return weather["isDay"].as<bool>() ? sun_filled_data : moon_filled_data;
            case 1 ... 3:
                return sun_wind_data;
            case 51 ... 65:
            case 80 ... 81:
                return cloud_rain_data;
            case 66 ... 67:
            case 71 ... 77:
            case 85 ... 86:
                return cloud_snow_data;
            case 82:
            case 95:
                return cloud_bolt_data;
            case 96 ... 99:
                return storm_data;
        }
    }
    ();
    Rect_t area = {
        .x = EPD_CENTER + 80,
        .y = 10,
        .width = 32,
        .height = 32};
    epd_copy_to_framebuffer(area, weather_image, framebuffer);

    drawBattery(EPD_WIDTH - 20, 25, RIGHT);
}

void drawEvents() {
    int y_cursor = START_HEIGHT - LINE_HEIGHT;
    uint8_t current_month;
    uint8_t current_day;

    int m2s = dateTime("t").toInt() - day();

    // Ensure all events are added to calendar highlights
    bool drawEvents = true;

    // Loop over all events
    for (JsonObject event : cal_events) {
        y_cursor += LINE_HEIGHT;
        if (y_cursor > EPD_HEIGHT) drawEvents = false;

        time_t events_time = parseTime(event["start"]);

        // New Month
        if (month(events_time) != current_month) {
            // Prevent event cutoff after new month
            if ((y_cursor + LINE_HEIGHT) > EPD_HEIGHT) drawEvents = false;

            current_month = month(events_time);
            current_day = 0;

            if (drawEvents) drawString(poppins_r_10, 30, y_cursor, dateTime(events_time, "M"), CENTER);

            y_cursor += LINE_HEIGHT + 5;
        }

        // New Day
        if (day(events_time) != current_day) {
            current_day = day(events_time);

            if (drawEvents) {
                if (current_day == day()) {
                    drawRoundedSquare(30, y_cursor - 10, 38, 4, 2, 0x00);
                }

                drawString(30, y_cursor, dateTime(events_time, "j"), CENTER);
            }

            if (month() == current_month || ((month() + 1) % 12) == current_month) {
                bool isCurrentMonth = month() == current_month;
                event_days[m2s * !isCurrentMonth + current_day - (day() * isCurrentMonth)] = true;
            }
        }

        if (drawEvents) {
            // Event title
            drawString(75, y_cursor, event["title"], LEFT, 350);

            // If event has a start time
            if (event.containsKey("end")) {
                drawString(450, y_cursor, dateTime(events_time, "g:i a"), LEFT);
            }
        }
    }
}

// void drawToDo() {
//     fillRect(600, TITLE_BAR_Y + 15, 2, EPD_HEIGHT, 0x00);

//     drawString(625, START_HEIGHT, "To-Do", LEFT);

//     int y_cursor = START_HEIGHT + LINE_HEIGHT / 2;

//     for (JsonVariant todo : todos) {
//         y_cursor += LINE_HEIGHT;
//         drawRoundedSquare(640, y_cursor - 10, 24, 4, 2, 0x00);
//         drawString(665, y_cursor, todo, LEFT, 280);
//     }
// }

void drawCalendar() {
    const int START_X = 600;
    int y_cursor = START_HEIGHT + 15;

    const char* WEEKDAY_NAMES[7] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};

    time_t t = now();
    tmElements_t tm;

    const int m2s = dateTime("t").toInt() - day();

    fillRect(START_X, TITLE_BAR_Y + 15, 2, EPD_HEIGHT, 0x80);

    while (1) {
        int weekdayOfFirst = (weekday(t) - (day(t) % 7) + 7) % 7;
        int daysInMonth = dateTime(t, "t").toInt();

        // Draw month name
        drawString(START_X + 165, y_cursor, dateTime(t, "F"), CENTER);

        // Draw days of the week
        for (int day = 0; day < 7; day++) {
            drawString(
                poppins_r_10,
                START_X + 50 + day * 42,
                y_cursor + 45,
                WEEKDAY_NAMES[day],
                CENTER);
        }

        auto firstOfWeek = [weekdayOfFirst](int date) { return (date - ((weekdayOfFirst + date - 1) % 7)); };

        int firstDay = max(firstOfWeek(day(t)) - 1, 0);
        int startingWeek = floor((weekdayOfFirst + firstDay) / 7);
        bool isCurrentMonth = month() == month(t);

        int x, y;
        // Draw day numbers
        for (int day_number = firstDay; day_number < daysInMonth; day_number++) {
            x = START_X + 50 + ((weekdayOfFirst + day_number) % 7) * 42;
            y = y_cursor + 90 + ((floor((weekdayOfFirst + day_number) / 7) - startingWeek) * 42);

            if (y > EPD_HEIGHT) return;

            if (t == now() && day_number + 1 == day()) {  // Highlight today
                drawRoundedSquare(x, y - 10, 42, 4, 2, 0x00);
            } else if (((isCurrentMonth && (day_number + 1) > day()) || !isCurrentMonth) && event_days[m2s * !isCurrentMonth + (day_number + 1) - (day() * isCurrentMonth)]) {  // Underline days with events
                fillRect(x - 13, y + 7, 28, 2, 0xAA);
            }

            drawString(
                x,
                y,
                String(day_number + 1),
                CENTER);
        }

        y_cursor = y + 60;

        breakTime(t, tm);
        tm.Month++;
        tm.Day = 1;
        t = makeTime(tm);
    }
}

// ---------------------------------- Utilities ----------------------------------

void pushToDisplay() {
    // Refresh only the title bar if events are unchanged
    Rect_t area = cal_events.isNull()
                      ? Rect_t({.x = 0,
                                .y = 0,
                                .width = EPD_WIDTH,
                                .height = TITLE_BAR_Y + 15})
                      : epd_full_screen();

    epd_init();
    epd_poweron();
    epd_clear_area(area);
    epd_draw_grayscale_image(area, framebuffer);  // Update the screen
    epd_poweroff_all();
}

void setupFrameBuffer() {
    preferences.begin("ticktick", false);

    framebuffer = (uint8_t*)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);

    if (!framebuffer) {
        Serial.println("alloc memory failed !!!");
        while (1)
            ;
    }

    memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
}

time_t parseTime(const char* iso8601String) {
    int year, month, day, hour, minute, second;
    sscanf(iso8601String, "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    return makeTime(hour, minute, second, day, month, year);
}

void error(String e) {
    // TODO implement error displaying
}

void sleep() {
    Serial.println("Going to sleep...");
    const int SLEEP_MIN = 60;
    esp_sleep_enable_timer_wakeup(SLEEP_MIN * 60 * 1000000LL);
    esp_deep_sleep_start();
}

// ------------------------------ Drawing Utilities ------------------------------

// Draw string with specified font and max width
void drawString(GFXfont const& font, int x, int y, String text, alignment align, int maxWidth) {
    char* data = const_cast<char*>(text.c_str());
    int x1, y1;  // the bounds of x,y and w and h of the variable 'text' in pixels.
    int w, h;
    int xx = x, yy = y;
    get_text_bounds((GFXfont*)&font, data, &xx, &yy, &x1, &y1, &w, &h, NULL);

    if (maxWidth && w > maxWidth) {
        do {
            text.remove(text.length() - 1);
            get_text_bounds((GFXfont*)&font, data, &xx, &yy, &x1, &y1, &w, &h, NULL);
        } while (w > maxWidth);
        text.trim();
        text += "...";
    }

    if (align == RIGHT) x = x - w;
    if (align == CENTER) x = x - w / 2;
    // int cursor_y = y + h; // make cursor anchor from bottom left
    write_string((GFXfont*)&font, data, &x, &y, framebuffer);
}

int drawMultiLineString(int x, int y, String text, alignment align, int maxWidth) {
    char* data = const_cast<char*>(text.c_str());
    int x1, y1;  // the bounds of x,y and w and h of the variable 'text' in pixels.
    int w, h;
    int xx = x, yy = y;
    GFXfont* font = (GFXfont*)&poppins_r_12;
    int lines = 1;

    StringSplitter* splitter = new StringSplitter(text, ' ', INFINITY);
    int itemCount = splitter->getItemCount();

    String multiline = "";
    String current_line = "";
    String tmp = "";

    for (int i = 0; i < itemCount; i++) {
        tmp.concat(splitter->getItemAtIndex(i) + " ");
        // Serial.println(tmp.c_str());
        get_text_bounds(font, tmp.c_str(), &xx, &yy, &x1, &y1, &w, &h, NULL);
        if (w > maxWidth) {
            multiline.concat("\n" + current_line);
            tmp = "";
            i--;
            lines++;
        } else {
            current_line = tmp;
        }
        delay(250);
    }
    multiline.concat("\n" + current_line);
    multiline.trim();

    write_string(font, multiline.c_str(), &x, &y, framebuffer);
    return lines;
}

// Draw string with specified font
void drawString(GFXfont const& font, int x, int y, String text, alignment align) {
    drawString(font, x, y, text, align, false);
}

// Draw string with default font and max width
void drawString(int x, int y, String text, alignment align, int maxWidth) {
    drawString(poppins_r_12, x, y, text, align, maxWidth);
}

// Draw string with default font and no max width
void drawString(int x, int y, String text, alignment align) {
    drawString(x, y, text, align, false);
}

void fillCircle(int x, int y, int r, uint8_t color) {
    epd_fill_circle(x, y, r, color, framebuffer);
}

void drawCircle(int x, int y, int r, uint8_t color) {
    epd_draw_circle(x, y, r, color, framebuffer);
}

void drawCircle(int x, int y, int r, int thickness, uint8_t color) {
    fillCircle(x, y, r, color);
    fillCircle(x, y, r - thickness, 0xff);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    epd_fill_rect(x, y, w, h, color, framebuffer);
}

// From the center
void fillRoundedRect(int x, int y, int w, int h, int r, uint8_t color) {
    fillRect(x - w / 2 + r, y - h / 2, w - r * 2, h + 1, color);  // top to bottom
    fillRect(x - w / 2, y - h / 2 + r, w + 1, h - r * 2, color);  // left to right

    fillCircle(x - w / 2 + r, y - h / 2 + r, r, color);  // top left
    fillCircle(x + w / 2 - r, y + h / 2 - r, r, color);  // bottom right

    if (w > r) {
        fillCircle(x + w / 2 - r, y - h / 2 + r, r, color);  // top right
    }

    if (h > r) {
        fillCircle(x - w / 2 + r, y + h / 2 - r, r, color);  // bottom left
    }
}

void drawRoundedRect(int x, int y, int w, int h, int r, int thickness, uint8_t color) {
    fillRoundedRect(x, y, w, h, r, color);
    fillRoundedRect(x, y, w - thickness * 2, h - thickness * 2, r - thickness / 2, 0xff);
}

void drawRoundedSquare(int x, int y, int w, int r, int thickness, uint8_t color) {
    drawRoundedRect(x, y, w, w, r, thickness, color);
}

void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color) {
    epd_draw_hline(x0, y0, length, color, framebuffer);
}

void drawBattery(int x, int y, alignment align) {
    const int TICK_WIDTH = 3;
    const int TICK_SPACE = 6;
    const int THICKNESS = 2;
    const int HEIGHT = 28;
    const int NUMBER_OF_TICKS = 6;

    const int MAIN_WIDTH = NUMBER_OF_TICKS * TICK_WIDTH + (NUMBER_OF_TICKS + 1) * TICK_SPACE + 2 * THICKNESS;

    switch (align) {
        case LEFT:
            x += (MAIN_WIDTH + THICKNESS) / 2;
            break;
        case RIGHT:
            x -= (MAIN_WIDTH + THICKNESS) / 2;
            break;
    }

    // Outside
    drawRoundedRect(x, y, MAIN_WIDTH, HEIGHT, THICKNESS, THICKNESS, 0x00);
    fillRoundedRect(x + (MAIN_WIDTH + THICKNESS) / 2, y, THICKNESS * 2, THICKNESS * 4, THICKNESS, 0x00);
    fillRoundedRect(x + (MAIN_WIDTH - THICKNESS) / 2, y, THICKNESS * 2, THICKNESS * 2, THICKNESS / 2, 0xff);
    fillRoundedRect(x + MAIN_WIDTH / 2 - THICKNESS, y, THICKNESS * 2, THICKNESS * 3, THICKNESS / 2, 0xff);

    int vref = 1100;
    uint8_t percentage = 100;
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        Serial.printf("eFuse Vref:%u mV\n", adc_chars.vref);
        vref = adc_chars.vref;
    }
    float voltage = analogRead(36) / 4096.0 * 6.566 * (vref / 1000.0);
    if (voltage > 1) {  // Only display if there is a valid reading
        percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
        if (voltage >= 4.20) percentage = 100;
        if (voltage <= 3.20) percentage = 0;  // orig 3.5
    }

    int BATTERY = min((int)round(percentage / (100 / NUMBER_OF_TICKS)), NUMBER_OF_TICKS);

    for (int i = 1; i <= BATTERY; i++)
        fillRect(x - MAIN_WIDTH / 2 + TICK_SPACE * i + TICK_WIDTH * i,
                 y - HEIGHT / 2 + THICKNESS + 5,
                 TICK_WIDTH,
                 HEIGHT - THICKNESS - 10,
                 0x00);
}