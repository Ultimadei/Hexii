#pragma once

#include "Animation.h"

#include <string>
#include <memory>

// Animator is a component that controls the animations of an object
class Animator final {
private:
	using SPAnimation = Animation::SPAnimation;
	typedef std::map<std::string, SPAnimation> AnimationMap;

	// This is a slightly fancy way of allowing syntax such as ` animation["animation_name"] = ...` even if animation name doesn't exist yet
	struct AnimationMapSelection {
	private:
		// Only `Animatable` can instantiate a selection because the selection is dependant on the animation map
		friend class Animator;
		inline AnimationMapSelection(AnimationMap& map, const std::string& key) : m_map(map), m_key(key), m_it(map.find(key)) {}
		AnimationMapSelection(AnimationMap&&, const std::string&) = delete; // Prevents improper usage through rvalue binding
		AnimationMapSelection(const AnimationMap&) = delete; // No copy constructor so the selection can't be assigned to a variable
		AnimationMapSelection(AnimationMap&&) = delete;

	public:
		inline std::string getKey() const { return m_key; }

		inline SPAnimation get() const {
			if (m_it != m_map.end()) return m_it->second;
			else return nullptr;
		}
		inline void remove() { if (m_it != m_map.end()) m_map.erase(m_it); }

		inline SPAnimation operator =(SPAnimation right) {
			if (m_it != m_map.end()) m_it->second = right;
			else m_map.emplace(std::make_pair(m_key, right));

			return right;
		}

		inline SPAnimation operator ->() const { return get(); }
		// TODO: Review this warning
		inline Animation& operator *() const { return *get(); }

		/// Implicit casts

		operator SPAnimation () const { return get(); }
		operator bool() const { return get() != nullptr; }

	private:
		const std::string m_key;
		const AnimationMap::iterator m_it;
		AnimationMap& m_map;

		inline const AnimationMap::iterator it() const { return m_map.find(m_key); }
	};

	typedef std::shared_ptr<AnimationMapSelection> SPAnimationMapSelection;

public:
	inline Animator() {}

	inline const AnimationMapSelection operator [](const std::string& key) const { return get(key); }
	inline AnimationMapSelection operator [](const std::string& key) { return get(key); }

	inline AnimationMapSelection get(const std::string& key) const {
		// Else it's implied that the key matches the cached selection so no updating is necessary
		if (m_cachedSelection == nullptr || m_cachedSelection->getKey() != key) m_cachedSelection = std::make_shared<AnimationMapSelection>(AnimationMapSelection(m_map, key));
		return *m_cachedSelection;
	}

	inline void update(float dt) { for (auto i : m_map) i.second->update(dt); }

private:
	mutable SPAnimationMapSelection m_cachedSelection;
	mutable AnimationMap m_map;
};