/*
 *
 * GlobalConnectorCleaningAlgorithm.cc source template automatically generated by a class generator
 * Creation date : mar. avr. 7 2015
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


#include "ArborClustering/GlobalConnectorCleaningAlgorithm.h"

#include "ArborObjects/CaloHit.h"
#include "ArborObjects/Connector.h"

#include "ArborUtility/ReferenceVectorTools.h"
#include "ArborUtility/OrderParameterTools.h"

#include "Pandora/AlgorithmHeaders.h"

namespace arbor_content
{

pandora::StatusCode GlobalConnectorCleaningAlgorithm::Run()
{
	const pandora::CaloHitList *pCaloHitList = NULL;
	PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCaloHitList));

	if(pCaloHitList->empty())
		return pandora::STATUS_CODE_SUCCESS;

	if(0 == m_cleaningStrategy)
	{
		PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, this->Clean(pCaloHitList));
	}
	else
	{
		pandora::OrderedCaloHitList orderedCaloHitList;
		PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, orderedCaloHitList.Add(*pCaloHitList));

		PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, this->Clean(orderedCaloHitList));
	}

	return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode GlobalConnectorCleaningAlgorithm::Clean(const pandora::CaloHitList *const pCaloHitList) const
{
	CleaningHelperList cleaningHelperList;

	for(pandora::CaloHitList::const_iterator iter = pCaloHitList->begin(), endIter = pCaloHitList->end() ;
			endIter != iter ; ++iter)
	{
		const CaloHit *pCaloHit = dynamic_cast<const CaloHit *>(*iter);

		if(NULL == pCaloHit)
			return pandora::STATUS_CODE_FAILURE;

		const ConnectorList &backwardConnectorList(ArborContentApi::GetConnectorList(pCaloHit, BACKWARD_DIRECTION));

		if(backwardConnectorList.size() < 2)
			continue;

		pandora::CartesianVector referenceVector(0, 0, 0);

		PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pReferenceVectorTool->ComputeReferenceVector(pCaloHit, referenceVector));

		const CaloHit *pBestCaloHit = NULL;
		float bestOrderParameter = std::numeric_limits<float>::max();
		pandora::CaloHitList deleteConnectionCaloHitList;

		// find the best connector with the smallest order parameter
		for(ConnectorList::const_iterator connectorIter = backwardConnectorList.begin(), connectorEndIter = backwardConnectorList.end() ;
				connectorEndIter != connectorIter ; ++connectorIter)
		{
			const Connector *pConnector = *connectorIter;
			const CaloHit *pFromCaloHit = pConnector->GetFrom();

			float orderParameter = 0.f;
			PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pOrderParameterTool->ComputeOrderParameter(pCaloHit, pConnector, referenceVector, orderParameter));

			if(orderParameter< bestOrderParameter)
			{
				if(NULL != pBestCaloHit)
					deleteConnectionCaloHitList.insert(pBestCaloHit);

				bestOrderParameter = orderParameter;
				pBestCaloHit = pFromCaloHit;
			}
			else
			{
				deleteConnectionCaloHitList.insert(pFromCaloHit);
			}
		}

		if(NULL != pBestCaloHit)
		{
			CleaningHelper cleaningHelper;
			cleaningHelper.m_pCaloHit = pCaloHit;
			cleaningHelper.m_deleteConnectionCaloHitList = deleteConnectionCaloHitList;
			cleaningHelperList.push_back(cleaningHelper);
		}
	}

	// remove all unneeded connections
	for(CleaningHelperList::const_iterator iter = cleaningHelperList.begin(), endIter = cleaningHelperList.end() ;
			endIter != iter ; ++iter)
	{
		for(pandora::CaloHitList::const_iterator hitIter = iter->m_deleteConnectionCaloHitList.begin(), hitEndIter = iter->m_deleteConnectionCaloHitList.end() ;
				hitEndIter != hitIter ; ++hitIter)
		{
			const CaloHit *const pCaloHit = dynamic_cast<const CaloHit *const>(*hitIter);
			PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, ArborContentApi::RemoveConnectionBetween(iter->m_pCaloHit, pCaloHit));
		}
	}

	cleaningHelperList.clear();

	return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode GlobalConnectorCleaningAlgorithm::Clean(const pandora::OrderedCaloHitList &orderedcaloHitList) const
{
	if(1 == m_cleaningStrategy)
	{
		for(pandora::OrderedCaloHitList::const_iterator iter = orderedcaloHitList.begin(), endIter = orderedcaloHitList.end() ;
				endIter != iter ; ++iter)
		{
			if(iter->second->empty())
				continue;
				
			PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, this->Clean(iter->second));
		}
	}
	else
	{
		for(pandora::OrderedCaloHitList::const_reverse_iterator iter = orderedcaloHitList.rbegin(), endIter = orderedcaloHitList.rend() ;
				endIter != iter ; ++iter)
		{
			if(iter->second->empty())
				continue;

			PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, this->Clean(iter->second));
		}
	}

	return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode GlobalConnectorCleaningAlgorithm::ReadSettings(const pandora::TiXmlHandle xmlHandle)
{
	pandora::AlgorithmTool *pAlgorithmTool = NULL;
	PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, pandora::XmlHelper::ProcessAlgorithmTool(*this, xmlHandle,
			"ReferenceVectorTool", pAlgorithmTool));

	m_pReferenceVectorTool = dynamic_cast<ReferenceVectorToolBase*>(pAlgorithmTool);

	if(NULL == m_pReferenceVectorTool)
		return pandora::STATUS_CODE_INVALID_PARAMETER;

	pAlgorithmTool = NULL;
	PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, pandora::XmlHelper::ProcessAlgorithmTool(*this, xmlHandle,
			"OrderParameterTool", pAlgorithmTool));

	m_pOrderParameterTool = dynamic_cast<OrderParameterToolBase*>(pAlgorithmTool);

	if(NULL == m_pOrderParameterTool)
		return pandora::STATUS_CODE_INVALID_PARAMETER;

	m_cleaningStrategy = 0;
	PANDORA_RETURN_RESULT_IF_AND_IF(pandora::STATUS_CODE_SUCCESS, pandora::STATUS_CODE_NOT_FOUND, !=, pandora::XmlHelper::ReadValue(xmlHandle,
			"CleaningStrategy", m_cleaningStrategy));

	if(2 < m_cleaningStrategy)
		return pandora::STATUS_CODE_INVALID_PARAMETER;

	return pandora::STATUS_CODE_SUCCESS;
}

} 

