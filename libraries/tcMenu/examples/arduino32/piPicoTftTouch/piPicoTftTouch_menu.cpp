/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
 */

#include <tcMenu.h>
#include "piPicoTftTouch_menu.h"
#include "ThemeCoolBlueModern.h"
#include <Fonts/OpenSansCyrillicLatin18.h>
#include <Fonts/RobotoMedium24.h>

// Global variable declarations
const  ConnectorLocalInfo applicationInfo = { "RPiPico Tft Encoder", "0c6972c4-addb-4931-a910-e4b2a85d2b2a" };

TFT_eSPI gfx;
TfteSpiDrawable gfxDrawable(&gfx, 45);
GraphicsDeviceRenderer renderer(30, applicationInfo.name, &gfxDrawable);
iotouch::TftSpiTouchInterrogator touchInterrogator(&gfx, 4096, 4096, true);
iotouch::TouchOrientationSettings touchOrientation(true, false, false);
MenuTouchScreenManager touchScreen(&touchInterrogator, &renderer, touchOrientation);
tcextras::IoaTouchScreenCalibrator touchCalibrator(&touchScreen, &renderer, 400);

// Global Menu Item declarations
ListRuntimeMenuItem menuRootList(17, 0, fnRootListRtCall, NULL);
const AnyMenuInfo minfoDialogs = { "Dialogs", 12, 0xffff, 0, onShowDialogs };
ActionMenuItem menuDialogs(&minfoDialogs, &menuRootList, INFO_LOCATION_PGM);
RENDERING_CALLBACK_NAME_OVERRIDDEN(fnStatusInfoRtCall, infoRenderingRtCall, "Info", -1)
TextMenuItem menuStatusInfo(fnStatusInfoRtCall, "", 19, 5, NULL);
AnyMenuInfo minfoStatusRestart = { "Restart", 11, 0xffff, 0, onRestart };
ActionMenuItem menuStatusRestart(&minfoStatusRestart, &menuStatusInfo, INFO_LOCATION_RAM);
const FloatMenuInfo minfoStatusLineVoltage = { "Line Voltage", 10, 0xffff, 2, NO_CALLBACK };
FloatMenuItem menuStatusLineVoltage(&minfoStatusLineVoltage, 235.0, &menuStatusRestart, INFO_LOCATION_PGM);
const BooleanMenuInfo minfoStatusAmpPower = { "Amp Power", 9, 0xffff, 1, NO_CALLBACK, NAMING_ON_OFF };
BooleanMenuItem menuStatusAmpPower(&minfoStatusAmpPower, false, &menuStatusLineVoltage, INFO_LOCATION_PGM);
const char enumStrStatusAmpStatus_0[] = "Standby";
const char enumStrStatusAmpStatus_1[] = "Warm up";
const char enumStrStatusAmpStatus_2[] = "DC protect";
const char enumStrStatusAmpStatus_3[] = "Overload";
const char enumStrStatusAmpStatus_4[] = "Overheat";
const char* const enumStrStatusAmpStatus[]  = { enumStrStatusAmpStatus_0, enumStrStatusAmpStatus_1, enumStrStatusAmpStatus_2, enumStrStatusAmpStatus_3, enumStrStatusAmpStatus_4 };
const EnumMenuInfo minfoStatusAmpStatus = { "Amp Status", 8, 0xffff, 4, NO_CALLBACK, enumStrStatusAmpStatus };
EnumMenuItem menuStatusAmpStatus(&minfoStatusAmpStatus, 0, &menuStatusAmpPower, INFO_LOCATION_PGM);
const SubMenuInfo minfoStatus = { "Status", 7, 0xffff, 0, NO_CALLBACK };
BackMenuItem menuBackStatus(&minfoStatus, &menuStatusAmpStatus, INFO_LOCATION_PGM);
SubMenuItem menuStatus(&minfoStatus, &menuBackStatus, &menuDialogs, INFO_LOCATION_PGM);
RENDERING_CALLBACK_NAME_INVOKE(fnSettingsNlgeRtCall, largeNumItemRenderFn, "NLge", -1, NO_CALLBACK)
EditableLargeNumberMenuItem menuSettingsNlge(fnSettingsNlgeRtCall, LargeFixedNumber(8, 0, 0U, 0U, false), 18, true, NULL);
RENDERING_CALLBACK_NAME_INVOKE(fnSettingsLargeNumRtCall, largeNumItemRenderFn, "LargeNum", -1, NO_CALLBACK)
EditableLargeNumberMenuItem menuSettingsLargeNum(fnSettingsLargeNumRtCall, LargeFixedNumber(8, 3, 100U, 500U, false), 16, false, &menuSettingsNlge);
RENDERING_CALLBACK_NAME_INVOKE(fnSettingsRGBRtCall, rgbAlphaItemRenderFn, "RGB", -1, NO_CALLBACK)
Rgb32MenuItem menuSettingsRGB(fnSettingsRGBRtCall, RgbColor32(0, 0, 0), 15, false, &menuSettingsLargeNum);
RENDERING_CALLBACK_NAME_INVOKE(fnSettingsIPRtCall, ipAddressRenderFn, "IP", 19, NO_CALLBACK)
IpAddressMenuItem menuSettingsIP(fnSettingsIPRtCall, IpAddressStorage(192, 168, 0, 33), 14, &menuSettingsRGB);
RENDERING_CALLBACK_NAME_INVOKE(fnSettingsTextRtCall, textItemRenderFn, "Text", 9, NO_CALLBACK)
TextMenuItem menuSettingsText(fnSettingsTextRtCall, "hello", 13, 10, &menuSettingsIP);
const BooleanMenuInfo minfoSettingsProtection = { "Protection", 6, 0xffff, 1, NO_CALLBACK, NAMING_ON_OFF };
BooleanMenuItem menuSettingsProtection(&minfoSettingsProtection, false, &menuSettingsText, INFO_LOCATION_PGM);
const AnalogMenuInfo minfoSettingsMaxOnVolume = { "Max On Volume", 5, 7, 255, NO_CALLBACK, -180, 2, "dB" };
AnalogMenuItem menuSettingsMaxOnVolume(&minfoSettingsMaxOnVolume, 144, &menuSettingsProtection, INFO_LOCATION_PGM);
const SubMenuInfo minfoSettings = { "Settings", 4, 0xffff, 0, NO_CALLBACK };
BackMenuItem menuBackSettings(&minfoSettings, &menuSettingsMaxOnVolume, INFO_LOCATION_PGM);
SubMenuItem menuSettings(&minfoSettings, &menuBackSettings, &menuStatus, INFO_LOCATION_PGM);
const BooleanMenuInfo minfoMute = { "Mute", 3, 4, 1, NO_CALLBACK, NAMING_TRUE_FALSE };
BooleanMenuItem menuMute(&minfoMute, false, &menuSettings, INFO_LOCATION_PGM);
const char enumStrInput_0[] = "CD Player";
const char enumStrInput_1[] = "Computer USB";
const char enumStrInput_2[] = "Turntable";
const char enumStrInput_3[] = "Line 1";
const char* const enumStrInput[]  = { enumStrInput_0, enumStrInput_1, enumStrInput_2, enumStrInput_3 };
const EnumMenuInfo minfoInput = { "Input", 2, 5, 3, NO_CALLBACK, enumStrInput };
EnumMenuItem menuInput(&minfoInput, 2, &menuMute, INFO_LOCATION_PGM);
const AnalogMenuInfo minfoVolume = { "Volume", 1, 2, 255, onVolumeChanged, -180, 2, "dB" };
AnalogMenuItem menuVolume(&minfoVolume, 80, &menuInput, INFO_LOCATION_PGM);

void setupMenu() {
    // First we set up eeprom and authentication (if needed).
    setSizeBasedEEPROMStorageEnabled(false);
    // Code generated by plugins.
    gfx.begin();
    gfx.setRotation(1);
    renderer.setUpdatesPerSecond(15);
    touchScreen.start();
    menuMgr.initWithoutInput(&renderer, &menuVolume);
    renderer.setTitleMode(BaseGraphicalRenderer::TITLE_FIRST_ROW);
    renderer.setUseSliderForAnalog(true);
    renderer.enableTcUnicode();
    installCoolBlueModernTheme(renderer, MenuFontDef(&OpenSansCyrillicLatin18, 0), MenuFontDef(&RobotoMedium24, 1), false);
}
