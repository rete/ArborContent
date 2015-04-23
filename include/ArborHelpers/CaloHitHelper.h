  /// \file CaloHitHelper.h
/*
 *
 * CaloHitHelper.h header template automatically generated by a class generator
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


#ifndef CALOHITHELPER_H
#define CALOHITHELPER_H

#include "Pandora/PandoraInternal.h"
#include "Pandora/StatusCodes.h"

#include "ArborObjects/CaloHit.h"
#include "ArborObjects/Connector.h"

namespace pandora { class Algorithm; class CaloHit; }

namespace arbor_content
{

/** 
 * @brief CaloHitHelper class
 */ 
class CaloHitHelper 
{
public:
	/**
	 *
	 */
	static pandora::StatusCode RemoveCurrentConnections(const pandora::Algorithm &algorithm);

	/**
	 *
	 */
	static pandora::StatusCode RemoveConnections(const pandora::CaloHitList *const pCaloHitList);

	/**
	 *
	 */
	static pandora::StatusCode ExtractCurrentSeedCaloHitList(const pandora::Algorithm &algorithm, pandora::CaloHitList &seedCaloHitList, bool discriminateLeafHits = false);

	/**
	 *
	 */
	static pandora::StatusCode ExtractSeedCaloHitList(const pandora::CaloHitList *const pCaloHitList, pandora::CaloHitList &seedCaloHitList, bool discriminateLeafHits = false);

	/**
	 *
	 */
	static pandora::StatusCode ExtractCurrentLeafCaloHitList(const pandora::Algorithm &algorithm, pandora::CaloHitList &leafCaloHitList);

	/**
	 *
	 */
	static pandora::StatusCode ExtractLeafCaloHitList(const pandora::CaloHitList *const pCaloHitList, pandora::CaloHitList &leafCaloHitList);

	/**
	 *
	 */
	static pandora::StatusCode BuildCaloHitList(const CaloHit *const pCaloHit, ConnectorDirection direction, pandora::CaloHitList &calohitList);

private:
};

} 

#endif  //  CALOHITHELPER_H
