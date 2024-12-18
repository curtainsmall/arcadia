#pragma once

namespace Arcadia
{
    class Zombieable
    {
    public:
        virtual auto CheckZombieState() const -> bool
        {
            return _ZombieState;
        }
        virtual void SetZombieState(bool state)
        {
            _ZombieState = state;
        }
    private:
        bool _ZombieState;
    };
}