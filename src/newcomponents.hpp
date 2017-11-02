#pragma once
#include "newapp.hpp"
//#include "asset.hpp"


namespace rack {


////////////////////
// Colors
////////////////////

#define COLOR_BLACK_TRANSPARENT nvgRGBA(0x00, 0x00, 0x00, 0x00)
#define COLOR_BLACK nvgRGB(0x00, 0x00, 0x00)
#define COLOR_WHITE nvgRGB(0xff, 0xff, 0xff)
#define COLOR_RED nvgRGB(0xed, 0x2c, 0x24)
#define COLOR_ORANGE nvgRGB(0xf2, 0xb1, 0x20)
#define COLOR_YELLOW nvgRGB(0xf9, 0xdf, 0x1c)
#define COLOR_GREEN nvgRGB(0x90, 0xc7, 0x3e)
#define COLOR_CYAN nvgRGB(0x22, 0xe6, 0xef)
#define COLOR_BLUE nvgRGB(0x29, 0xb2, 0xef)
#define COLOR_PURPLE nvgRGB(0xd5, 0x2b, 0xed)

////////////////////
// Knobs
////////////////////

struct RoundKnob : SVGKnob { };

struct RoundBlackKnob : RoundKnob { };

struct RoundSmallBlackKnob : RoundBlackKnob { };

struct RoundLargeBlackKnob : RoundBlackKnob { };

struct RoundHugeBlackKnob : RoundBlackKnob { };

struct RoundSmallBlackSnapKnob : RoundSmallBlackKnob { };


struct Davies1900hKnob : SVGKnob { };

struct Davies1900hWhiteKnob : Davies1900hKnob { };

struct Davies1900hBlackKnob : Davies1900hKnob { };

struct Davies1900hRedKnob : Davies1900hKnob { };

struct Davies1900hLargeWhiteKnob : Davies1900hKnob { };

struct Davies1900hLargeBlackKnob : Davies1900hKnob { };

struct Davies1900hLargeRedKnob : Davies1900hKnob { };


struct Rogan : SVGKnob { };

struct Rogan6PSWhite : Rogan { };

struct Rogan5PSGray : Rogan { };

struct Rogan3PSBlue : Rogan { };

struct Rogan3PSRed : Rogan { };

struct Rogan3PSGreen : Rogan { };

struct Rogan3PSWhite : Rogan { };

struct Rogan3PBlue : Rogan { };

struct Rogan3PRed : Rogan { };

struct Rogan3PGreen : Rogan { };

struct Rogan3PWhite : Rogan { };

struct Rogan2SGray : Rogan { };

struct Rogan2PSBlue : Rogan { };

struct Rogan2PSRed : Rogan { };

struct Rogan2PSGreen : Rogan { };

struct Rogan2PSWhite : Rogan { };

struct Rogan2PBlue : Rogan { };

struct Rogan2PRed : Rogan { };

struct Rogan2PGreen : Rogan { };

struct Rogan2PWhite : Rogan { };

struct Rogan1PSBlue : Rogan { };

struct Rogan1PSRed : Rogan { };

struct Rogan1PSGreen : Rogan { };

struct Rogan1PSWhite : Rogan { };

struct Rogan1PBlue : Rogan { };

struct Rogan1PRed : Rogan { };

struct Rogan1PGreen : Rogan { };

struct Rogan1PWhite : Rogan { };


struct SynthTechAlco : SVGKnob { };

struct Trimpot : SVGKnob { };

struct BefacoBigKnob : SVGKnob { };

struct BefacoBigSnapKnob : BefacoBigKnob { };

struct BefacoTinyKnob : SVGKnob { };

struct BefacoSlidePot : SVGSlider { };

////////////////////
// Jacks
////////////////////

struct PJ301MPort : SVGPort { };

struct PJ3410Port : SVGPort { };

struct CL1362Port : SVGPort { };

////////////////////
// Lights
////////////////////

struct RedLight : ColorLightWidget { };

struct GreenLight : ColorLightWidget { };

struct YellowLight : ColorLightWidget { };

struct BlueLight : ColorLightWidget { };

/** Reads two adjacent lightIds, so `lightId` and `lightId + 1` must be defined */
struct GreenRedLight : ColorLightWidget { };

struct RedGreenBlueLight : ColorLightWidget { };


template <typename BASE>
struct LargeLight : BASE { };

template <typename BASE>
struct MediumLight : BASE { };

template <typename BASE>
struct SmallLight : BASE { };

template <typename BASE>
struct TinyLight : BASE { };

////////////////////
// Switches and Buttons
////////////////////

struct NKK : SVGSwitch, ToggleSwitch { };

struct CKSS : SVGSwitch, ToggleSwitch { };

struct CKD6 : SVGSwitch, MomentarySwitch { };

struct TL1105 : SVGSwitch, MomentarySwitch { };

struct LEDButton : SVGSwitch, MomentarySwitch { };

struct BefacoSwitch : SVGSwitch, ToggleSwitch { };

struct BefacoPush : SVGSwitch, MomentarySwitch { };

struct PB61303 : SVGSwitch, MomentarySwitch { };


////////////////////
// Misc
////////////////////

struct ScrewSilver : SVGScrew { };

struct ScrewBlack : SVGScrew { };

struct LightPanel : Panel { };

struct DarkPanel : Panel { };


} // namespace rack

