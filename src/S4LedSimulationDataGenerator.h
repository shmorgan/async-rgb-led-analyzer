#ifndef S4LED_SIMULATION_DATA_GENERATOR
#define S4LED_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <AnalyzerHelpers.h>
#include "S4LedHelpers.h"
#include <string>

class S4LedAnalyzerSettings;

class S4LedSimulationDataGenerator
{
  public:
    S4LedSimulationDataGenerator();
    ~S4LedSimulationDataGenerator();

    void Initialize( U32 simulation_sample_rate, S4LedAnalyzerSettings* settings );
    U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

  protected:
    S4LedAnalyzerSettings* mSettings;
    U32 mSimulationSampleRateHz;

  protected:
    void CreateRGBWord();
    RGBValue RandomRGBValue() const;

    void WriteRGBTriple( const RGBValue& rgb );
    void WriteUIntData( U16 data, U8 bit_count );
    void WriteBit( bool b );

    void WriteReset();

    ClockGenerator mClockGenerator;
    SimulationChannelDescriptor mLEDSimulationData;

    // largest value for a color channel in the selected controller.
    // this is 2^bitSize - 1
    U32 mMaximumChannelValue = 255;

    U32 mFrameCount = 0;

    /// do we generate high-speed data for some frames of this controller?
    /// This depends on both the controller support and the requested
    /// sample rate, if it's below 18Mhz we won't generate high speed data
    bool mDoGenerateHighSpeedMode = false;
    bool mHighSpeedMode = false;
};
#endif // S4LED_SIMULATION_DATA_GENERATOR
