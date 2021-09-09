#include "ParentBase.h"

#include "AnimationBase.h"
#include "ElementBase.h"

template<typename _chTy>
ParentBase<_chTy>::ParentBase(ParentBase<_chTy>&& right) noexcept :
	m_children(reinterpret_cast<std::map<std::string, _chTy*>&&>(right.m_children))
{}

template<typename _chTy>
ParentBase<_chTy>::~ParentBase() {
	for (auto i : m_children) {
		delete i.second;
	}
	m_children.clear();
}

template<typename _chTy>
bool ParentBase<_chTy>::setChild(const std::string& name, _chTy* child, bool conditional) {
	if (conditional) {
		// If the name exists, this function should not overwrite it because conditional is true
		if (getChild(name) == nullptr) {
			// The argument should be deleted to prevent a memory leak
			delete child;
			return false;
		}
	}
	else {
		// Checks for name and removes it if it exists
		removeChild(name);
	}

	m_children.emplace(std::make_pair(name, child));
	return true;
}

template<typename _chTy>
const _chTy* ParentBase<_chTy>::getChild(const std::string& name) const {
	auto found = m_children.find(name);
	if (found != m_children.end()) { // The name was found
		return found->second;
	}

	return nullptr;
}

template<typename _chTy>
_chTy* ParentBase<_chTy>::accessChild(const std::string& name) {
	return const_cast<_chTy*>(getChild(name));
}

template<typename _chTy>
void ParentBase<_chTy>::removeChild(const std::string& name) {
	auto found = m_children.find(name);
	if (found != m_children.end()) { // The name was found
		delete found->second;
		m_children.erase(found);
	}
}
 
/// ParentBase implementation using Animation children

template ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>::ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>();
template ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>::ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>(ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>&&) noexcept;
template ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>::~ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>();
template bool ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>::setChild(const std::string& name, std::pair<std::string, std::map<std::string, AnimationBase*>>* child, bool conditional);
template const std::pair<std::string, std::map<std::string, AnimationBase*>>* ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>::getChild(const std::string& name) const;
template std::pair<std::string, std::map<std::string, AnimationBase*>>* ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>::accessChild(const std::string& name);
template void ParentBase<std::pair<std::string, std::map<std::string, AnimationBase*>>>::removeChild(const std::string& name);

/// ParentBase implementation using ElementBase children

template ParentBase<ElementBase>::ParentBase<ElementBase>();
template ParentBase<ElementBase>::ParentBase<ElementBase>(ParentBase<ElementBase>&&) noexcept;
template ParentBase<ElementBase>::~ParentBase<ElementBase>();
template bool ParentBase<ElementBase>::setChild(const std::string& name, ElementBase* child, bool conditional);
template const ElementBase* ParentBase<ElementBase>::getChild(const std::string& name) const;
template ElementBase* ParentBase<ElementBase>::accessChild(const std::string& name);
template void ParentBase<ElementBase>::removeChild(const std::string& name);