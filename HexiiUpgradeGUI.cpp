#include "HexiiUpgradeGUI.h"
#include "Trig.hpp"
#include "ResourceManager.h"

HexiiUpgradeGUI::HexiiUpgradeGUI() : m_nodes(75.0f, [this]() { return generateUpgradeHex(); }) {
	active = false;

	for (unsigned int i = 0; i < 6; i++) m_nodes.setHex(m_nodes.getIndex(1, PI_THIRDS * i).index);

	std::function<void(const FloatAnimation&)> translation = [this](const FloatAnimation& animation) {applyTranslation(animation); };
	std::function<void(const ColorAnimation&)> color = [this](const ColorAnimation& animation) {applyColor(animation); };

	// Used to set active to false when the animations are complete
	m_animator["deactivate"] = std::make_shared<FloatAnimation>([this](const FloatAnimation& animation) { if (animation.getOutput() == 1.0f) active = false; }, 0.0f, 1.0f, BezierCurves::linear, m_animationDuration);

	m_animator["popoutTranslation"] = std::make_shared<FloatAnimation>(translation, 0.0f, 200.0f, BezierCurves::elasticOut, m_animationDuration);
	m_animator["popoutTranslationReverse"] = std::make_shared<FloatAnimation>(translation, 200.0f, 0.0f, BezierCurves::elasticIn, m_animationDuration);

	m_animator["popoutColor"] = std::make_shared<ColorAnimation>(color, sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 255), BezierCurves::linear, m_animationDuration * 0.6f);
	m_animator["popoutColorReverse"] = std::make_shared<ColorAnimation>(color, sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 0), BezierCurves::linear, m_animationDuration * 0.6f);
}

void HexiiUpgradeGUI::applyTranslation(const FloatAnimation& animation) const {
	float dist = animation.getOutput();
	float angle = 0;

	for (auto& it : m_nodes.hexMap()) {
		it.second->setPosition(sf::Vector2f(sin(angle) * dist, cos(angle) * dist));
		angle += PI_THIRDS;
	}
}

void HexiiUpgradeGUI::applyColor(const ColorAnimation& animation) const {
	for (auto& it : m_nodes.hexMap()) it.second->setFillColor(animation.getOutput());
}

HexiiUpgradeGUI::SPHexagon HexiiUpgradeGUI::generateUpgradeHex() {
	SPHexagon hex = std::make_shared<Hexagon>(75.0f);

	hex->setFillColor(sf::Color(255, 255, 255, 0));

	return hex;
}

void HexiiUpgradeGUI::activate(SPHexagon target, unsigned short availableUpgrades) {
	active = true;
	setPosition(target->getPosition());

	m_animator["popoutTranslationReverse"]->stop();
	m_animator["popoutColorReverse"]->stop();

	m_animator["popoutTranslation"]->play();
	m_animator["popoutColor"]->play(-0.2f * m_animationDuration);

	unsigned short i = 0;
	for (auto& it : m_nodes.hexMap()) {
		SPHexagon hex = it.second;

		SPTexture texture = nullptr;
		
		// TODO: Implement upgrade icons
		switch (i) {
		case 0:
			//texture = ResourceManager::getTexture("upgradeGreenMatterYieldIcon");
			//break;
		default:
			texture = ResourceManager::getTexture("upgradeLockedIcon");
			break;
		}

		sf::Vector2u textureSize = texture->getSize();

		hex->setTexture(texture);
		
		i++;
	}
}

void HexiiUpgradeGUI::deactivate() {
	m_animator["deactivate"]->play();

	m_animator["popoutTranslation"]->stop();
	m_animator["popoutColor"]->stop();

	m_animator["popoutTranslationReverse"]->play();
	m_animator["popoutColorReverse"]->play(-0.2f * m_animationDuration);
}

void HexiiUpgradeGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!active) return;
	states.transform.combine(getTransform());
	target.draw(m_nodes, states);
}