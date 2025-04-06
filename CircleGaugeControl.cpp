/*
 * Copyright (C) 2025 Kravchuk Roman <darkkitsunezx128k2309@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "CircleGaugeControl.h"
#include <GradientRadial.h>
#include <stdio.h>

CircleGaugeControl::CircleGaugeControl(BRect frame, const char* name,
                                    BMessage* message, float minValue,
                                    float maxValue, float currentValue,
                                    const char* label,const char* unit)
        :
        BControl(frame, name,label, message,
        B_FOLLOW_NONE, B_WILL_DRAW | B_SUBPIXEL_PRECISE | B_FULL_UPDATE_ON_RESIZE | B_NAVIGABLE),
          fMinValue(minValue),
          fMaxValue(maxValue),
          fCurrentValue(currentValue),
          fLabel(label),
          fUnit(unit)
          {
              _Init();
          }

CircleGaugeControl::CircleGaugeControl(BRect frame, const char* name, BMessage* message,
                            float minValue, float maxValue, float currentValue,
                             const char* label, const char* unit,
                             uint32 resizingMode, uint32 flags)
        :
        BControl(frame, name,label, message, resizingMode, flags),
        fMinValue(minValue),
        fMaxValue(maxValue),
        fCurrentValue(currentValue),
        fLabel(label),
        fUnit(unit)
        {
            _Init();
        }

CircleGaugeControl::~CircleGaugeControl()
{
}

CircleGaugeControl::CircleGaugeControl(BMessage* archive)
        :
        BControl(archive)
        {
            _Init();
        }

inline void
CircleGaugeControl::_Init()
{
    fStyle = LINE_NEEDLE;
    fStartAngle = 210.0;
    fSweepAngle = 120.0;
    fMajorTicks = 10;
    fMinorTicksPerMajor = 4;
    fOuterRadius = 0.85;
    fInnerRadius = 0.75;

    // Set default colors
    fDialColor = ui_color(B_CONTROL_BACKGROUND_COLOR);        // Light gray dial
    fBackgroundColor = ui_color(B_CONTROL_BORDER_COLOR);      // Light gray arc
    fForegroundColor = (rgb_color){0, 120, 215, 255};         // Blue active arc
    fNeedleColor = (rgb_color){220, 0, 0, 255};               // Red needle
    fTickColor = (rgb_color){60, 60, 60, 255};                // Dark gray ticks
    fTextColor = ui_color(B_CONTROL_TEXT_COLOR);              // Near black text
    fValueArcColor = (rgb_color){30, 180, 30, 255};           // Green value arc
}

BArchivable*
CircleGaugeControl::Instantiate(BMessage* archive)
{
        if (validate_instantiation(archive, "CircleGaugeControl"))
            return new CircleGaugeControl(archive);

        return NULL;
}

status_t
CircleGaugeControl::Archive(BMessage* archive, bool deep) const
{
    status_t error = BControl::Archive(archive, deep);
    return error;
}

void
CircleGaugeControl::SetFlags(uint32 flags)
{
    BControl::SetFlags(flags);
}

void
CircleGaugeControl::SetResizingMode(uint32 mode)
{
    BControl::SetResizingMode(mode);
}

void
CircleGaugeControl::SetEnabled(bool on)
{
    BControl::SetEnabled(on);
}

void
CircleGaugeControl::Draw(BRect rect)
{
        // Calculate center and radius of the meter
        BRect bounds = Bounds();
        float centerX = bounds.Width() / 2;
        float centerY = bounds.Height() * 0.6;  // Move center point down for meter view
        BPoint center(centerX, centerY);
        float maxRadius = MIN(centerX, centerY) * fOuterRadius;
        float innerRadius = maxRadius * fInnerRadius;


        // Draw the gauge face (filled sector)
        SetHighColor(ui_color(B_CONTROL_BACKGROUND_COLOR));

        // Draw border arc - use StrokeArc instead of BeginLineArray
        SetHighColor(fBackgroundColor);
        SetPenSize(8.0);
        StrokeArc(center, maxRadius, maxRadius, fStartAngle - fSweepAngle, fSweepAngle);

        // Calculate current angle
        float currentAngle = ValueToAngle(fCurrentValue);

        // Draw the value arc (from min to current) - using StrokeArc
        if (fCurrentValue > fMinValue) {
            SetHighColor(fValueArcColor);
            // Calculate the angle difference between current and start
            float valueSweep = fStartAngle - currentAngle;
            StrokeArc(center, maxRadius, maxRadius, currentAngle, valueSweep);
        }

        // Draw major tick marks and labels
        BFont font;
        font.SetSize(12.0);
        SetFont(&font);
        SetHighColor(fTickColor);

        for (int i = 0; i <= fMajorTicks; i++) {
            float tickPercentage = (float)i / fMajorTicks;
            float tickValue = fMinValue + (tickPercentage * (fMaxValue - fMinValue));
            float tickAngle = fStartAngle - (tickPercentage * fSweepAngle);

            // Calculate tick positions
            BPoint outerPoint = GetPointOnArc(tickAngle, maxRadius, center);
            BPoint innerPoint = GetPointOnArc(tickAngle, maxRadius - 15, center);

            // Draw major tick line
            SetPenSize(2.0);
            StrokeLine(outerPoint, innerPoint);

            // Draw tick value
            char valueText[32];
            sprintf(valueText, "%.0f", tickValue);

            // Position tick value text
            float textWidth = font.StringWidth(valueText);
            BPoint textPoint = GetPointOnArc(tickAngle, maxRadius - 30, center);

            // Adjust text position for better alignment
            textPoint.x -= textWidth / 2;
            textPoint.y += 5;  // Account for text height

            SetHighColor(fTextColor);
            DrawString(valueText, textPoint);
            SetHighColor(fTickColor);

            // Draw minor ticks if not the last major tick
            if (i < fMajorTicks) {
                SetPenSize(1.0);
                for (int j = 1; j <= fMinorTicksPerMajor; j++) {
                    float minorPercentage = tickPercentage + (j * (1.0 / fMajorTicks) / (fMinorTicksPerMajor + 1));
                    float minorAngle = fStartAngle - (minorPercentage * fSweepAngle);

                    BPoint minorOuterPoint = GetPointOnArc(minorAngle, maxRadius, center);
                    BPoint minorInnerPoint = GetPointOnArc(minorAngle, maxRadius - 8, center);

                    StrokeLine(minorOuterPoint, minorInnerPoint);
                }
            }
        }

        // Draw needle
        SetHighColor(fNeedleColor);
        float needleLength = maxRadius - 20;
        float needleWidth = needleLength * 0.1;

        if(fStyle == LINE_NEEDLE)
        {
        BPoint needlePoint = GetPointOnArc(currentAngle, needleLength, center);
        // Create needle line path
        StrokeLine(center,needlePoint,B_SOLID_HIGH);
        }

        if(fStyle == TRIANGLE_NEEDLE)
        {
            BPoint needlePoint = GetPointOnArc(currentAngle, needleLength, center);
            BPoint needleBase1 = GetPointOnArc(currentAngle + 90, needleWidth, center);
            BPoint needleBase2 = GetPointOnArc(currentAngle - 90, needleWidth, center);

            // Create needle triangle path
            BPoint needlePoints[3] = { needlePoint, needleBase1, needleBase2 };
            FillPolygon(needlePoints, 3);
        }
        //

        // Draw needle center cap
        BGradientRadial gr(center, maxRadius/8);
        gr.AddColor(ui_color(B_CONTROL_BACKGROUND_COLOR), 0);
        gr.AddColor(ui_color(B_PANEL_BACKGROUND_COLOR), 255);
        FillEllipse(center, maxRadius/8, maxRadius/8, gr);
        SetHighColor(ui_color(B_CONTROL_BORDER_COLOR));
        StrokeEllipse(center,maxRadius/8,maxRadius/8,B_SOLID_HIGH);

        // Draw current value with unit below the meter
        font.SetSize(be_bold_font->Size()+8);
        SetFont(&font);

        char valueText[64];
        if (fUnit.Length() > 0) {
            sprintf(valueText, "%.1f %s", fCurrentValue, fUnit.String());
        } else {
            sprintf(valueText, "%.1f", fCurrentValue);
        }
        BString displayText = valueText;

        SetHighColor(fTextColor);
        font_height fontHeight;
        font.GetHeight(&fontHeight);

        float textWidth = font.StringWidth(displayText.String());
        float textHeight = fontHeight.ascent + fontHeight.descent;

        DrawString(displayText.String(),
                   BPoint(centerX - textWidth / 2, centerY + maxRadius * 0.6));

        // Draw label below the value
        font.SetSize(be_bold_font->Size());
        SetFont(&font);
        textWidth = font.StringWidth(fLabel.String());
        DrawString(fLabel.String(),
                   BPoint(centerX - textWidth / 2, centerY + maxRadius * 0.6 + textHeight));
}

BSize
CircleGaugeControl::MinSize()
{
    return BSize(128.f, 128.f);
}

void
CircleGaugeControl::MouseUp(BPoint point)
{
	BControl::MouseUp(point);
}

void
CircleGaugeControl::MouseDown(BPoint point)
{
    uint32 buttons;
    GetMouse(&point, &buttons, true);
	BControl::MouseDown(point);
}

void
CircleGaugeControl::KeyDown(const char *bytes, int32 numBytes)
{
    if ( numBytes >= 1 ) {
        switch ( bytes[0] ) {
            default:
                BControl::KeyDown(bytes, numBytes);
                break;
        }
    }
}

void
CircleGaugeControl::AttachedToWindow()
{
    ResizeToPreferred();
    BControl::AttachedToWindow();
}

void
CircleGaugeControl::WindowActivated(bool state)
{
    BControl::WindowActivated(state);
}

void
CircleGaugeControl::FrameResized(float width,float height)
{
    ResizeToPreferred();
    BControl::FrameResized(width, height);
    Draw(Bounds());
    Invalidate(Bounds());
}

void
CircleGaugeControl::MessageReceived(BMessage* message)
{
    switch ( message->what )
    {
        default:
            BControl::MessageReceived(message);
    }
}

void
CircleGaugeControl::SetValue(float value)
{
    if (value < fMinValue){
        fCurrentValue = fMinValue;
        value = fMinValue;
    }
    else if (value > fMaxValue){
        fCurrentValue = fMaxValue;
        value = fMinValue;
    }
    else
        fCurrentValue = value;

    Invalidate();
}

void
CircleGaugeControl::SetStyle(needle_style style)
{
    if(fStyle == NULL)
    {
        fStyle = LINE_NEEDLE;
    }
    fStyle = style;
}

void
CircleGaugeControl::SetUnit(const char* unit)
{
    fUnit = unit;
    Invalidate();
}

void
CircleGaugeControl::SetValueArcColor(rgb_color color)
{
    fValueArcColor = color;
    Invalidate();
}

void
CircleGaugeControl::SetNeedleColor(rgb_color color)
{
    fNeedleColor = color;
    Invalidate();
}

void
CircleGaugeControl::SetTickColor(rgb_color color)
{
    fTickColor = color;
    Invalidate();
}

void
CircleGaugeControl::SetTextColor(rgb_color color)
{
    fTextColor = color;
    Invalidate();
}


void
CircleGaugeControl::SetDialColor(rgb_color color)
{
    fDialColor = color;
    Invalidate();
}

void
CircleGaugeControl::SetBackgroundColor(rgb_color color)
{
    fBackgroundColor = color;
    Invalidate();
}

void
CircleGaugeControl::SetForegroundColor(rgb_color color)
{
    fForegroundColor = color;
    Invalidate();
}

void
CircleGaugeControl::SetAngles(float startAngle, float sweepAngle)
{
    fStartAngle = startAngle;
    fSweepAngle = sweepAngle;
    Invalidate();
}

void
CircleGaugeControl::SetTicks(int majorTicks, int minorTicksPerMajor)
{
    fMajorTicks = majorTicks;
    fMinorTicksPerMajor = minorTicksPerMajor;
    Invalidate();
}

void
CircleGaugeControl::SetRadius(float outerRadius, float innerRadius)
{
    fOuterRadius = outerRadius;
    fInnerRadius = innerRadius;
    Invalidate();
}


inline float
CircleGaugeControl::DegreeToRadian(float degrees) const
{
    return degrees * M_PI / 180.0;
}

inline BPoint
CircleGaugeControl::GetPointOnArc(float angle, float radius, BPoint center) const
{
    float radians = DegreeToRadian(angle);
    return BPoint(center.x + radius * cos(radians),
                  center.y - radius * sin(radians));
}

// Calculate angle for a given value
inline float
CircleGaugeControl::ValueToAngle(float value) const
{
    float percentage = (value - fMinValue) / (fMaxValue - fMinValue);
    return fStartAngle - (percentage * fSweepAngle);
}
