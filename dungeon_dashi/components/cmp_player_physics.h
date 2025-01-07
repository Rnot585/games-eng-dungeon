#pragma once

#include "cmp_physics.h"

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
	b2Vec2 _size;
	sf::Vector2f _maxVelocity;
	bool _grounded;
	float _groundspeed;

	double _gravityChangeCooldown;
	bool _gravityChangePressedLastFrame;
	double _fireballCooldown;
	bool _fireballPressedLastFrame;
	

	bool isGrounded() const;

public:
	void update(double dt) override;

	explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);

	PlayerPhysicsComponent() = delete;

	bool fireBallUnlocked;
};
