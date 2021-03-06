  /// \file ArborBFieldPlugin.cc
/*
 *
 * ArborBFieldPlugin.cc source template automatically generated by a class generator
 * Creation date : sam. mars 21 2015
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
 * @author John Marshall
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ArborPlugins/ArborBFieldPlugin.h"

namespace arbor_content
{

ArborBFieldPlugin::ArborBFieldPlugin(const float innerBField, const float muonBarrelBField, const float muonEndCapBField) :
    m_innerBField(innerBField),
    m_muonBarrelBField(muonBarrelBField),
    m_muonEndCapBField(muonEndCapBField),
    m_muonEndCapInnerZ(std::numeric_limits<float>::max()),
    m_coilMidPointR(std::numeric_limits<float>::max())
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

float ArborBFieldPlugin::GetBField(const pandora::CartesianVector &positionVector) const
{
    if (std::fabs(positionVector.GetZ()) >= m_muonEndCapInnerZ)
        return m_muonEndCapBField;

    if (std::sqrt(positionVector.GetX() * positionVector.GetX() + positionVector.GetY() * positionVector.GetY()) >= m_coilMidPointR)
        return m_muonBarrelBField;

    return m_innerBField;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode ArborBFieldPlugin::Initialize()
{
    try
    {
        m_muonEndCapInnerZ = this->GetPandora().GetGeometry()->GetSubDetector(pandora::MUON_ENDCAP).GetInnerZCoordinate();
        m_coilMidPointR = (0.5f * (this->GetPandora().GetGeometry()->GetSubDetector(pandora::COIL).GetInnerRCoordinate() +
            this->GetPandora().GetGeometry()->GetSubDetector(pandora::COIL).GetOuterRCoordinate()));
    }
    catch (pandora::StatusCodeException &statusCodeException)
    {
        std::cout << "ArborBFieldPlugin: Unable to extract Muon EndCap and Coil geometry." << std::endl;
        return statusCodeException.GetStatusCode();
    }

    return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode ArborBFieldPlugin::ReadSettings(const pandora::TiXmlHandle /*xmlHandle*/)
{
	return pandora::STATUS_CODE_SUCCESS;
}

}
