  /// \file ReferenceVectorTools.cc
/*
 *
 * ReferenceVectorTools.cc source template automatically generated by a class generator
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


#include "ArborUtility/ReferenceVectorTools.h"

#include "Helpers/XmlHelper.h"

namespace arbor_content
{

pandora::StatusCode SimpleReferenceVectorTool::ComputeReferenceVector(const CaloHit *const pCaloHit, pandora::CartesianVector &referenceVector) const
{
	referenceVector = pandora::CartesianVector(0, 0, 0);

	pandora::CartesianVector meanForwardDirection(0, 0, 0);
	pandora::CartesianVector meanBackwardDirection(0, 0, 0);

	PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, this->GetMeanDirection(pCaloHit, FORWARD_DIRECTION, meanForwardDirection,
			m_forwardConnectorWeight, m_referenceDirectionDepth, pCaloHit->GetPseudoLayer()));
	PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, this->GetMeanDirection(pCaloHit, BACKWARD_DIRECTION, meanBackwardDirection,
			m_backwardConnectorWeight, m_referenceDirectionDepth, pCaloHit->GetPseudoLayer()));

	if(meanForwardDirection == meanBackwardDirection)
		return pandora::STATUS_CODE_FAILURE;

	referenceVector = meanBackwardDirection - meanForwardDirection;

	return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode SimpleReferenceVectorTool::GetMeanDirection(const CaloHit *const pCaloHit, ConnectorDirection connectorDirection,
		 pandora::CartesianVector &direction, float weight, unsigned int depth, unsigned initialPseudoLayer) const
{
	if(NULL == pCaloHit)
		return pandora::STATUS_CODE_FAILURE;

	if(0 == depth)
		return pandora::STATUS_CODE_SUCCESS;

	const ConnectorList &connectorList(ArborContentApi::GetConnectorList(pCaloHit, connectorDirection));

	if(connectorList.empty())
		return pandora::STATUS_CODE_SUCCESS;

	const unsigned int pseudoLayer = pCaloHit->GetPseudoLayer();
	const pandora::CartesianVector &positionVector(pCaloHit->GetPositionVector());

	for(ConnectorList::const_iterator iter = connectorList.begin() , endIter = connectorList.end() ;
			endIter != iter ; ++iter)
	{
		const Connector *const pConnector = *iter;
		const CaloHit *const pConnectedCaloHit = pConnector->Get(connectorDirection);

		const unsigned int connectedCaloHitPseudoLayer = pConnectedCaloHit->GetPseudoLayer();
		const unsigned int pseudoLayerDifference = std::max(pseudoLayer, connectedCaloHitPseudoLayer) - std::min(pseudoLayer, connectedCaloHitPseudoLayer);
		const unsigned int maxPseudoLayerDifference = std::max(initialPseudoLayer, connectedCaloHitPseudoLayer) - std::min(initialPseudoLayer, connectedCaloHitPseudoLayer);

		if(pseudoLayerDifference > maxPseudoLayerDifference)
			continue;

		pandora::CartesianVector differencePosition(pConnectedCaloHit->GetPositionVector() - positionVector);
		direction += differencePosition * weight;

		PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, this->GetMeanDirection(pConnectedCaloHit, connectorDirection,
				direction, weight, depth-1, initialPseudoLayer));
	}

	return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode SimpleReferenceVectorTool::ReadSettings(const pandora::TiXmlHandle xmlHandle)
{
	m_backwardConnectorWeight = 2.f;
	PANDORA_RETURN_RESULT_IF_AND_IF(pandora::STATUS_CODE_SUCCESS, pandora::STATUS_CODE_NOT_FOUND, !=, pandora::XmlHelper::ReadValue(xmlHandle,
			"BackwardConnectorWeight", m_backwardConnectorWeight));

	m_forwardConnectorWeight = 3.f;
	PANDORA_RETURN_RESULT_IF_AND_IF(pandora::STATUS_CODE_SUCCESS, pandora::STATUS_CODE_NOT_FOUND, !=, pandora::XmlHelper::ReadValue(xmlHandle,
			"ForwardConnectorWeight", m_forwardConnectorWeight));

	m_referenceDirectionDepth = 1;
	PANDORA_RETURN_RESULT_IF_AND_IF(pandora::STATUS_CODE_SUCCESS, pandora::STATUS_CODE_NOT_FOUND, !=, pandora::XmlHelper::ReadValue(xmlHandle,
			"ReferenceDirectionDepth", m_referenceDirectionDepth));

	m_referenceDirectionMaximumForwardLayer = std::numeric_limits<unsigned int>::max();
	PANDORA_RETURN_RESULT_IF_AND_IF(pandora::STATUS_CODE_SUCCESS, pandora::STATUS_CODE_NOT_FOUND, !=, pandora::XmlHelper::ReadValue(xmlHandle,
			"ReferenceDirectionMaximumForwardLayer", m_referenceDirectionMaximumForwardLayer));

	if(m_referenceDirectionDepth == 0 || m_referenceDirectionMaximumForwardLayer == 0)
		return pandora::STATUS_CODE_INVALID_PARAMETER;

	return pandora::STATUS_CODE_SUCCESS;
}

} 

