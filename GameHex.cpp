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

	m_onMouseEnter = [this]() { return mouseEnter(); };
	m_onMouseExit = [this]() {return mouseExit(); };
	m_onMouseClick = [this]() { return mouseClick(); };
	m_onMouseRelease = [this]() { return mouseRelease(); };
}

void GameHex::mouseEnter() {
	if(animator["hoverColor"]) animator["hoverColor"]->play();
	if (animator["hoverColorReverse"]) animator["hoverColorReverse"]->stop();
}

void GameHex::mouseExit() {
	if (animator["hoverColorReverse"]) animator["hoverColorReverse"]->play();
	if (animator["hoverColor"]) animator["hoverColor"]->stop();
}

void GameHex::update(float dt) {
	m_yieldProgress += m_yieldSpeed * (dt / DisplayManager::getTargetFPS()) * (m_held ? m_activeSpeedMultiplier : 1.0f);

	while (m_yieldProgress > 1.0f) {
		m_yieldProgress -= 1.0f;
		m_onYield(*this, m_yieldAmount);
	}

	m_shader->setUniform("progress", m_yieldProgress);
	m_shader->setUniform("centre", sf::Vector2f(DisplayManager::worldToScreen(getPosition(), true)));

	Hexagon::update(dt);
}