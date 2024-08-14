#ifndef PLAYER_COLLIDER_HPP
#define PLAYER_COLLIDER_HPP

#include "Fronge.hpp"

namespace tron
{
	class PlayerCollider final : public fro::Component
	{
	public:
		PlayerCollider() = default;
		PlayerCollider(PlayerCollider const&) = default;
		PlayerCollider(PlayerCollider&&) noexcept = default;

		virtual ~PlayerCollider() override = default;

		PlayerCollider& operator=(PlayerCollider const&) = default;
		PlayerCollider& operator=(PlayerCollider&&) noexcept = default;

	private:
		fro::EventListener<fro::Entity, fro::Component, std::type_index const> mOnComponentAttachEvent
		{
			[smartThis = fro::Reference{ this }](fro::Entity& entity, fro::Component& component, std::type_index const& typeIndex)
			{
				auto& parentingEntity{ smartThis->mParentingEntity };

				if (&*smartThis == &component)
				{
					parentingEntity = entity;
					auto const rigidbody{ parentingEntity->findComponent<fro::Rigidbody>() };
					if (not rigidbody)
						return false;

					rigidbody->mBeginContactEvent.addListener(smartThis->mOnContactBeginEvent);
					return true;
				}

				if (parentingEntity.valid() and &*parentingEntity == &entity and typeIndex == typeid(fro::Rigidbody))
				{
					fro::Rigidbody& rigidbody{ static_cast<fro::Rigidbody&>(component) };
					rigidbody.mBeginContactEvent.addListener(smartThis->mOnContactBeginEvent);
					return true;
				}

				return false;
			}, fro::EntityManager::getComponentAttachEvent()
		};

		fro::EventListener<fro::Entity, fro::Component, std::type_index const> mOnComponentDetachEvent
		{
			[smartThis = fro::Reference{ this }](fro::Entity& entity, fro::Component& component, std::type_index const& typeIndex)
			{
				auto& parentingEntity{ smartThis->mParentingEntity };

				if (&*smartThis == &component)
				{
					auto const rigidbody{ parentingEntity->findComponent<fro::Rigidbody>() };
					if (rigidbody)
						rigidbody->mBeginContactEvent.removeListener(smartThis->mOnContactBeginEvent);

					parentingEntity.reset();
					return true;
				}

				if (parentingEntity.valid() and &*parentingEntity == &entity and typeIndex == typeid(fro::Rigidbody))
				{
					fro::Rigidbody& rigidbody{ static_cast<fro::Rigidbody&>(component) };
					rigidbody.mBeginContactEvent.removeListener(smartThis->mOnContactBeginEvent);
					return true;
				}

				return false;
			}, fro::EntityManager::getComponentDetachEvent()
		};

		fro::EventListener<fro::Collider, fro::Rigidbody, fro::Collider> mOnContactBeginEvent
		{
			[smartThis = fro::Reference{ this }](fro::Collider const&, fro::Rigidbody const&, fro::Collider const& collider)
			{
				if (not (collider.getCategoryBits() & fro::createBitfield(4)) and
					not (collider.getCategoryBits() & fro::createBitfield(5)))
					return false;

				smartThis->mParentingEntity->markDoomed();
				smartThis->mParentingEntity->findComponent<fro::Transform>()->getChildren().front()->getParentingEntity()->markDoomed();
				return true;
			}
		};

		fro::Reference<fro::Entity> mParentingEntity{};
	};
}

#endif