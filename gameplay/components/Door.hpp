//
// Created by sebastian on 19.04.26.
//

#pragma once

namespace gameplay::components {
    struct Door {
    public:
        enum class State {
            Closed,
            Opening,
            Opened,
            Closing,
        };

        State CurrentState = State::Closed;
    };
}
