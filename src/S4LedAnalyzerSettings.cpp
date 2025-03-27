#include "S4LedAnalyzerSettings.h"

#include <cassert>

#include <AnalyzerHelpers.h>

const char* DEFAULT_CHANNEL_NAME = "S4 Addressable LEDs (Async)";

double operator"" _ns( unsigned long long x )
{
    return x * 1e-9;
}

double operator"" _us( unsigned long long x )
{
    return x * 1e-6;
}

S4LedAnalyzerSettings::S4LedAnalyzerSettings()
{
    InitControllerData();

    mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
    mInputChannelInterface->SetTitleAndTooltip( "LED Channel", "Standard S4 Addressable LEDs (Async)" );
    mInputChannelInterface->SetChannel( mInputChannel );

    mControllerInterface.reset( new AnalyzerSettingInterfaceNumberList() );
    mControllerInterface->SetTitleAndTooltip( "LED Controller", "Specify the LED controller in use." );

    int index = 0;

    for( const auto& controllerData : mControllers )
    {
        mControllerInterface->AddNumber( index++, controllerData.mName.c_str(), controllerData.mDescription.c_str() );
    }

    mControllerInterface->SetNumber( mLEDController );

    AddInterface( mInputChannelInterface.get() );
    AddInterface( mControllerInterface.get() );

    AddExportOption( 0, "Export as text/csv file" );
    AddExportExtension( 0, "text", "txt" );
    AddExportExtension( 0, "csv", "csv" );

    ClearChannels();
    AddChannel( mInputChannel, DEFAULT_CHANNEL_NAME, false );
}

S4LedAnalyzerSettings::~S4LedAnalyzerSettings()
{
}

void S4LedAnalyzerSettings::InitControllerData()
{
    // order of values here must correspond to the Controller enum
    mControllers = {
        // name, 
		// description, 
		// bits per channel, 
		// channels per frame, 
		// reset time nsec, 
		// low-speed data nsec, 
		// has high speed, 
		// high speed data
        // nsec, 
		// color layout

        { "P9864B",
          "S4 24-bit RGB controller",
          8,
          3,
          { 50_us, 50_us, 50_us },
          {
              // low-speed times
			  // T1H   - 1us - 8.6us - 10us
			  // T1L   - 5.1us - 6.4us - 7.6us
			  // T0H   - 1us - 11.816us - 15us
			  // T0L   - 2.5us - 3.2us - 3.8us
			  // TendH -    1us
			  // TendL - 10.2us - 12.8us - 15.3us
              { { 2_us, 3_us, 4_ns }, { 1_us, 12_us, 15_us } }, // 0-bit times T0L - T0H
              { { 5_us, 6_us, 8_us }, { 1_us,  9_us, 11_us } }, // 1-bit times T1L - T1H
          },
          false,
          { {}, {} },
          LAYOUT_GRB },

    };
}

bool S4LedAnalyzerSettings::SetSettingsFromInterfaces()
{
    mInputChannel = mInputChannelInterface->GetChannel();
    // explicit cast to keep MSVC happy
    const int index = static_cast<int>( mControllerInterface->GetNumber() );
    mLEDController = static_cast<Controller>( index );

    ClearChannels();
    AddChannel( mInputChannel, DEFAULT_CHANNEL_NAME, true );

    return true;
}

void S4LedAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mInputChannelInterface->SetChannel( mInputChannel );
    mControllerInterface->SetNumber( mLEDController );
}

void S4LedAnalyzerSettings::LoadSettings( const char* settings )
{
    SimpleArchive text_archive;
    text_archive.SetString( settings );

    U32 controllerInt;
    text_archive >> mInputChannel;
    text_archive >> controllerInt;
    mLEDController = static_cast<Controller>( controllerInt );

    ClearChannels();
    AddChannel( mInputChannel, DEFAULT_CHANNEL_NAME, true );

    UpdateInterfacesFromSettings();
}

const char* S4LedAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << mInputChannel;
    text_archive << mLEDController;

    return SetReturnString( text_archive.GetString() );
}

U8 S4LedAnalyzerSettings::BitSize() const
{
    return mControllers.at( mLEDController ).mBitsPerChannel;
}

U8 S4LedAnalyzerSettings::LEDChannelCount() const
{
    return mControllers.at( mLEDController ).mChannelCount;
}

bool S4LedAnalyzerSettings::IsHighSpeedSupported() const
{
    return mControllers.at( mLEDController ).mHasHighSpeed;
}

BitTiming S4LedAnalyzerSettings::DataTiming( BitState value, bool isHighSpeed ) const
{
    const auto& c = mControllers.at( mLEDController );
    assert( !isHighSpeed || c.mHasHighSpeed );

    return isHighSpeed ? c.mDataTimingHighSpeed[ value ] : c.mDataTiming[ value ];
}

TimingTolerance S4LedAnalyzerSettings::ResetTiming() const
{
    return mControllers.at( mLEDController ).mResetTiming;
}

ColorLayout S4LedAnalyzerSettings::GetColorLayout() const
{
    return mControllers.at( mLEDController ).mLayout;
}
