
/*
 *  TimerBase.cpp
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

#include "Macros.hpp"
#include "Log.hpp"
#include <sfeMovie/TimerBase.hpp>

namespace sfe
{
    TimerBase::Observer::Observer()
    {
    }

    TimerBase::Observer::~Observer()
    {
    }

    void TimerBase::Observer::willPlay(const TimerBase& timer)
    {
    }

    void TimerBase::Observer::didPlay(const TimerBase& timer, Status previousStatus)
    {
    }

    void TimerBase::Observer::didPause(const TimerBase& timer, Status previousStatus)
    {
    }

    void TimerBase::Observer::didStop(const TimerBase& timer, Status previousStatus)
    {
    }

    bool TimerBase::Observer::didSeek(const TimerBase& timer, sf::Time oldPosition)
    {
        return true;
    }

    TimerBase::~TimerBase()
    {
    }

    void TimerBase::addObserver(Observer& anObserver, int priority)
    {
        CHECK(m_observers.find(&anObserver) == m_observers.end(), "TimerBase::addObserver() - cannot add the same observer twice");

        m_observers.insert(std::make_pair(&anObserver, priority));
        m_observersByPriority[priority].insert(&anObserver);
    }

    void TimerBase::removeObserver(Observer& anObserver)
    {
        std::map<Observer*, int>::iterator it = m_observers.find(&anObserver);

        if (it == m_observers.end())
        {
            sfeLogWarning("TimerBase::removeObserver() - removing an unregistered observer. Ignored.");
        }
        else
        {
            m_observersByPriority[it->second].erase(&anObserver);
            m_observers.erase(it);
        }
    }

    void TimerBase::play()
    {
        CHECK(getStatus() != Playing, "TimerBase::play() - playing twice");
        notifyObservers(Playing);
        Status oldStatus = getStatus();
        m_status = Playing;
        onPlay();
        notifyObservers(oldStatus, getStatus());
    }

    void TimerBase::pause()
    {
        CHECK(getStatus() != Paused, "TimerBase::pause() - paused twice");
        Status oldStatus = getStatus();
        m_status = Paused;
        onPause();
        notifyObservers(oldStatus, getStatus());
    }

    void TimerBase::stop()
    {
        CHECK(getStatus() != Stopped, "TimerBase::stop() - stopped twice");
        Status oldStatus = getStatus();
        m_status = Stopped;
        onStop();
        notifyObservers(oldStatus, getStatus());
        seek(sf::Time::Zero);
    }

    bool TimerBase::seek(sf::Time position)
    {
        Status oldStatus = getStatus();
        sf::Time oldPosition = getOffset();
        bool couldSeek = false;

        if (oldStatus == Playing)
            pause();

        onSeek(position);
        couldSeek = notifyObservers(oldPosition);

        if (oldStatus == Playing)
            play();

        return couldSeek;
    }

    Status TimerBase::getStatus() const
    {
        return m_status;
    }

    void TimerBase::notifyObservers(Status futureStatus)
    {
        for (std::pair<int, std::set<Observer*> >&& pairByPriority : m_observersByPriority)
        {
            for (Observer* observer : pairByPriority.second)
            {
                switch(futureStatus)
                {
                    case Playing:
                        observer->willPlay(*this);
                        break;

                    default:
                        CHECK(false, "TimerBase::notifyObservers() - unhandled case in switch");
                }
            }
        }
    }

    void TimerBase::notifyObservers(Status oldStatus, Status newStatus)
    {
        CHECK(oldStatus != newStatus, "TimerBase::notifyObservers() - inconsistency: no change happened");

        for (std::pair<int, std::set<Observer*> >&& pairByPriority : m_observersByPriority)
        {
            for (Observer* observer : pairByPriority.second)
            {
                switch(newStatus)
                {
                    case Playing:
                        observer->didPlay(*this, oldStatus);
                        break;

                    case Paused:
                        observer->didPause(*this, oldStatus);
                        break;

                    case Stopped:
                        observer->didStop(*this, oldStatus);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    bool TimerBase::notifyObservers(sf::Time oldPosition)
    {
        CHECK(getStatus() != Playing, "inconsistency in timer");
        bool successfullSeeking = true;

        for (std::pair<int, std::set<Observer*> >&& pairByPriority : m_observersByPriority)
        {
            for (Observer* observer : pairByPriority.second)
            {
                if (! observer->didSeek(*this, oldPosition))
                    successfullSeeking = false;
            }
        }

        return successfullSeeking;
    }

}
