  /// \file ParentClusteringAlgorithm.h
/*
 *
 * ParentClusteringAlgorithm.h header template automatically generated by a class generator
 * Creation date : mer. avr. 8 2015
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


#ifndef PARENTCLUSTERINGALGORITHM_H
#define PARENTCLUSTERINGALGORITHM_H

#include "Pandora/Algorithm.h"

namespace arbor_content
{

/**
 *  @brief  ClusteringParentAlgorithm class
 */
class ClusteringParentAlgorithm : public pandora::Algorithm
{
public:
    /**
     *  @brief  Factory class for instantiating algorithm
     */
    class Factory : public pandora::AlgorithmFactory
    {
    public:
        pandora::Algorithm *CreateAlgorithm() const;
    };

    /**
     *  @brief Default constructor
     */
    ClusteringParentAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    std::string     m_clusteringAlgorithmName;      ///< The name of the clustering algorithm to run
    std::string     m_associationAlgorithmName;     ///< The name of the topological association algorithm to run

    std::string     m_inputCaloHitListName;         ///< The name of the input calo hit list, containing the hits to be clustered
    bool            m_restoreOriginalCaloHitList;   ///< Whether to restore the original calo hit list as the "current" list upon completion

    std::string     m_clusterListName;              ///< The name under which to save the new cluster list
    bool            m_replaceCurrentClusterList;    ///< Whether to subsequently use the new cluster list as the "current" list
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *ClusteringParentAlgorithm::Factory::CreateAlgorithm() const
{
    return new ClusteringParentAlgorithm();
}

}

#endif  //  PARENTCLUSTERINGALGORITHM_H