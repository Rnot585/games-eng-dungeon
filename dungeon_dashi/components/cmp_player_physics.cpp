#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>
#include "cmp_bullet.h"
#include "cmp_actor_movement.h"
#include "cmp_hurt_player.h"

using namespace std;
using namespace sf;
using namespace Physics;


static std::map<String, Keyboard::Key> keybindsMap = {
	{"move_Left", Keyboard::A},
	{"move_Right", Keyboard::D},
	//{"move_Up", Keyboard::W},
	//{"move_Down", Keyboard::S},
	{"move_Jump", Keyboard::Space},
	{"shoot", Keyboard::RShift},
	{"move_GravityFlip", Keyboard::Up},
	{"key_Interact", Keyboard::E}
};

bool CharDir = false;    //Direction character is facing --  True=Left / False=Right

//Keyboard::Key keybinds[] = { 
//    Keyboard::A, //Move left
//    Keyboard::D, //Move right
//    Keyboard::W, //Move up
//    Keyboard::S, //Move Down
//    Keyboard::Space, //Jump
//    Keyboard::Up }; //Change gravity

bool PlayerPhysicsComponent::isGrounded() const {
	auto touch = getTouching();
	const auto& pos = _body->GetPosition();
	const float halfPlrHeigt = _size.y * .5f;
	const float halfPlrWidth = _size.x * .52f;
	b2WorldManifold manifold;
	for (const auto& contact : touch) {
		contact->GetWorldManifold(&manifold);
		const int numPoints = contact->GetManifold()->pointCount;
		bool onTop = numPoints > 0;
		bool onBottom = numPoints > 0;
		// If all contacts are below the player.
		for (int j = 0; j < numPoints; j++) {
			onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
		}
		if (onTop) {
			return true;
		}
		// If all contacts are above the player.
		for (int j = 0; j < numPoints; j++) {
			onBottom &= (manifold.points[j].y > pos.y + halfPlrHeigt);
		}
		if (onBottom) {
			return true;
		}
	}

	return false;
}

void PlayerPhysicsComponent::update(double dt) {

	const auto pos = _parent->getPosition();
	auto sprite = _parent->get_components<SpriteComponent>()[0];

	//Teleport to start if we fall off map.
	if (pos.y > ls::getHeight() * ls::getTileSize()) {
		teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	}

	int dir = (Keyboard::isKeyPressed(keybindsMap["move_Left"]) ? -1 : 0) + (Keyboard::isKeyPressed(keybindsMap["move_Right"]) ? 1 : 0);
	if (dir) {
		if (CharDir == dir > 0) {
			sprite->getSprite().scale(-1.f, 1.f);  //Flip sprite Direction
			CharDir = !CharDir;
		}
		impulse({ (float)(dt * _groundspeed * dir), 0 });
		sprite->playAnimation("walk", false);
		//sprite->getSprite().scale({ float(CharDir * -2) + 1, 1 });
	}
	else {
		dampen({ 0.9f, 1.0f });
	}

	//if (Keyboard::isKeyPressed(keybindsMap["move_Left"]) ||
	//	Keyboard::isKeyPressed(keybindsMap["move_Right"])) {
	//	// Moving Either Left or Right
	//	if (Keyboard::isKeyPressed(keybindsMap["move_Right"])) {
	//		if (getVelocity().x < _maxVelocity.x)
	//			if (CharDir == true) {  //Check if character is facing right
	//				
	//			}
	//			else {
	//				sprite->getSprite().scale(-1.f, 1.f);  //Flip sprite Direction
	//				impulse({ (float)(dt * _groundspeed), 0 });
	//				CharDir = false;
	//			}
	//
	//		impulse({ (float)(dt * _groundspeed), 0 });
	//
	//	}
	//	else {
	//		if (getVelocity().x > -_maxVelocity.x)
	//			if (CharDir == false) {  //Check if character is facing left
	//				impulse({ -(float)(dt * _groundspeed), 0 });
	//			}
	//			else {
	//				sprite->getSprite().scale(-1.f, 1.f);  //Flip sprite Direction
	//				impulse({ -(float)(dt * _groundspeed), 0 });
	//				CharDir = true;
	//			}
	//
	//	}
	//
	//	sprite->playAnimation("walk", false);
	//}
	//else {
	//	// Dampen X axis movement
	//	dampen({ 0.9f, 1.0f });
	//}



	// Handle Jump
	if (Keyboard::isKeyPressed(keybindsMap["move_Jump"])) {
		_grounded = isGrounded();
		if (_grounded) {
			setVelocity(Vector2f(getVelocity().x, 0.f));
			teleport(Vector2f(pos.x, pos.y - 2.0f));
			impulse(Vector2f(0, -6.f * _body->GetGravityScale()));
			sprite->playAnimation("jump", true);

		}
	}
	// Handle Fireballs
	if (Keyboard::isKeyPressed(keybindsMap["shoot"]) && _fireballCooldown <= 0 && _fireballPressedLastFrame == false && fireBallUnlocked) {
		auto fireball = _parent->scene->makeEntity();
		
		if (CharDir == true) {
			fireball->setPosition(Vector2f(_parent->getPosition().x - 20.f, _parent->getPosition().y));
		}
		else
		{
			fireball->setPosition(Vector2f(_parent->getPosition().x + 20.f, _parent->getPosition().y));
		}


		fireball->addComponent<BulletComponent>();


		shared_ptr<sf::Texture> fireTex = make_shared<sf::Texture>();
		fireTex->loadFromFile("res/spritesheets/Fireballsprst.png");

		auto s = fireball->addComponent<SpriteComponent>();
		s->setTexure(fireTex);
		s->getSprite().setScale(Vector2f(40.f, 40.f) / 16.f);
		s->getSprite().setOrigin(Vector2f(8.f, 8.f));

		auto p = fireball->addComponent<PhysicsComponent>(true,Vector2f(8.f, 8.f));
		//fireball->setPosition(Vector2f(fireball->getPosition().x + (20.f * dt), fireball->getPosition().y));
		if (CharDir == true) {
			p->impulse(Vector2f(-20.f, 0.f));
		}
		else
		{
			p->impulse(Vector2f(20.f, 0.f));
		}
		
		
		fireball->addComponent<HurtComponent>(true);
		
		


		fireball->get_components<SpriteComponent>()[0]->addAnimation("fire", { 0, 5 });
		fireball->get_components<SpriteComponent>()[0]->playAnimation("fire", false);

		
		sprite->playAnimation("fireball", true);

		_fireballCooldown = 0.5;
		
		
	}
	else if (_fireballCooldown > 0) {
		_fireballCooldown -= dt;
		
	}

	_fireballPressedLastFrame = Keyboard::isKeyPressed(keybindsMap["shoot"]); //Makes it so fireballs can only shoot once per button press

	//Handle Gravity Inversion
	if (Keyboard::isKeyPressed(keybindsMap["move_GravityFlip"]) && _gravityChangeCooldown <= 0 && _gravityChangePressedLastFrame == false) {
		int GravMod = _body->GetGravityScale();
		_body->SetGravityScale(GravMod * -1);   //New function which reverses the Gravity scaling on the player object exclusively

		_gravityChangeCooldown = 0.1;

		sprite->getSprite().scale(1.f, -1.f);  //Flip sprite upside down
	}
	else if (_gravityChangeCooldown > 0) { //Added cooldown to stop gravity changing too quickly
		//std::cout << _gravityChangeCooldown << "\n";
		_gravityChangeCooldown -= dt;
	}

	_gravityChangePressedLastFrame = Keyboard::isKeyPressed(keybindsMap["move_GravityFlip"]); //Makes it so gravity can only change once per button press

	

	//Are we in air?
	if (!isGrounded()) {
		// Check to see if we have landed yet
		_grounded = isGrounded();
		// disable friction while jumping
		setFriction(0.f);


		//std::cout << _grounded << "\n";
		sprite->playAnimation("jump", false);
	}
	else {
		setFriction(0.1f);
	}

	// Clamp velocity.
	auto v = getVelocity();
	v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
	v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
	setVelocity(v);

	PhysicsComponent::update(dt);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
	const Vector2f& size)
	: PhysicsComponent(p, true, size) {
	_size = sv2_to_bv2(size, true);
	_maxVelocity = Vector2f(200.f, 400.f);
	_groundspeed = 30.f;
	_grounded = false;
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(true);
	//Bullet items have higher-res collision detection
	_body->SetBullet(true);
	fireBallUnlocked = false;

	_gravityChangePressedLastFrame = false;
	_gravityChangeCooldown = 0.1;
}
