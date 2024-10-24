//
// Created by carlo on 2024-10-24.
//

#ifndef INPUTSYSTEM_HPP
#define INPUTSYSTEM_HPP

namespace SYSTEMS
{
    class InputSystem
    {
        InputSystem();
        InputSystem* GetInstance()
        {
            if (instance == nullptr)
            {
                instance = new InputSystem;
            }
            return instance;
        }
        static InputSystem* instance;
    };

    InputSystem* InputSystem::instance = nullptr;
   
}

#endif //INPUTSYSTEM_HPP
