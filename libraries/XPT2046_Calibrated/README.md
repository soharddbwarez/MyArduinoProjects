# XPT2046_Calibrated Arduino touchscreen library

This library is a fork of the excellent XPT2046_Touchscreen library by Paul Stoffregen.

The only addition to this library is support for 3-point calibration as described in the paper by Fang & Chang (Texas Instruments):

http://www.ti.com/lit/an/slyt277/slyt277.pdf

See [examples/TouchCalibration/TouchCalibration.ino](examples/TouchCalibration/TouchCalibration.ino) for usage example and other helpful documentation.

The header from that example sketch follows.

    // -----------------------------------------------------------------------------
    //
    // the process of calibrating a touchscreen, at a high-level, consists of:
    //
    // (ONE-TIME)
    //   1. pick three random points a,b,c (not co-linear) in -screen- coordinates.
    //   2. for each point p, obtain the corresponding -touchscreen- coorindate p_t.
    // (ALWAYS/RUNTIME)
    //   3. with all six points (a, b, c, a_t, b_t, c_t), along with the screen
    //      height and width for the current orientation (per setRotation()), pass
    //      a calibration object to the touchscreen via calibrate().
    //
    // any subsequent calls to get touch position (getPoint() or readData()) after
    // calling calibrate() will return (x,y)-points in -screen- coordinates instead
    // of -touchscreen- coordinates -- also corrected for screen orientation!
    //
    // --
    //
    // how to use this library:
    //
    //   1. usage of this library is identical to XPT2046_Touchscreen. however, to
    //      enable calibration, after calling begin() and setRotation(), you must
    //      then call calibrate() with an appropriate TS_Calibration object as
    //      parameter. afterwards, all calls to obtain touch position (getPoint()
    //      and readData()) will return (x,y) points in screen coordinates rather
    //      than touchscreen coordinates. this sketch simply assists with obtaining
    //      the parameters required to construct a TS_Calibration object. these
    //      parameters are fixed, hardware descriptors that should never change for
    //      a given touchscreen. so once the parameters are obtained one time, you
    //      should never have to calculate them again unless your screen is moved
    //      or removed for some reason.
    //
    // --
    //
    // how to use this sketch:
    //
    //   1. the sketch below assists performing steps 1-2 above. simply update the
    //      GPIO pin defines below per your configuration, ensure the
    //      VERIFY_CALIBRATION macro is commented out, and then compile and flash.
    //
    //   2. once running, touch the center of each crosshair. you will see the
    //      needed point-translation mapping drawn to screen (and also printed to
    //      serial console if open). the point on the left-hand side of each line is
    //      the -screen- coordinate of the crosshair, and on the right-hand side is
    //      the -touchscreen- coordinate of the last touch.
    //
    //   3. once all three lines have been drawn, substitute your -touchscreen-
    //      coordinates into the _touchPoint[] array below, uncomment the
    //      VERIFY_CALIBRATION macro below, and then compile and flash.
    //
    //   4. now when you touch the crosshairs, both points listed on each line will
    //      be in -screen- coordinates -- one for the crosshair and one for the
    //      touch! verify that the distance between crosshair and touch coordinate
    //      approaches zero as you approach closer to the center of the crosshair.
    //
    // be sure to use something pokey, fine-tipped to touch the crosshair center.
    //
    // -----------------------------------------------------------------------------
