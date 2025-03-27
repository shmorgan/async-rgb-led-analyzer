#ifndef S4LED_ANALYZER_SETTINGS
#define S4LED_ANALYZER_SETTINGS

#include <vector>

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

#include "S4LedHelpers.h"

class S4LedAnalyzerSettings : public AnalyzerSettings
{
  public:
    S4LedAnalyzerSettings();
    virtual ~S4LedAnalyzerSettings();

    // delete all the copy/move operators for rule-of-5 compliance
    S4LedAnalyzerSettings( const S4LedAnalyzerSettings& ) = delete;
    S4LedAnalyzerSettings( S4LedAnalyzerSettings&& ) = delete;
    const S4LedAnalyzerSettings& operator=( const S4LedAnalyzerSettings& ) = delete;
    S4LedAnalyzerSettings& operator=( S4LedAnalyzerSettings&& ) = delete;


    bool SetSettingsFromInterfaces() override;
    void UpdateInterfacesFromSettings();
    void LoadSettings( const char* settings ) override;
    const char* SaveSettings() override;

    enum Controller
    {
		LED_P9864B = 0
    };

    Controller mLEDController = LED_P9864B;
    Channel mInputChannel = UNDEFINED_CHANNEL;

    /// bits ber LED channel, either 8 or 12 at present
    U8 BitSize() const;

    /// LED channel count, either 3 (RGB) or 9 (three RGB outputs) at present
    U8 LEDChannelCount() const;

    bool IsHighSpeedSupported() const;

    BitTiming DataTiming( BitState value, bool isHighSpeed = false ) const;

    TimingTolerance ResetTiming() const;

    ColorLayout GetColorLayout() const;

  protected:
    void InitControllerData();

    std::unique_ptr<AnalyzerSettingInterfaceChannel> mInputChannelInterface;
    std::unique_ptr<AnalyzerSettingInterfaceNumberList> mControllerInterface;

    // we can't do direct defualt initialisation here, since according to C++11
    // that makes this type non-POD and hence unsuitable for direct initialisation.
    // C++14 fixes this.
    struct LedControllerData
    {
        std::string mName;
        std::string mDescription;
        U8 mBitsPerChannel; // = 8;
        U8 mChannelCount;   // = 3;
        TimingTolerance mResetTiming;
        BitTiming mDataTiming[ 2 ]; // BIT_HIGH and BIT_LOW

        bool mHasHighSpeed;                  // = true
        BitTiming mDataTimingHighSpeed[ 2 ]; // BIT_HIGH and BIT_LOW

        ColorLayout mLayout; // = LAYOUT_RGB
    };

    std::vector<LedControllerData> mControllers;
};

#endif // S4LED_ANALYZER_SETTINGS
