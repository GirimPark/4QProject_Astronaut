#pragma once
#include "Singleton.h"
class UIFunctor;

class UIFunctorFactory
	: public Singleton<UIFunctorFactory>
{
public:
	template<typename FunctorType>
	std::weak_ptr<FunctorType> GetFunctor();

    virtual void Finalize() override;

private:
	std::unordered_map<const std::type_info*, std::shared_ptr<UIFunctor>> m_functors;
};

template<typename FunctorType>
inline std::weak_ptr<FunctorType> UIFunctorFactory::GetFunctor()
{
    auto it = m_functors.find(&typeid(FunctorType));

    if (it != m_functors.end())
    {
        return std::static_pointer_cast<FunctorType>(it->second);
    }
    else
    {
        auto functor = std::make_shared<FunctorType>();
        m_functors[&typeid(FunctorType)] = functor;
        return functor;
    }
}
