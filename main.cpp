/*
 * Copyright (C) 2025 Kravchuk Roman <darkkitsunezx128k2309@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "CircleGaugeControl.h"

#include <Application.h>
#include <Catalog.h>
#include <InterfaceDefs.h>
#include <LayoutBuilder.h>
#include <Rect.h>
#include <SeparatorView.h>
#include <Slider.h>
#include <Window.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"

const uint32 MSG_CHANGE_VALUE ='cval';

class RtSlider : public BSlider {
public:
    RtSlider(BRect frame, const char *name, const char *label, BMessage *message, int32 minValue, int32 maxValue,
                 thumb_style thumbType = B_BLOCK_THUMB,
                 uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
                 uint32 flags = B_NAVIGABLE | B_WILL_DRAW | B_FRAME_EVENTS);

    void MouseDown(BPoint point);
    void MouseMoved(BPoint point, uint32 transit, const BMessage *message);
    void MouseUp(BPoint point);
private:
    bool mouseDown;
};

RtSlider::RtSlider(BRect frame, const char *name, const char *label,
                   BMessage *message, int32 minValue, int32 maxValue,
                   thumb_style thumbType, uint32 resizingMode,
                   uint32 flags)
    :
    BSlider(frame, name, label, message, minValue, maxValue,
            thumbType, resizingMode, flags),
            mouseDown(false)
            {
            }

void
RtSlider::MouseDown(BPoint point)
{
    mouseDown = true;
    BSlider::MouseDown(point);
    Invoke();
}

void
RtSlider::MouseMoved(BPoint point, uint32 transit, const BMessage *message)
{
    if (mouseDown) {
        Invoke();
    }
    BSlider::MouseMoved(point, transit, message);
}

void
RtSlider::MouseUp(BPoint point)
{
    mouseDown = false;
    BSlider::MouseUp(point);
    Invoke();
}

//Main window class
class GaugeWindow : public BWindow
{
    public:
        GaugeWindow();
        ~GaugeWindow();
        void 	MessageReceived(BMessage* message);

    private:
        rgb_color color;
        CircleGaugeControl* gauge;
        CircleGaugeControl* gauge2;
        RtSlider* slider;
};

// Main application class
class SpeedometerApp : public BApplication {
public:
    SpeedometerApp()
    :
    BApplication("application/x-vnd.hfsfox-gaugetestapp")
    {
    }

    void ReadyToRun() {
        // Create a window
        BWindow* w = new GaugeWindow();
    }
};

GaugeWindow::~GaugeWindow()
{
}

GaugeWindow::GaugeWindow()
    :
    BWindow(BRect(50,50,270,270),"Gauge test window",B_TITLED_WINDOW,
          B_QUIT_ON_WINDOW_CLOSE | B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
    slider = new RtSlider(Bounds(), NULL, "Gauge values", new BMessage(MSG_CHANGE_VALUE), 0, 1000, B_TRIANGLE_THUMB);
    slider->SetHashMarkCount(10);
    slider->SetHashMarks(B_HASH_MARKS_BOTTOM);
    slider->SetLimitLabels(B_TRANSLATE("0%"), B_TRANSLATE("100 %"));
    slider->SetValue(0);

    gauge = new CircleGaugeControl(Bounds(), "meter1",
                                                       new BMessage(), 0.0, 100.0, 84.4, "meter 1", "A");
    gauge2 = new CircleGaugeControl(Bounds(), "meter2",
                                                       new BMessage(), 0.0, 100.0, 57.6, "meter 2", "V");

    rgb_color fArcColor = (rgb_color){0, 120, 215, 255};

    // Customize the gauge
    gauge->SetAngles(220.0, 260.0);   // Start at 220°, sweep 260° (wide gauge)
    gauge->SetTicks(10, 5);           // 10 major ticks, 5 minor ticks between each
    gauge->SetStyle(TRIANGLE_NEEDLE);
    gauge->SetValueArcColor(fArcColor);

    gauge2->SetAngles(220.0, 260.0);  // Start at 220°, sweep 260° (wide gauge)
    gauge2->SetTicks(10, 9);          // 10 major ticks, 5 minor ticks between each


    // Add the gauge to the window
    BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
        .AddGroup(B_HORIZONTAL,0)
            .Add(gauge)
            .Add(gauge2)
        .End()
            .Add(new BSeparatorView(B_HORIZONTAL))
            .Add(slider)
    .End();

    this->Show();
}

void
GaugeWindow::MessageReceived(BMessage* msg)
{
    switch ( msg->what )
    {
        case MSG_CHANGE_VALUE:
            {
                gauge->SetValue(slider->Value()/10.0f);
                gauge2->SetValue(slider->Value()/10.0f);
            }
            break;
        default:
            BWindow::MessageReceived(msg);
    }
}

int main() {
    SpeedometerApp app;
    app.Run();
    return 0;
}
