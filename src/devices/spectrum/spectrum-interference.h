/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */


#ifndef SPECTRUM_INTERFERENCE_H
#define SPECTRUM_INTERFERENCE_H

#include <ns3/object.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/spectrum-value.h>

namespace ns3 {




class SpectrumErrorModel;



/**
 * This class implements a gaussian interference model, i.e., all
 * incoming signals are added to the total interference.
 *
 */
class SpectrumInterference : public Object
{
public:
  SpectrumInterference ();
  virtual ~SpectrumInterference ();

  /**
   * set the SpectrumErrorModel to be used.
   *
   * @param e
   */
  void SetErrorModel (Ptr<SpectrumErrorModel> e);

  /**
   * notify that the PHY is starting a RX attempt
   *
   * @param p the packet corresponding to the signal being RX
   * @param rxPsd the power spectral density of the signal being RX
   */
  void StartRx (Ptr<const Packet> p, Ptr<const SpectrumValue> rxPsd);


  /**
   * notify that the RX attempt has ended. The receiving PHY must call
   * this method upon RX end in order to:
   * 1) know if RX was successful or not
   * 2) free up resources that might eventually be used for the
   * calculation of interference. Note that for this reason this
   * method must also be called when RX is aborted.
   *
   *
   * @return true if RX was successful, false otherwise
   */
  bool EndRx ();


  /**
   * notify that a new signal is being perceived in the medium. This
   * method is to be called for all incoming signal, regardless of
   * wether they're useful signals or interferers.
   *
   * @param spd the power spectral density of the new signal
   * @param duration the duration of the new signal
   */
  void AddSignal (Ptr<const SpectrumValue>, const Time duration);


  /**
   *
   * @param noisePsd the Noise Power Spectral Density in power units
   * (Watt, Pascal...) per Hz.
   */
  void SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd);


protected:
  
  void DoDispose ();

private:
  void ConditionallyEvaluateChunk ();
  void DoAddSignal  (Ptr<const SpectrumValue> spd);
  void DoSubtractSignal  (Ptr<const SpectrumValue> spd);



  bool m_receiving;

  Ptr<const SpectrumValue> m_rxSignal; /**< stores the power spectral density of
                                  * the signal whose RX is being
                                  * attempted
                                  */

  Ptr<SpectrumValue> m_allSignals; /**< stores the spectral
                                    * power density of the sum of incoming signals;
                                    * does not include noise, includes the SPD of the signal being RX
                                    */

  Ptr<const SpectrumValue> m_noise;

  Time m_lastChangeTime;     /**< the time of the last change in
                                m_TotalPower */

  Ptr<SpectrumErrorModel> m_errorModel;



};



} // namespace ns3





#endif /* SPECTRUM_INTERFERENCE_H */
