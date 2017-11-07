#ifndef CTPPS_CTPPSRawToDigi_CTPPSPixelDataFormatter_h
#define CTPPS_CTPPSRawToDigi_CTPPSPixelDataFormatter_h
/** \class CTPPSPixelDataFormatter
 *
 *  Transform CTPPSPixel raw data of a given  FED to digi
 *  
 *
 * FED OUTPUT DATA FORMAT (F.Ferro from SiPixel code)
 * ----------------------
 * The output is transmitted through a 64 bit S-link connection.
 * The packet format is defined by the CMS RU group to be :
 * 1st packet header, 64 bits, includes a 6 bit FED id.
 * 2nd packet header, 64 bits.
 * .......................... (detector data)
 * packet trailer, 64 bits.
 * of the 64 bit pixel data records consists of 2
 * 32 bit words. Each 32 bit word includes data from 1 pixel,
 * the bit fields are the following:
 *
 * 6 bit link ID (max 36)   - this defines the input link within 1 FED.
 * 5 bit ROC ID (max 24)    - this defines the readout chip within one link.
 * 5 bit DCOL ID (max 26)   - this defines the double column index with 1 chip.
 * 8 bit pixel ID (max 180) - this defines the pixel address within 1 DCOL.
 * 8 bit ADC vales          - this has the charge amplitude.
 *
 * So, 1 pixel occupies 4 bytes.
 * If the number of pixels is odd, one extra 32 bit word is added (value 0)
 * to fill all 64 bits.
 *
 * The CTPPSPixelDataFormatter interpret/format ONLY detector data words
 * (not FED headers or trailer, which are treated elsewhere).
 */
//

#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigi.h" 
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelDAQMapping.h" 
#include "DataFormats/Common/interface/DetSetVector.h"

#include "EventFilter/CTPPSRawToDigi/interface/RPixErrorChecker.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelIndices.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSElectronicIndex.h"
#include "FWCore/Utilities/interface/typedefs.h"

#include <cstdint>
#include <vector>
#include <map>

class FEDRawData;
//class CTPPSPixelDAQMapping;

class CTPPSPixelDataFormatter {

public:

  typedef edm::DetSetVector<CTPPSPixelDigi> Collection;

  typedef std::map<int, FEDRawData> RawData;
  typedef std::vector<CTPPSPixelDigi> DetDigis;
  typedef std::map<cms_uint32_t,DetDigis> Digis;

  typedef uint32_t Word32;
  typedef uint64_t Word64;

  CTPPSPixelDataFormatter(std::map<CTPPSPixelFramePosition, CTPPSPixelROCInfo> const &mapping);

  int nWords() const { return theWordCounter; }
  int nDigis() const { return theDigiCounter; }

  void interpretRawData( bool& errorsInEvent, int fedId,  const FEDRawData & data, Collection & digis);

void formatRawData( unsigned int lvl1_ID, RawData & fedRawData, const Digis & digis, std::map< std::map<const uint32_t,short unsigned int>, std::map<short unsigned int,short unsigned int>> iDdet2fed);

private:

  mutable int theWordCounter;
  mutable int theDigiCounter;

  RPixErrorChecker errorcheck;

  int m_ADC_shift, m_PXID_shift, m_DCOL_shift, m_ROC_shift, m_LINK_shift;
  Word32 m_LINK_mask, m_ROC_mask, m_DCOL_mask, m_PXID_mask, m_ADC_mask;
  int maxROCIndex;
  

  int allDetDigis;
  int hasDetDigis;
  int checkError(const Word32& data) const;

  std::string print(const Word64    & word) const;

  const std::map<CTPPSPixelFramePosition, CTPPSPixelROCInfo> &mapping_;

  CTPPSPixelIndices theIndices;

};

#endif