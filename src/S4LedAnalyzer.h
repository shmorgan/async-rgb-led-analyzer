#ifndef S4LED_ANALYZER_H
#define S4LED_ANALYZER_H

#include <Analyzer.h>

#include "S4LedSimulationDataGenerator.h"
#include "S4LedHelpers.h"

// forward decls
class S4LedAnalyzerSettings;
class S4LedAnalyzerResults;

class S4LedAnalyzer : public Analyzer2
{
  public:
    S4LedAnalyzer();
    virtual ~S4LedAnalyzer();

    void SetupResults() override;
    void WorkerThread() override;

    U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels ) override;
    U32 GetMinimumSampleRateHz() override;

    const char* GetAnalyzerName() const override;
    bool NeedsRerun() override;

  protected: // vars
    std::unique_ptr<S4LedAnalyzerSettings> mSettings;
    std::unique_ptr<S4LedAnalyzerResults> mResults;
    AnalyzerChannelData* mChannelData = nullptr;

    S4LedSimulationDataGenerator mSimulationDataGenerator;
    bool mSimulationInitialized = false;

    // analysis vars:
    double mSampleRateHz = 0;

    // minimum valid low time for a data bit, in either speed mode supported
    // by the controller.
    double mMinimumLowDurationSec = 0.0;

    bool mFirstBitAfterReset = false;
    bool mDidDetectHighSpeed = false;

  private:
    struct AddressResult
    {
        bool mValid = false;
        bool mIsReset = false;
        U8 mAddress;
        U64 mValueBeginSample = 0;
        U64 mValueEndSample = 0;
    };

    struct RGBResult
    {
        bool mValid = false;
        bool mIsReset = false;
        RGBValue mRGB;
        U64 mValueBeginSample = 0;
        U64 mValueEndSample = 0;
    };

    RGBResult ReadRGBTriple();
    AddressResult ReadPixelAddress();

    struct ReadResult
    {
        bool mValid = false;
        bool mIsReset = false;
        BitState mBitValue = BIT_LOW;
        U64 mBeginSample = 0;
        U64 mEndSample = 0;
    };

    ReadResult ReadBit();
    void SynchronizeToReset();

    bool DetectSpeedMode( double positiveTimeSec, double negativeTimeSec, BitState& value );
};

extern "C"
{
    ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
    ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer();
    ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );
}

#endif // S4LED_ANALYZER_H
