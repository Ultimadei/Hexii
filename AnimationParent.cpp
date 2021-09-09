#include <vector>

#include "AnimationParent.h"

AnimationParent::AnimationParent() {}

AnimationParent::AnimationParent(AnimationParent&& right) noexcept :
	ParentBase(reinterpret_cast<ParentBase&&>(right))
{}

bool AnimationParent::setAnimation(const std::string& name, AnimationBase* child, bool conditional, const std::string& state) {
	AnimationStatePair* statePair = accessChild(name);

	if (statePair == nullptr) {
		auto createdStatePair = new std::pair<std::string, AnimationMap>(state, AnimationMap());

		createdStatePair->second.emplace(std::make_pair(state, child));

		return setChild(name, createdStatePair);
	}
	else {
		if (conditional) {
			auto found = statePair->second.find(state);
			if (found == statePair->second.end()) { 
				// Delete the parameter to prevent a memory leak
				delete child;
				return false; 
			}
		}
		else {
			removeAnimation(name, state);
		}
	}

	statePair->first = state;
	statePair->second.emplace(std::make_pair(state, child));
	return true;
}

void AnimationParent::switchAnimation(
	const std::string& name, 
	const std::string& state, 
	SwitchMode mode, 
	bool deactivatePrevious,
	float duration, 
	float offset) 
{
	AnimationStatePair* targetPair = accessChild(name);
	if (targetPair == nullptr) { return; }
	AnimationBase* target = accessAnimation(targetPair, state);
	if (target == nullptr) { return; }
	AnimationBase* previous = accessAnimation(targetPair, targetPair->first);

	targetPair->first = state;

	switch (mode) {
	case SwitchMode::IMMITATE:
		targetPair->first = state;
		if (previous != nullptr) {
			if (previous->getActive(false)) { // previous was active, immitate it
				target->activate(duration, previous->getElapsedTime(true) + offset);
			}
			else {
				target->deactivate();
			}
		} 
		break;
	case SwitchMode::IMMITATE_IGNORE:
		targetPair->first = state;
		if (previous != nullptr) {
			if (previous->getActive(false)) { // previous was active, immitate it
				target->activate(duration, previous->getElapsedTime(true) + offset);
			}
		}
		break;
	case SwitchMode::REVERSE_IMMITATE:
		targetPair->first = state;
		if (previous != nullptr) {
			if (!previous->getActive(false)) { // previous was not active
				target->activate(duration, offset);
			}
			else {
				target->deactivate();
			}
		}
		break;
	case SwitchMode::REVERSE_IMMITATE_IGNORE:
		targetPair->first = state;
		if (previous != nullptr) {
			if (!previous->getActive(false)) { // previous was not active
				target->activate(duration, offset);
			}
		}
		break;
	case SwitchMode::DEACTIVATE:
		target->deactivate();
		break;
	case SwitchMode::ACTIVATE:
		target->activate(duration, offset);
		break;
	case SwitchMode::ACTIVATE_IMMITATE_OFFSET:
		target->activate(duration, previous->getElapsedTime(true) + offset);
		break;
	case SwitchMode::ACTIVATE_IMMITATE_INVERSE_OFFSET:
		target->activate(duration, duration - (previous->getElapsedTime(false) + offset));
		break;
	case SwitchMode::IGNORE:
	default:
		break;
	}

	if (deactivatePrevious && previous != nullptr) { previous->deactivate(); }
}

const AnimationBase* AnimationParent::getAnimation(const std::string& name, const std::string& state) const {
	const AnimationStatePair* statePair = getChild(name);

	return getAnimation(statePair, state);
}

AnimationBase* AnimationParent::accessAnimation(const std::string& name, const std::string& state) {
	return const_cast<AnimationBase*>(getAnimation(name, state));
}

void AnimationParent::removeAnimation(const std::string& name, const std::string& state) {
	if (state == "_default_") {
		removeChild(name);
		return;
	}

	AnimationStatePair* statePair = accessChild(name);

	if (statePair != nullptr) {
		auto found = statePair->second.find(state);
		if (found != statePair->second.end()) { // state exists
			delete found->second;
			statePair->second.erase(state);
		}
	}
}

void AnimationParent::updateAnimations(float dt) {
	for (auto i : m_children) {
		AnimationBase* updateTarget = accessAnimation(i.first, i.second->first);

		updateTarget->update(dt);
	}
}

const AnimationBase* AnimationParent::getAnimation(const AnimationStatePair* target, const std::string& state) const {
	if (target != nullptr) {
		auto found = target->second.find(state);
		if (found != target->second.end()) {
			return found->second;
		}
	}

	return nullptr;
}

AnimationBase* AnimationParent::accessAnimation(const AnimationStatePair* target, const std::string& state) {
	return const_cast<AnimationBase*>(getAnimation(target, state));
}
