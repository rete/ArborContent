/*
 *
 * CaloHit.cc source template automatically generated by a class generator
 * Creation date : lun. mars 30 2015
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


#include "ArborObjects/CaloHit.h"

namespace arbor_content
{

CaloHit::CaloHit(const PandoraApi::CaloHit::Parameters &parameters) :
		pandora::CaloHit(parameters)
{
	m_pCaloHitMetaData = new CaloHitMetaData(this);
}

//------------------------------------------------------------------------------------------------------------------------------------------

// TODO copy the connector list in the new calo hit
CaloHit::CaloHit(const PandoraContentApi::CaloHitFragment::Parameters &parameters) :
		pandora::CaloHit(parameters)

{
	const CaloHit *const pCaloHitCopy = dynamic_cast<const CaloHit *const>(parameters.m_pOriginalCaloHit);

	if(NULL == pCaloHitCopy)
		throw pandora::StatusCodeException(pandora::STATUS_CODE_FAILURE);

	m_pCaloHitMetaData = new CaloHitMetaData(this);
	m_hitTagMap = pCaloHitCopy->m_hitTagMap;
}

//------------------------------------------------------------------------------------------------------------------------------------------

CaloHit::~CaloHit()
{
	PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, ArborContentApi::RemoveAndDeleteAllConnections(this));
	delete m_pCaloHitMetaData;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void CaloHit::ClearTagMap()
{
	m_hitTagMap.reset();
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode CaloHit::SaveReclusterMetaData(const std::string &clusterListName)
{
	ReclusterMetaDataMap::iterator findIter = m_reclusterMetaDataMap.find(clusterListName);

	if(m_reclusterMetaDataMap.end() != findIter)
		return pandora::STATUS_CODE_ALREADY_PRESENT;

	// backup the meta data
	m_reclusterMetaDataMap.insert(ReclusterMetaDataMap::value_type(clusterListName, m_pCaloHitMetaData));
	m_pCaloHitMetaData = new CaloHitMetaData(this);

	return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode CaloHit::EndReclustering(const std::string &selectedClusterListName)
{
	ReclusterMetaDataMap::iterator findIter = m_reclusterMetaDataMap.find(selectedClusterListName);

	if(m_reclusterMetaDataMap.end() == findIter)
		return pandora::STATUS_CODE_NOT_FOUND;

	for(ReclusterMetaDataMap::iterator iter = m_reclusterMetaDataMap.begin(), endIter = m_reclusterMetaDataMap.end() ;
			endIter != iter ; ++iter)
	{
		// adopt the selected meta data
		if(iter->first == selectedClusterListName)
		{
			delete m_pCaloHitMetaData;
			m_pCaloHitMetaData = iter->second;

			continue;
		}

		// get connector list copy
		ConnectorList connectorList(iter->second->GetConnectorList(FORWARD_DIRECTION));

		for(ConnectorList::iterator coIter = connectorList.begin(), coEndIter = connectorList.end() ;
				coEndIter != coIter ; ++coIter)
			delete *coIter;

		delete iter->second;
	}

	m_reclusterMetaDataMap.clear();

	return pandora::STATUS_CODE_SUCCESS;
}

} 

