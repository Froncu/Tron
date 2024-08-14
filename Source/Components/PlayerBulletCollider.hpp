#ifndef PLAYER_BULLET_COLLIDER_HPP
#define PLAYER_BULLET_COLLIDER_HPP

#include "Fronge.hpp"

namespace tron
{
	class PlayerBulletCollider final : public fro::Component
	{
	public:
		PlayerBulletCollider() = default;
		PlayerBulletCollider(PlayerBulletCollider const&) = default;
		PlayerBulletCollider(PlayerBulletCollider&&) noexcept = default;

		virtual ~PlayerBulletCollider() override = default;

		PlayerBulletCollider& operator=(PlayerBulletCollider const&) = default;
		PlayerBulletCollider& operator=(PlayerBulletCollider&&) noexcept = default;

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
			[smartThis = fro::Reference{ this }](fro::Collider const&, fro::Rigidbody const& rigibody, fro::Collider const&)
			{
				if (rigibody.getType() not_eq fro::Rigidbody::Type::STATIC)
				{
					smartThis->mParentingEntity->markDoomed();
					return true;
				}

				++smartThis->mBounceCount;
				if (smartThis->mBounceCount == 5)
					smartThis->mParentingEntity->markDoomed();

				return true;
			}
		};

		fro::Reference<fro::Entity> mParentingEntity{};
		std::size_t mBounceCount{};
	};
}

#endif