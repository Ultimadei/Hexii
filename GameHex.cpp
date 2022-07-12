#include "GameHex.h"

#include "ResourceManager.h"
#include "DisplayManager.h"

GameHex::GameHex(float size, sf::Vector2f initialPosition) :
	Hexagon(size, initialPosition),
	m_yieldProgress(0.0f),
	m_yieldSpeed(0.5f),
	m_activeSpeedMultiplier(3.0f),
	m_yieldAmount(1),
	m_held(false)
{
	setShader(ResourceManager::getShader("hex"));
	setTexture(ResourceManager::getTexture("hex"));

	m_shader->setUniform("progressBar", *ResourceManager::getTexture("hexProgressBar"));
}

void GameHex::onMouseEnter() {
	switchAnimation(
		"hover.color", "_default_",
		AnimationParent::SwitchMode::ACTIVATE, true,
		0.5f, 0.0f
	);
}

void GameHex::onMouseExit() {
	switchAnimation(
		"hover.color", "_reverse_",
		AnimationParent::SwitchMode::ACTIVATE_IMMITATE_INVERSE_OFFSET, true,
		0.5f, 0.0f
	);
}

void GameHex::onMouseClick() {
	m_held = true;
}

void GameHex::onMouseRelease() {
	m_held = false;
}

void GameHex::update(float dt) {
	m_yieldProgress += m_yieldSpeed * (dt / 60.0f) * (m_held ? m_activeSpeedMultiplier : 1.0f);

	while (m_yieldProgress > 1.0f) {
		m_yieldProgress -= 1.0f;
		m_onYield(this, m_yieldAmount);
	}

	m_shader->setUniform("progress", m_yieldProgress);
	m_shader->setUniform("centre", sf::Vector2f(DisplayManager::worldToScreen(getPosition(), true)));
}