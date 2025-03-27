#ifndef S4LED_ANALYZER_RESULTS
#define S4LED_ANALYZER_RESULTS

#include <AnalyzerResults.h>

#include "S4LedHelpers.h" // for RGBValue

class S4LedAnalyzer;
class S4LedAnalyzerSettings;

class S4LedAnalyzerResults : public AnalyzerResults
{
  public:
    S4LedAnalyzerResults( S4LedAnalyzer* analyzer, S4LedAnalyzerSettings* settings );
    virtual ~S4LedAnalyzerResults();

    void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base ) override;
    void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id ) override;

    void GenerateFrameTabularText( U64 frame_index, DisplayBase display_base ) override;
    void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base ) override;
    void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base ) override;

  protected: // functions
  protected: // vars
    S4LedAnalyzerSettings* mSettings = nullptr;
    S4LedAnalyzer* mAnalyzer = nullptr;

  private:
    void GenerateRGBStrings( const RGBValue& rgb, DisplayBase base, size_t bufSize, char* redBuf, char* greenBuff, char* blueBuf );
};

#endif // S4LED_ANALYZER_RESULTS
