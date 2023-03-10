/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
 */

#include <tcMenu.h>
#include "stm32DuinoDemo_menu.h"
#include "ThemeMonoInverse.h"

// Global variable declarations
const  ConnectorLocalInfo applicationInfo = { "stm32DuinoDemo", "ecd5607f-55eb-4252-a512-aab769452dd3" };
TcMenuRemoteServer remoteServer(applicationInfo);
HalStm32EepromAbstraction glBspRom;
EepromAuthenticatorManager authManager(4);
U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI gfx(U8G2_R0, PF13, PD15, PF12);
U8g2Drawable gfxDrawable(&gfx);
GraphicsDeviceRenderer renderer(30, applicationInfo.name, &gfxDrawable);
EthernetServer server(3333);
EthernetInitialisation ethernetInitialisation(&server);
EthernetTagValTransport ethernetTransport;
TagValueRemoteServerConnection ethernetConnection(ethernetTransport, ethernetInitialisation);

// Global Menu Item declarations
const char enumStrDecimalStep_0[] = "1x";
const char enumStrDecimalStep_1[] = "2x";
const char enumStrDecimalStep_2[] = "4x";
const char* const enumStrDecimalStep[]  = { enumStrDecimalStep_0, enumStrDecimalStep_1, enumStrDecimalStep_2 };
const EnumMenuInfo minfoDecimalStep = { "Decimal Step", 16, 23, 2, onDecimalStepChange, enumStrDecimalStep };
EnumMenuItem menuDecimalStep(&minfoDecimalStep, 0, NULL, INFO_LOCATION_PGM);
const char pgmStrRuntimesAuthenticatorText[] = { "Authenticator" };
EepromAuthenticationInfoMenuItem menuRuntimesAuthenticator(pgmStrRuntimesAuthenticatorText, NO_CALLBACK, 15, NULL);
const char pgmStrRuntimesIoTMonitorText[] = { "IoT Monitor" };
RemoteMenuItem menuRuntimesIoTMonitor(pgmStrRuntimesIoTMonitorText, 14, &menuRuntimesAuthenticator);
ListRuntimeMenuItem menuRuntimesCustomList(13, 0, fnRuntimesCustomListRtCall, &menuRuntimesIoTMonitor);
RENDERING_CALLBACK_NAME_INVOKE(fnRuntimesTextRtCall, textItemRenderFn, "Text", 18, NO_CALLBACK)
TextMenuItem menuRuntimesText(fnRuntimesTextRtCall, "", 12, 5, &menuRuntimesCustomList);
const SubMenuInfo minfoRuntimes = { "Runtimes", 11, 0xffff, 0, NO_CALLBACK };
BackMenuItem menuBackRuntimes(&minfoRuntimes, &menuRuntimesText, INFO_LOCATION_PGM);
SubMenuItem menuRuntimes(&minfoRuntimes, &menuBackRuntimes, &menuDecimalStep, INFO_LOCATION_PGM);
extern char ramDataSet[];
RENDERING_CALLBACK_NAME_INVOKE(fnMoreItemsScrollRtCall, enumItemRenderFn, "Scroll", -1, NO_CALLBACK)
ScrollChoiceMenuItem menuMoreItemsScroll(10, fnMoreItemsScrollRtCall, 0, ramDataSet, 10, 5, NULL);
const FloatMenuInfo minfoMoreItemsNumber = { "Number", 9, 0xffff, 2, NO_CALLBACK };
FloatMenuItem menuMoreItemsNumber(&minfoMoreItemsNumber, 0.0, &menuMoreItemsScroll, INFO_LOCATION_PGM);
const AnyMenuInfo minfoMoreItemsPressMe = { "Save", 8, 0xffff, 0, saveWasPressed };
ActionMenuItem menuMoreItemsPressMe(&minfoMoreItemsPressMe, &menuMoreItemsNumber, INFO_LOCATION_PGM);
const BooleanMenuInfo minfoMoreItemsPower = { "Power", 7, 17, 1, NO_CALLBACK, NAMING_ON_OFF };
BooleanMenuItem menuMoreItemsPower(&minfoMoreItemsPower, false, &menuMoreItemsPressMe, INFO_LOCATION_PGM);
const BooleanMenuInfo minfoMoreItemsToppings = { "Toppings", 6, 16, 1, NO_CALLBACK, NAMING_YES_NO };
BooleanMenuItem menuMoreItemsToppings(&minfoMoreItemsToppings, false, &menuMoreItemsPower, INFO_LOCATION_PGM);
const char enumStrMoreItemsOptions_0[] = "Pizza";
const char enumStrMoreItemsOptions_1[] = "Pasta";
const char enumStrMoreItemsOptions_2[] = "Salad";
const char* const enumStrMoreItemsOptions[]  = { enumStrMoreItemsOptions_0, enumStrMoreItemsOptions_1, enumStrMoreItemsOptions_2 };
const EnumMenuInfo minfoMoreItemsOptions = { "Options", 5, 14, 2, NO_CALLBACK, enumStrMoreItemsOptions };
EnumMenuItem menuMoreItemsOptions(&minfoMoreItemsOptions, 0, &menuMoreItemsToppings, INFO_LOCATION_PGM);
const SubMenuInfo minfoMoreItems = { "More Items", 4, 0xffff, 0, NO_CALLBACK };
BackMenuItem menuBackMoreItems(&minfoMoreItems, &menuMoreItemsOptions, INFO_LOCATION_PGM);
SubMenuItem menuMoreItems(&minfoMoreItems, &menuBackMoreItems, &menuRuntimes, INFO_LOCATION_PGM);
RENDERING_CALLBACK_NAME_INVOKE(fnLgeNumRtCall, largeNumItemRenderFn, "Lge Num", 6, largeNumDidChange)
EditableLargeNumberMenuItem menuLgeNum(fnLgeNumRtCall, LargeFixedNumber(9, 3, 0U, 0U, false), 3, true, &menuMoreItems);
const AnalogMenuInfo minfoHalves = { "Halves", 2, 4, 255, NO_CALLBACK, 0, 2, "dB" };
AnalogMenuItem menuHalves(&minfoHalves, 0, &menuLgeNum, INFO_LOCATION_PGM);
const AnalogMenuInfo minfoDecimal = { "Decimal", 1, 2, 1000, decimalDidChange, 0, 10, "d" };
AnalogMenuItem menuDecimal(&minfoDecimal, 0, &menuHalves, INFO_LOCATION_PGM);

void setupMenu() {
    // First we set up eeprom and authentication (if needed).
    setSizeBasedEEPROMStorageEnabled(false);
    glBspRom.initialise(0);
    menuMgr.setEepromRef(&glBspRom);
    authManager.initialise(menuMgr.getEepromAbstraction(), 150);
    menuMgr.setAuthenticator(&authManager);
    // Code generated by plugins.
    gfx.begin();
    renderer.setUpdatesPerSecond(5);
    switches.init(internalDigitalIo(), SWITCHES_POLL_EVERYTHING, true);
    menuMgr.initForEncoder(&renderer, &menuDecimal, PC8, PC10, PC9);
    remoteServer.addConnection(&ethernetConnection);
    renderer.setTitleMode(BaseGraphicalRenderer::TITLE_ALWAYS);
    renderer.setUseSliderForAnalog(false);
    installMonoInverseTitleTheme(renderer, MenuFontDef(nullptr, 1), MenuFontDef(u8g2_font_prospero_bold_nbp_tr, 1), true);

    // We have an IoT monitor, register the server
    menuRuntimesIoTMonitor.setRemoteServer(remoteServer);

    // We have an EEPROM authenticator, it needs initialising
    menuRuntimesAuthenticator.init();
}

