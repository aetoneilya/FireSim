#include "AnimationManager.h"

Animation::Animation(sf::Texture &t, int x, int y, int w, int h, int count, float Speed, int step) {
    speed = Speed;
    sprite.setTexture(t);

    currentFrame = 0;
    isPlaying = true;

    for (int i = 0; i < count; i++) {
        frames.push_back(sf::IntRect(x + i * step, y, w, h));
    }
}

Animation::Animation() {
    speed = 0.005;
    currentFrame = 0;
    isPlaying = false;
}

void Animation::tick(float time) {
    if (!isPlaying) return;

    currentFrame += speed * time;

    if (currentFrame > frames.size()) {
        currentFrame -= frames.size();
    }
    int i = currentFrame;
    sprite.setTextureRect(frames[i]);
}

void AnimationManager::create(std::string name, sf::Texture &t, int x, int y, int w, int h, int count,
                              float speed, int step) {
    Animation anim(t, x, y, w, h, count, speed, step);
    animList.emplace(name, anim);
    currentAnim = name;
}

void AnimationManager::draw(sf::RenderWindow &target, int x = 0, int y = 0) {
    animList[currentAnim].sprite.setPosition(x, y);
    target.draw(animList[currentAnim].sprite);
}

void AnimationManager::set(std::string name) { currentAnim = name; }

void AnimationManager::tick(float time) { animList[currentAnim].tick(time); }

void AnimationManager::pause() { animList[currentAnim].isPlaying = false; }

void AnimationManager::play() { animList[currentAnim].isPlaying = true; }