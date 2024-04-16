# TODO
## Current tasks

- [ ] Complete texture support
    - [ ] Add texture coordinates
    - [ ] Fix the shaders so they use texture coords if available
    - [ ] Add shaders from application level NOT in engine.cpp

- [ ] Add text rendering support

- [ ] Include Dear ImGUI
    - [ ] Make it simple on application side to add GUI stuff
        - Wrapper maybe?
    - [ ] Add a debug menu for the engine

## Future tasks

- [ ] Add Line renderer support
  - [ ] Add raycasting for physics objects
  - [ ] Define better the difference between physics and colliders
- [ ] Add Particle System support
- [ ] Add a sound library
  - [ ] Add sound support on application level(wrapper)
- [ ] Create an installer with cmake so that the games can be shipped