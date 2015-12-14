  /// \file SdhcalQuadraticEnergyFunction.cc
/*
 *
 * SdhcalQuadraticEnergyFunction.cc source template automatically generated by a class generator
 * Creation date : mar. nov. 17 2015
 *
 * This file is part of ArborContent libraries.
 * 
 * ArborContent is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * ArborContent is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ArborContent.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#include "ArborPlugins/SdhcalQuadraticEnergyFunction.h"

#include "Pandora/AlgorithmHeaders.h"

namespace arbor_content
{

pandora::StatusCode SdhcalQuadraticEnergyFunction::MakeEnergyCorrections(const pandora::Cluster *const pCluster, float &correctedEnergy) const
{
	unsigned int NHadronicHit  = 0;
	unsigned int NHadronicHit1 = 0;
	unsigned int NHadronicHit2 = 0;
	unsigned int NHadronicHit3 = 0;

	pandora::CaloHitList clusterCaloHitList;
	pCluster->GetOrderedCaloHitList().GetCaloHitList(clusterCaloHitList);

	float emEnergy = 0.f;

	for(pandora::CaloHitList::const_iterator iter = clusterCaloHitList.begin(), endIter = clusterCaloHitList.end() ;
			endIter != iter ; ++iter)
	{
		const pandora::CaloHit *const pCaloHit = *iter;

		if(pandora::ECAL != pCaloHit->GetHitType())
		{
			emEnergy += pCaloHit->GetElectromagneticEnergy();
			continue;
		}

		if(m_sdhcalThresholds.at(0) - pCaloHit->GetInputEnergy() < std::numeric_limits<float>::epsilon())
			NHadronicHit1++;
		else if(m_sdhcalThresholds.at(1) - pCaloHit->GetInputEnergy() < std::numeric_limits<float>::epsilon())
			NHadronicHit2++;
		else if(m_sdhcalThresholds.at(2) - pCaloHit->GetInputEnergy() < std::numeric_limits<float>::epsilon())
			NHadronicHit3++;
		else
			return pandora::STATUS_CODE_SUCCESS;

		NHadronicHit++;
	}

	const float alpha = m_energyConstantParameters.at(0) + m_energyConstantParameters.at(1)*NHadronicHit + m_energyConstantParameters.at(2)*NHadronicHit*NHadronicHit;
	const float beta  = m_energyConstantParameters.at(3) + m_energyConstantParameters.at(4)*NHadronicHit + m_energyConstantParameters.at(5)*NHadronicHit*NHadronicHit;
	const float gamma = m_energyConstantParameters.at(6) + m_energyConstantParameters.at(7)*NHadronicHit + m_energyConstantParameters.at(8)*NHadronicHit*NHadronicHit;

	correctedEnergy = emEnergy + NHadronicHit1*alpha + NHadronicHit2*beta + NHadronicHit3*gamma;

	return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode SdhcalQuadraticEnergyFunction::ReadSettings(const pandora::TiXmlHandle xmlHandle)
{
	pandora::StatusCode statusCode = pandora::XmlHelper::ReadVectorOfValues(xmlHandle,
			"EnergyConstantParameters", m_energyConstantParameters);

	if(statusCode != pandora::STATUS_CODE_SUCCESS)
	{
		if(statusCode == pandora::STATUS_CODE_NOT_FOUND)
		{
			m_energyConstantParameters.push_back(0.0385315);
			m_energyConstantParameters.push_back(4.22584e-05);
			m_energyConstantParameters.push_back(-7.54657e-09);
			m_energyConstantParameters.push_back(0.0784297);
		    m_energyConstantParameters.push_back(-5.69439e-05);
		    m_energyConstantParameters.push_back(-4.95924e-08);
		    m_energyConstantParameters.push_back(0.127212);
		    m_energyConstantParameters.push_back(4.56414e-05);
		    m_energyConstantParameters.push_back(1.41142e-08);
		}
		else
			return statusCode;
	}

	if(9 != m_energyConstantParameters.size())
		return pandora::STATUS_CODE_INVALID_PARAMETER;

	statusCode = pandora::XmlHelper::ReadVectorOfValues(xmlHandle,
			"SdhcalThresholds", m_sdhcalThresholds);

	if(statusCode != pandora::STATUS_CODE_SUCCESS)
	{
		if(statusCode == pandora::STATUS_CODE_NOT_FOUND)
		{
			m_sdhcalThresholds.push_back(1);
			m_sdhcalThresholds.push_back(2);
			m_sdhcalThresholds.push_back(3);
		}
		else
			return statusCode;
	}

	if(3 != m_sdhcalThresholds.size())
		return pandora::STATUS_CODE_INVALID_PARAMETER;

	for(int i=0 ; i<m_sdhcalThresholds.size() ; i++)
		std::cout << "m_sdhcalThresholds.at(i) : " << m_sdhcalThresholds.at(i) << std::endl;

	return pandora::STATUS_CODE_SUCCESS;
}

} 

