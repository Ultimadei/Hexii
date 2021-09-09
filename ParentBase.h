#pragma once

#include <map>
#include <string>

// Abstract base class for 'parents' to support an arbitrary number of children of type _chTy
template <typename _chTy>
class ParentBase {
public:
	inline ParentBase() {}
	ParentBase(ParentBase<_chTy>&& right) noexcept;
	ParentBase(const ParentBase<_chTy>&) = delete;

	virtual ~ParentBase();

	// If conditional, the child will only be set as long as the specified name is available
	bool setChild(const std::string& name, _chTy* child, bool conditional = false);
	const _chTy* getChild(const std::string& name) const;
	_chTy* accessChild(const std::string& name);
	void removeChild(const std::string& name);
protected:
	std::map<std::string, _chTy*> m_children;
};