//-------------------------------------------------
//
/**  \class L1MuBMPtaLut
 *
 *   Look-up tables for pt-assignment
 *
 *
 *   $Date: 2007/03/30 07:48:02 $
 *   $Revision: 1.1 $
 *
 *   N. Neumeister            CERN EP
 */
//
//--------------------------------------------------
#ifndef L1MUBM_PTA_LUT_H
#define L1MUBM_PTA_LUT_H

//---------------
// C++ Headers --
//---------------

#include "CondFormats/Serialization/interface/Serializable.h"

#include <vector>
#include <map>

//----------------------
// Base Class Headers --
//----------------------


//------------------------------------
// Collaborating Class Declarations --
//------------------------------------


//              ---------------------
//              -- Class Interface --
//              ---------------------


class L1MuBMPtaLut {

  public:

    /// constructor
    L1MuBMPtaLut();

    /// destructor
    virtual ~L1MuBMPtaLut();

    /// reset pt-assignment look-up tables
    void reset();

    /// load pt-assignment look-up tables
    int load();

    /// print pt-assignment look-up tables
    void print() const;

    /// get pt-value for a given address
    int getPt(int pta_ind, int address) const;

    /// get pt-assignment LUT threshold
    int getPtLutThreshold(int pta_ind) const;

  private:

    /// set precision for look-up tables
    void setPrecision();

  private:

    typedef std::map<short, short, std::less<short> > LUT;

    std::vector<LUT> pta_lut;
    std::vector<int> pta_threshold;

    unsigned short int nbit_phi;
    

  COND_SERIALIZABLE;
};

#endif
