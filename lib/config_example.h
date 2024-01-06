#define DEBUG false

#define SSID "YOUR_SSID"
#define PASS "YOUR_PASSWORD"
#define TICKTICK_ID "TICKTICK_CALENDAR_ID"
// Uncomment to add weather support
// #define LATITUDE "12.345"
// #define LONGITUDE "-67.890"

#if DEBUG
#define EVENTS_HOST "http://192.168.68.105:3000/api/events"
#else
#define EVENTS_HOST "https://ticktick-events.vercel.app/api/events"
#endif
