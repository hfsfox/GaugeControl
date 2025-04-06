/*
 * Copyright (C) 2025 Kravchuk Roman <darkkitsunezx128k2309@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <Control.h>
#include <Message.h>
#include <Point.h>
#include <Rect.h>
#include <View.h>

enum needle_style
{
    LINE_NEEDLE = 0,
    TRIANGLE_NEEDLE = 1
};

class CircleGaugeControl: public BControl
{
	public:
		CircleGaugeControl(BRect frame, const char* name, BMessage* message,
                           float minValue,float maxValue, float currentValue,
                            const char* label, const char* unit);

        CircleGaugeControl(BRect frame, const char* name, BMessage* message,
                           float minValue,float maxValue, float currentValue,
						   const char* label, const char* unit,
						   uint32 resizingMode, uint32 flags);

        CircleGaugeControl(BMessage* archive);
		~CircleGaugeControl();

    public:
		void    Draw( BRect rect );
		void	FrameResized(float width, float height);
    	void	Update();
    	void 	MouseDown(BPoint point);
		void 	MouseUp(BPoint point);
		void	KeyDown(const char *bytes, int32 numBytes);
        void    SetFlags(uint32 flags);
        void    SetResizingMode(uint32 mode);
        void    SetEnabled(bool on);
		void    AttachedToWindow();
        void    WindowActivated(bool state);
    	void 	MessageReceived(BMessage* message);
    	BSize   MinSize();
		static	BArchivable*		Instantiate(BMessage* data);
		virtual	status_t			Archive(BMessage* data,
											bool deep = true) const;
    public:
        void SetAngles(float startAngle, float sweepAngle);
        void SetTicks(int majorTicks, int minorTicksPerMajor);
        void SetRadius(float outerRadius, float innerRadius);
        void SetDialColor(rgb_color color);
        void SetBackgroundColor(rgb_color color);
        void SetForegroundColor(rgb_color color);
        void SetNeedleColor(rgb_color color);
        void SetTickColor(rgb_color color);
        void SetTextColor(rgb_color color);
        void SetValueArcColor(rgb_color color);
    public:
        void SetValue(float value);
        void SetUnit(const char* unit);
    public:
        void SetStyle(needle_style style);
    private:
        void _Init();

    private:
        float DegreeToRadian(float degrees) const;
        float ValueToAngle(float value) const;
        BPoint GetPointOnArc(float angle, float radius, BPoint center) const;


    private:
    float fMinValue;
    float fMaxValue;
    float fCurrentValue;
    BString fLabel;
    BString fUnit;

    needle_style fStyle;

    // Colors
    rgb_color fDialColor;
    rgb_color fBackgroundColor;
    rgb_color fForegroundColor;
    rgb_color fNeedleColor;
    rgb_color fTickColor;
    rgb_color fTextColor;
    rgb_color fValueArcColor;

    // Gauge properties
    float fStartAngle;     // Starting angle in degrees
    float fSweepAngle;     // Sweep angle in degrees
    int fMajorTicks;       // Number of major tick marks
    int fMinorTicksPerMajor; // Number of minor ticks between major ticks
    float fOuterRadius;    // Outer radius relative to view size
    float fInnerRadius;    // Inner radius relative to outer radius
};
