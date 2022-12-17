#include "TcMenuDCLoad_menu.h"
#include <BaseDialog.h>
#include <BaseRenderers.h>
#include <EditableLargeNumberMenuItem.h>
#include <EepromItemStorage.h>
#include <GfxMenuConfig.h>
#include <MenuHistoryNavigator.h>
#include <MenuItems.h>
#include <MenuIterator.h>
#include <MessageProcessors.h>
#include <RemoteAuthentication.h>
#include <RemoteConnector.h>
#include <RemoteMenuItem.h>
#include <RemoteTypes.h>
#include <RuntimeMenuItem.h>
#include <ScrollChoiceMenuItem.h>
#include <SecuredMenuPopup.h>
#include <tcMenu.h>
#include <tcMenuKeyboard.h>
#include <tcMenuVersion.h>
#include <tcUtil.h>

// default CPP main file for sketch
#include <PlatformDetermination.h>

#include <TaskManagerIO.h>

void setup() {
    setupMenu();
}

void loop() {
    taskManager.runLoop();
}



void CALLBACK_FUNCTION TogglePin13(int id) {
    // TODO - your menu change code
}
