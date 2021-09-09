#pragma once

#include "ParentBase.h"
#include "AnimationBase.h"

// Supports having an arbitrary number of animation 'children'
// AnimationParents actually have maps of animation 'states' that can be accessed, rather than just a straight animation
class AnimationParent :
	public ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>> {
private:
	typedef std::map<std::string, AnimationBase*> AnimationMap;
	typedef std::pair<std::string, AnimationMap> AnimationStatePair;
public:
	enum class SwitchMode {
		IGNORE,
		IMMITATE,
		IMMITATE_IGNORE,
		REVERSE_IMMITATE,
		REVERSE_IMMITATE_IGNORE,
		DEACTIVATE,
		ACTIVATE,
		ACTIVATE_IMMITATE_OFFSET,
		ACTIVATE_IMMITATE_INVERSE_OFFSET
	};
public:
	AnimationParent();
	AnimationParent(AnimationParent&& right) noexcept;
	AnimationParent(const AnimationParent& right) = delete;

	// Adds 'target' using 'state' and 'name' (and sets that state to be the updating one)
	bool setAnimation(const std::string& name, AnimationBase* target, bool conditional = false, const std::string& state = "_default_");

	// Switches 'name' to 'state' animation, lasting for 'duration'. 
	// Note: If 'duration' may differ from the previous duration, do not use the IMMITATE SwitchModes
	// SwitchMode::IGNORE will change the updating state but modify neither
	// SwitchMode::IMMITATE will use ACTIVATE_IMMITATE_OFFSET if the previous state was active \
	//	otherwise it will use DEACTIVATE. (duration is ignored in this case)
	// SwitchMode::IMMITATE_IGNORE is the same as IMMITATE except it will IGNORE if the previous state was not active \
	// 	instead of DEACTIVATE
	// SwitchMode::REVERSE_IMMITATE will use ACTIVATE if the previous state was not active \
	// 	otherwise it will use DEACTIVATE.
 	// SwitchMode::REVERSE_IMMITATE_IGNORE will use ACTIVATE if the previous state was not active \
	// 	otherwise it will use IGNORE
	// SwitchMode::DEACTIVATE will switch the updating state and deactivate it (duration and offset are ignored in this case)
	// SwitchMode::ACTIVATE will switch the updating state and activate it with 'duration' and 'offset'
	// SwitchMode::ACTIVATE_IMMITATE_OFFSET will switch the updating state and activate it with 'duration' and \
	//	the previous state's progress as offset, plus 'offset'
	// SwitchMode::ACTIVATE_IMMITATE_INVERSE_OFFSET is the same as ACTIVATE_IMMITATE_OFFSET except offset = 'duration' - offset
	// If deactivatePrevious is true, the previous state will be deactivated. Otherwise it will be ignored
	void switchAnimation(
		const std::string& name,
		const std::string& state,
		SwitchMode mode = SwitchMode::IGNORE,
		bool deactivatePrevious = true,
		float duration = 0.0f,
		float offset = 0.0f
	);
	
	const AnimationBase* getAnimation(const std::string& name, const std::string& state = "_default_") const;
	// The animation returned will be updating 'state' after this call
	AnimationBase* accessAnimation(const std::string& name, const std::string& state = "_default_");
	
	// If state "_default_" is passed, all of 'name' animations are removed. Otherwise, just 'name.state'
	void removeAnimation(const std::string& name, const std::string& state = "_default_");
protected:
	void updateAnimations(float dt);
private:
	/// Utility functions

	const AnimationBase* getAnimation(const AnimationStatePair* target, const std::string& state = "_default_") const;
	AnimationBase* accessAnimation(const AnimationStatePair* target, const std::string& state = "_default_");
};