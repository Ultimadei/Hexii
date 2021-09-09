#include "HexiiUpgradeOverlay.h"
#include "TranslationAnimation.h"
#include "ColorAnimation.h"
#include "SpriteElement.h"

#include "ResourceManager.h"
#include "Trig.h"

HexiiUpgradeOverlay::HexiiUpgradeOverlay(const sf::Vector2f& basePosition) :
	m_basePosition(basePosition),
	m_active(false),
	m_upgradeNodes{ {75.0f}, {75.0f}, {75.0f}, {75.0f}, {75.0f}, {75.0f} }
{
	for (unsigned int i = 0; i < 6; i++) {
		m_upgradeNodes[i].setFillColor(sf::Color(255, 255, 255, 0));

		float angle = degreesToRadians(30.0f + 60.0f * ((float)i + 1.0f));

		sf::Vector2f destination(std::sin(angle) * 200.0f, std::cos(angle) * 200.0f);

		SpriteElement* sprite = new SpriteElement;
		m_upgradeNodes[i].setElement("upgradeOverlay.icon", sprite);

		m_upgradeNodes[i].setAnimation("upgradeOverlay.popout.translation", new TranslationAnimation(
			&m_upgradeNodes[i],
			sf::Vector2f(0.0f, 0.0f),
			destination,
			BezierCurvePresets::elasticOut,
			0.0f
		), false, "_default_");

		m_upgradeNodes[i].setAnimation("upgradeOverlay.popout.color", new ColorAnimation(
			{ &m_upgradeNodes[i], sprite },
			sf::Color(255, 255, 255, 0),
			sf::Color(255, 255, 255, 255),
			BezierCurvePresets::linear,
			0.0f
		), false, "_default_");

		m_upgradeNodes[i].setAnimation("upgradeOverlay.popout.translation", new TranslationAnimation(
			&m_upgradeNodes[i],
			sf::Vector2f(0.0f, 0.0f),
			-destination,
			BezierCurvePresets::elasticIn,
			0.0f
		), false, "_reverse_");

		m_upgradeNodes[i].setAnimation("upgradeOverlay.popout.color", new ColorAnimation(
			{ &m_upgradeNodes[i], sprite },
			sf::Color(255, 255, 255, 255),
			sf::Color(255, 255, 255, 0),
			BezierCurvePresets::linear,
			0.0f
		), false, "_reverse_");
	}
}

void HexiiUpgradeOverlay::activate(const Hexagon* target, short availableUpgrades) {
	m_active = true;
	setPosition(m_basePosition + target->getPosition());

	for (unsigned int i = 0; i < 6; i++) {
		m_upgradeNodes[i].setPosition(0.0f, 0.0f);

		m_upgradeNodes[i].switchAnimation(
			"upgradeOverlay.popout.translation", "_default_",
			AnimationParent::SwitchMode::ACTIVATE, true,
			0.75f, 0.0f
		);

		m_upgradeNodes[i].switchAnimation(
			"upgradeOverlay.popout.color", "_default_",
			AnimationParent::SwitchMode::ACTIVATE, true,
			0.35f, 0.0f
		);

		ElementBase* sprite = m_upgradeNodes[i].accessElement("upgradeOverlay.icon");

		sf::Texture* texture = nullptr;
			
		switch (i) {
		case 0:
			//texture = ResourceManager::getTexture("upgradeGreenMatterYieldIcon");
			//break;
		default:
			texture = ResourceManager::getTexture("upgradeLockedIcon");
			break;
		}

		sf::Vector2u textureSize = texture->getSize();

		sprite->setTexture(texture);
		sprite->setOrigin((float)textureSize.x * 0.5f, (float)textureSize.y * 0.5f);
	}
}

void HexiiUpgradeOverlay::deactivate() {
	m_active = false;

	for (unsigned int i = 0; i < 6; i++) {
		m_upgradeNodes[i].switchAnimation(
			"upgradeOverlay.popout.translation", "_reverse_",
			AnimationParent::SwitchMode::ACTIVATE, true,
			0.75f, 0.0f
		);

		m_upgradeNodes[i].switchAnimation(
			"upgradeOverlay.popout.color", "_reverse_",
			AnimationParent::SwitchMode::ACTIVATE, true,
			0.35f, -0.15f
		);
	}
}

void HexiiUpgradeOverlay::setBasePosition(const sf::Vector2f& basePosition) {
	m_basePosition = basePosition;
}

bool HexiiUpgradeOverlay::getActive() const {
	return m_active;
}

float HexiiUpgradeOverlay::getProgress() const {
	if (m_active) { return m_upgradeNodes[0].getAnimation("upgradeOverlay.popout.translation", "_default_")->getElapsedTime(false) / 0.75f; }
	else { return m_upgradeNodes[0].getAnimation("upgradeOverlay.popout.translation", "_reverse_")->getElapsedTime(true) / 0.75f; }
}

void HexiiUpgradeOverlay::update(float dt) {
	for (unsigned int i = 0; i < 6; i++) {
		m_upgradeNodes[i].update(dt);
	}
}

void HexiiUpgradeOverlay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform.combine(getTransform());

	for (unsigned int i = 0; i < 6; i++) {
		target.draw(m_upgradeNodes[i], states);
	}
}