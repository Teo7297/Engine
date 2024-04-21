# TODO
## Current tasks

- [ ] Complete texture support
    - [x] Add texture coordinates
    - [x] Fix the shaders so they use texture coords if available
    - [x] Add shaders from application level NOT in engine.cpp
    - [x] Make textures optional


- [ ] Add text rendering support
  - [ ] Batch render text
- [ ] Include Dear ImGUI
    - [ ] Make it simple on application side to add GUI stuff
        - Wrapper maybe?
    - [ ] Add a debug menu for the engine

## Future tasks

- [ ] Batch rendering of arbitrary objects
- [ ] Add Line renderer support
  - [ ] Add raycasting for physics objects
  - [ ] Define better the difference between physics and colliders
- [ ] Add Particle System support
- [ ] Add a sound library
  - [ ] Add sound support on application level(wrapper)
- [ ] Create an installer with cmake so that the games can be shipped