
/*
 *  TimerBase.hpp
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

#ifndef SFEMOVIE_TIMERBASE_HPP
#define SFEMOVIE_TIMERBASE_HPP

#include <map>
#include <memory>
#include <set>
#include <SFML/System.hpp>
#include <sfeMovie/Movie.hpp>

namespace sfe
{
    class TimerBase
    {
    public:
        class Observer
        {
        public:
            Observer();
            virtual ~Observer();
            virtual void willPlay(const TimerBase& timer);
            virtual void didPlay(const TimerBase& timer, Status previousStatus);
            virtual void didPause(const TimerBase& timer, Status previousStatus);
            virtual void didStop(const TimerBase& timer, Status previousStatus);
            virtual bool didSeek(const TimerBase& timer, sf::Time oldPosition);
        };

        virtual ~TimerBase();

        void addObserver(Observer& anObserver, int priority = 0);
        void removeObserver(Observer& anObserver);

        void play();
        void pause();
        void stop();
        bool seek(sf::Time position);
        Status getStatus() const;

        virtual sf::Time getOffset() const = 0;

    protected:
        virtual void onPlay() {}
        virtual void onPause() {}
        virtual void onStop() {}
        virtual void onSeek(sf::Time position) { (void)position; }

        Status m_status = Stopped;

    private:
        void notifyObservers(Status newStatus);
        void notifyObservers(Status oldStatus, Status newStatus);
        bool notifyObservers(sf::Time oldPosition);

        std::map<Observer*, int> m_observers;
        std::map<int, std::set<Observer*>> m_observersByPriority;
    };
}

#endif
