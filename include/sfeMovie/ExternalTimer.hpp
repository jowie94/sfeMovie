
/*
 *  ExternalTimer.hpp
 *  sfeMovie project
 *
 *  Copyright (C) 2010-2015 Lucas Soltic
 *  lucas.soltic@orange.fr
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#ifndef SFEMOVIE_EXTERNALTIMER_HPP
#define SFEMOVIE_EXTERNALTIMER_HPP

#include <sfeMovie/TimerBase.hpp>
#include <SFML/System.hpp>

namespace sfe
{
    class ExternalTimer : public TimerBase
    {
    public:
        void setExternalOffset(sf::Time offset);
        sf::Time getOffset() const override;

    protected:
        void onStop() override;
        void onSeek(sf::Time position) override;

    private:
        sf::Time m_currentOffset{sf::Time::Zero};
    };
}

#endif
